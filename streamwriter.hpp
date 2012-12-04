#ifndef STREAMWRITER_HPP
#define STREAMWRITER_HPP

#include "common.hpp"
#include "types.hpp"

#include <iostream>

// streamwriter.hpp
class StreamWriter
{
public:
  StreamWriter(std::ostream&);

  template <class T>
  void save(const T&)
  {
    NOT_IMPLEMENTED("`save()' of StreamWriter called.\n");
  }

  virtual ~StreamWriter() = 0;		// don't close stream here
  
protected:
  std::ostream* stream;
};

template <typename Writer, typename T>
typename std::enable_if <std::is_base_of<StreamWriter, Writer>::value, Writer&>::type
operator<<(Writer & writer, const T & T_data)
{
  // For classes, `save' should rely on `serialize' to write the members
  writer.save(T_data);
  serialize(writer, T_data);
  return writer;
}

template <typename Writer, typename T>
typename std::enable_if <std::is_base_of<StreamWriter, Writer>::value
&& std::is_polymorphic<T>::value, Writer&>::type
operator<<(Writer & writer, T* T_data)
{
  // T is of pointer type. Assume polymorphic
  auto match_elem = InfoList<Writer>::get_matching_type(T_data);
  match_elem->call_serialize(writer, T_data);

  return writer;
}

template <typename Writer, typename T>
typename std::enable_if<std::is_base_of<StreamWriter, Writer>::value>::type
serialize(Writer & writer, const T & T_data)
{
}
#endif
