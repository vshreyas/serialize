// text_streamreader.hpp
// Read from stream and store into object
#ifndef TEXT_STREAMREADER_HPP
#define TEXT_STREAMREADER_HPP

#include <iostream>
#include <string>
#include <typeinfo>

#include "streamreader.hpp"

class TextStreamReader
{
public:
  TextStreamReader();
  TextStreamReader(std::istream &);
  ~TextStreamReader();

  TextStreamReader& operator>>(char &);
  TextStreamReader& operator>>(int &);
  TextStreamReader& operator>>(double &);
  TextStreamReader& operator>>(std::string &);

private:
  template <class T>
  bool read_and_check_types(const T &);
  std::istream* stream;
};

template <class T>
bool TextStreamReader::read_and_check_types(const T & data)
{
  std::string serialized_type_name;
  *stream>>serialized_type_name;
  if (serialized_type_name != typeid(data).name())
    {
      std::cout<<"Types: Expected: "<<typeid(data).name()<< ", and Present: "<<serialized_type_name<<" do not match!\n";
      return false;
    }
  return true;
}

#endif // TEXT_STREAMREADER_HPP
