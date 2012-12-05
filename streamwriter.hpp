/**
 * @file   streamwriter.hpp
 * @author Shashwat Lal Das <shashwat@mashed-potato>
 * @date   Tue Dec  4 18:13:57 2012
 * 
 * @brief Definition of the base StreamWriter abstract class and the
 * << operator for objects derived from StreamWriter. Derived classes
 * must implement the `save' member function for every type that is
 * going to be serialized.
 */

#ifndef STREAMWRITER_HPP
#define STREAMWRITER_HPP

#include "common.hpp"
#include "types.hpp"

#include <iostream>

/**
 * Base StreamWriter abstract class. All user-implemented stream
 * writers must inherit from this.
 *
 * This provides an ostream* member and a constructor taking an open
 * ostream object.
 *
 * All derived classes must implement a (templated) `save()' function
 * which writes data from a given variable into the stream. Whether to
 * write the type identifer or not is to be decided by the derived
 * class. 
 *
 * For classes, the user must define a `serialize()' function which
 * serializes individual members of the class.  For polymorphics, the
 * user must first register the derived classes using
 * REGISTER_TYPE(<writer object>,<derived class name>).
 */
class StreamWriter
{
public:
  /** 
   * Construct from a given open ostream object.
   *
   * @param m_stream ostream object, must be open.
   */  
  StreamWriter(ostream& m_stream): stream(&m_stream)
  {
  }

  /** 
   * Pure virtual destructor to make this an abstract class.
   */
  virtual ~StreamWriter() = 0;		// don't close stream here
  
protected:
  std::ostream* stream;		/**< stream to write to */
};

StreamWriter::~StreamWriter()
{
}

/** 
 * Applies to all objects derived from StreamWriter - when << is
 * called for an object of type T to be serialized, the save function
 * (for type T) of this derived class is called.
 *
 * After performing `save', the serialize function is called which is
 * by default, trivial. This may be overloaded by the user to do
 * something else in addition if necessary.
 *
 * @param writer Derived StreamWriter instance
 * @param T_data data to serialize, must be convertible to a const reference
 *
 * @return Derived StreamWriter instance
 */
template <typename Writer, typename T>
typename std::enable_if <std::is_base_of<StreamWriter, Writer>::value, Writer&>::type
operator<<(Writer & writer, const T & T_data)
{
  // For classes, `save' should rely on `serialize' to write the members
  writer.save(T_data);
  serialize(writer, T_data);
  return writer;
}

/** 
 * If the object to be serialized is of (Polymorphic *) type, get the
 * actual (derived) type of the object and serialize it.
 *
 * @param writer Derived StreamWriter instance
 * @param T_data Pointer to polymorphic type whose contents are to be serialized
 *
 * @return Derived StreamWriter instance
 */
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

/** 
 * Default implementation of `serialize'.
 *
 * For classes, it is essential that the user overload it for the
 * particular user-defined class, and write the serialization code for
 * each member which should be serialized, in addition to potentially
 * casting it to its base class and serializing that.
 *
 * @param writer Derived StreamWriter instance
 * @param T_data Data to be serialized
 *
 * @return Derived StreamWriter instance
 */
template <typename Writer, typename T>
typename std::enable_if<std::is_base_of<StreamWriter, Writer>::value>::type
serialize(Writer & writer, const T & T_data)
{
}
#endif
