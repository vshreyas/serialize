#ifndef STREAMWRITER_HPP
#define STREAMWRITER_HPP

#include <iostream>

// streamwriter.hpp
class StreamWriter
{
public:
  StreamWriter();
  StreamWriter(std::ostream&);
  ~StreamWriter();		// don't close stream here
  
  void set_stream(std::ostream&);
  std::ostream& get_stream();
  
protected:
  std::ostream& stream;
};

#endif
