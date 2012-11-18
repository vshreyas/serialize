// streamreader.hpp
#ifndef STREAMREADER_HPP
#define STREAMREADER_HPP

#include "common.hpp"

#include <iostream>

class StreamReader
{
public:
  StreamReader(std::istream&);

  template <class T>
  void load(T&)
  {
    NOT_IMPLEMENTED("`load()' of StreamReader called.\n");
  }
  
  virtual ~StreamReader() = 0;		// don't close stream here

protected:
  std::istream* stream;
};

// >> operator for all classes derived from StreamReader 

template <typename Reader, typename T>
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value, Reader&>::type
operator>>(Reader & reader, T & T_data)
{
  // For classes, `load' should rely on `deserialize' to read the members
  reader.load(T_data);
  deserialize(reader, T_data);
  return reader;
}

// Default deserialize function which does nothing.  For classes, the
// user defines this which tells the library what members to deserialize.
template <typename Reader, typename T>
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value>::type
deserialize(Reader & reader, T & T_data)
{
}

#endif
