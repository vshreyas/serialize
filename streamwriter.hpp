#ifndef STREAMWRITER_HPP
#define STREAMWRITER_HPP

#include <iostream>

// streamwriter.hpp
class StreamWriter
{
public:
  StreamWriter(std::ostream&);
  virtual ~StreamWriter() = 0;		// don't close stream here
  
protected:
  std::ostream* stream;
};

template <typename Writer, typename T>
typename std::enable_if<std::is_base_of<StreamWriter, Writer>::value, Writer&>::type
operator<<(Writer & writer, const T & T_data)
{
  // For classes, `save' should rely on `serialize' to write the members
  writer.save(T_data);
  serialize(writer, T_data);
  return writer;
}

template <typename Writer, typename T>
typename std::enable_if<std::is_base_of<StreamWriter, Writer>::value>::type
serialize(Writer & writer, const T & T_data)
{
}
	  
#endif
