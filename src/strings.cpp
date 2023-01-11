#include <algorithm>
#include <fwjson/strings.h>
#include <string>
#include <string_view>

namespace fwjson::strings {

namespace {

using CharT = std::string::value_type;

constexpr auto g_deltaZ = 'Z' - 'z';

constexpr std::string_view g_strTrue = "true";
constexpr std::string_view g_strFalse = "false";

constexpr CharT asciiToLower(CharT in)
{
	return in >= 'A' && in <= 'Z' ? in - g_deltaZ : in;
}

#define CHECK_STR(value, constant, result) \
	if (value == constant) { \
		if (bOk) (*bOk) = true; \
		return result; \
	}

} // namespace

bool toBool(const std::string& value, bool* bOk)
{
	if (!value.empty()) {
		CHECK_STR(value, g_strTrue, true)
		CHECK_STR(value, g_strFalse, false)

		auto tmp = value;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), asciiToLower);
		CHECK_STR(tmp, g_strTrue, true)
		CHECK_STR(tmp, g_strFalse, false)
	}
	if(bOk) (*bOk) = false;
	return false;
}

std::string fromBool(bool value)
{
	const auto& result = value ? g_strTrue : g_strFalse;
	return std::string(result);
}

std::string_view trimLeft(const std::string& input)
{
	auto it = std::find_if(input.cbegin(), input.cend(), [](auto ch){
		return !std::isspace(ch);
	});
	return { it, input.end() };
}

std::string_view trimRight(const std::string& input)
{
	auto it = std::find_if(input.crbegin(), input.crend(), [](auto ch){
		return !std::isspace(ch);
	});
	return { input.begin(), it.base() };
}

std::string_view trim(const std::string& input)
{
	std::string_view left = trimLeft(input);
	std::string_view right = trimRight(input);
	if (left.begin() == left.end() || right.begin() == right.end()) {
		return std::string_view();
	}
	return { left.begin(), right.end() };
}

} // namespace fwjson::strings