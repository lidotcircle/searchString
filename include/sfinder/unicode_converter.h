#ifndef _UNICODE_CONVERTER_H_
#define _UNICODE_CONVERTER_H_

#include <string>
#include <vector>


std::vector<int> decoder_utf8(const std::string& str, size_t& n);
std::string      encoder_utf8(const std::vector<int>& str, size_t& n);

std::vector<int> decoder_utf16(const std::string& str, size_t& n, bool little_endian = false);
std::vector<int> encoder_utf16(const std::string& str, size_t& n, bool little_endian = false);

std::vector<int> gb2312_to_unicode(const std::vector<int>& gb2312str);
std::vector<int> unicode_to_gb2312(const std::vector<int>& gb2312str);

bool is_desired_unicode_code_point(int val);

#endif // _UNICODE_CONVERTER_H_
