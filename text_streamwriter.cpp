#include <iostream>

#include "text_streamwriter.hpp"

using namespace std;

TextStreamWriter::TextStreamWriter(): stream(0)
{
}

TextStreamWriter::TextStreamWriter(ostream& m_stream): stream(&m_stream)
{
}

TextStreamWriter::~TextStreamWriter()
{
}

TextStreamWriter& TextStreamWriter::operator<<(const char & char_data)
{
  *stream<<type_and_delim(char_data)<<char_data<<endl;
  return *this;
}

TextStreamWriter& TextStreamWriter::operator<<(const int & int_data)
{
  *stream<<type_and_delim(int_data)<<int_data<<endl;
  return *this;
}

TextStreamWriter& TextStreamWriter::operator<<(const double & double_data)
{
  *stream<<type_and_delim(double_data)<<double_data<<endl;
  return *this;
}

TextStreamWriter& TextStreamWriter::operator<<(const string & string_data)
{
  *stream<<type_and_delim(string_data)<<string_data.size()<<" "<<string_data<<endl;
  return *this;
}
