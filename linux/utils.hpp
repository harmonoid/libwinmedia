#include <codecvt>
#include <locale>
#include <string>

#ifndef UTILS_H_
#define UTILS_H_
inline std::wstring ConvertToWideString(const std::string& string) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
  return converter.from_bytes(string);
}

inline std::string ConvertToString(const std::wstring& wide_string) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
  return converter.to_bytes(wide_string);
}
#endif
