/**
 * @file   streamreader.hpp
 * @author Shashwat Lal Das <shashwat@mashed-potato>
 * @date   Tue Nov 20 23:09:49 2012
 * 
 * @brief Defines the interface for stream reading objects through the
 * StreamReader abstract class. Also defines the default >> operator
 * for these objects and the default `deserialize()' function.
 * 
 */

#ifndef STREAMREADER_HPP
#define STREAMREADER_HPP

#include "common.hpp"
#include "types.hpp"

#include <iostream>

/** 
 * Base StreamReader abstract class. All user-implemented stream
 * readers must inherit from this.
 *
 * This provides an istream* member and a constructor taking an
 * istream object.
 *
 * All derived classes must implement a templated `load()' function
 * which reads data into the given variable, and it is the correct
 * place to do type-checking if necessary.
 *
 * For classes, where user-intervention is required, `load()' can
 * depend on the user-defined `deserialize()' function for that class
 * which will perform the deserialization for individual members.
 *
 * For polymorphics, the user must first register the derived classes using
 * REGISTER_TYPE(<reader object>,<derived class name>).
 */
class StreamReader
{
public:
  /** 
   * Constructor with a given istream object - the member istream
   * object should refer to the user's object after the call.
   *
   * @param is istream object
   */
  StreamReader(istream& m_stream): stream(&m_stream)
  {
  }

  /** 
   * Virtual destructor - derived classes must not close the istream
   * object inside.
   * Forces class to be abstract.
   */
  virtual ~StreamReader() = 0;

protected:
  /**
   * istream object from where data has to be read.
   */
  istream* stream;
};

/** 
 * Trivial definition of virtual destructor.
 */
StreamReader::~StreamReader()
{
}

/** 
 * >> operator enabled for derived classes of StreamReader.
 *
 * @param reader Object of a derived class of StreamReader
 * @param T_data Object to be read into
 *
 * @return The given StreamReader derived object
 */
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
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value
&& std::is_polymorphic<T>::value, Reader&>::type
  operator>>(Reader & reader, T* & T_data)
{
  // T is of pointer type. Assume polymorphic
  std::string type_name_stored;
  reader>>type_name_stored;

  // Matching Info object corresponding to the dynamic type
  auto match_elem = InfoList<Reader>::get_matching_type_by_key(type_name_stored);
  // call_deserialize returns void*, so cast to T* and return
  T_data = static_cast<T*>(match_elem->call_deserialize(reader));

  return reader;
}

/** 
 * Default implementation which should do nothing. The user must
 * define specialized functions for their classes, which will
 * deserialize members individually.
 *
 * @param reader Object of a derived class of StreamReader
 * @param T_data Object to be read into
 *
 * @return void
 */
template <typename Reader, typename T>
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value>::type
deserialize(Reader & reader, T & T_data)
{
}
#endif
