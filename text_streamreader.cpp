#include "text_streamreader.hpp"

using namespace std;

TextStreamReader::TextStreamReader(): stream(0)
{
}

TextStreamReader::TextStreamReader(istream & m_stream): stream(&m_stream)
{
}

TextStreamReader::~TextStreamReader()
{
}

TextStreamReader& TextStreamReader::operator>>(char & char_data)
{
  if (read_and_check_types(char_data))
    *stream>>char_data;
  return *this;
}

TextStreamReader& TextStreamReader::operator>>(int & int_data)
{
  if (read_and_check_types(int_data))
    *stream>>int_data;
  return *this;
}

TextStreamReader& TextStreamReader::operator>>(double & double_data)
{
  if (read_and_check_types(double_data))
    *stream>>double_data;

  return *this;
}

TextStreamReader& TextStreamReader::operator>>(string & string_data)
{
  long len;
  if (read_and_check_types(string_data))
    *stream>>len;
  stream->get();
  char* s = new char[len + 1];
  stream->get(s, len + 1);
  string_data = string(s, len);
  delete[] s;
  return *this;
}
