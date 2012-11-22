#ifndef SERIALIZE_EXCEPTION_HANDLING_HPP
#define SERIALIZE_EXCEPTION_HANDLING_HPP

#include <iostream>
#include <string>

bool check_eof(istream& oiStream);
bool check_fail(istream& oiStream);

bool read_and_check_types(istream& oiStream, char &char_data);
bool read_and_check_types(istream& oiStream, int &int_data);
bool read_and_check_types(istream& oiStream, double &double_data);
bool read_and_check_types(istream& oiStream, std::string &string_data);

#endif