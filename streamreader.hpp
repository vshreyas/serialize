// streamreader.hpp
#ifndef STREAMREADER_HPP
#define STREAMREADER_HPP

#include <iostream>

class StreamReader
{
public:
  StreamReader();
  StreamReader(const std::istream&);
  ~StreamReader();		// don't close stream here

  void set_stream(const std::istream&);
  std::istream& get_stream();
  
protected:
  std::istream& stream;
};

#endif
