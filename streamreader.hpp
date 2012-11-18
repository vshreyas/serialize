// streamreader.hpp
#ifndef STREAMREADER_HPP
#define STREAMREADER_HPP

#include <iostream>

class StreamReader
{
public:
  StreamReader(std::istream&);
  virtual ~StreamReader() = 0;		// don't close stream here

protected:
  std::istream* stream;
};

template <typename Reader, typename T>
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value, Reader&>::type
operator>>(Reader & reader, T & T_data)
{
  // For classes, `load' should rely on `deserialize' to read the members
  reader.load(T_data);
  deserialize(reader, T_data);
  return reader;
}

template <typename Reader, typename T>
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value>::type
deserialize(Reader & reader, T & T_data)
{
}

#endif
