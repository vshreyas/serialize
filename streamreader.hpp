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
  StreamReader(std::istream& is);

  /** 
   * Virtual destructor - derived classes must not close the istream
   * object inside.
   */
  virtual ~StreamReader() = 0;

  /** 
   * Dummy templated load function - if this ends up being called,
   * it's an error.
   *
   * @param T_data Object to be read into
   */
  template <class T>
  void load(T& T_data)
  {
    NOT_IMPLEMENTED("`load()' of StreamReader called.\n");
  }

protected:
  /**
   * istream object from where data has to be read.
   */
  std::istream* stream;
};

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
typename std::enable_if <std::is_base_of<StreamReader, Reader>::value
&& std::is_polymorphic<T>::value, Reader&>::type
operator>>(Reader & reader, T* & T_data)
{
  // T is of pointer type. Assume polymorphic
  reader.load(T_data);
  //reader.save(T_data);
  //serialize(reader, T_data);
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
