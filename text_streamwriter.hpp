/**
 * @file   text_streamwriter.hpp
 * @author Shashwat Lal Das <shashwat@mashed-potato>
 * @date   Tue Dec  4 17:33:52 2012
 * 
 * @brief Serialize data in the text (human-readable) format.  Support
 * is provided, by default, for fundamental types, arrays and
 * std::strings. Additional user-defined functions are required to
 * handle classes and polymorphic types.
 *
 * Types are currently not written for any object apart from
 * polymorphics, where they are absolutely necessary.
 */

#ifndef TEXT_STREAMWRITER_HPP
#define TEXT_STREAMWRITER_HPP

#include <cstring>
#include <iostream>
#include <string>
#include <typeinfo>

#include "streamwriter.hpp"
#include "types.hpp"

/**
 * Serializes data in a text format. Format chosen is: one item per
 * line.
 */
class TextStreamWriter: public StreamWriter
{
public:
  /** 
   * Needs an opened ostream object.
   *
   * @param m_stream Open ostream object
   */
  TextStreamWriter(ostream& m_stream): StreamWriter(m_stream)
  { }

  /** 
   * Trivial destructor
   */
  ~TextStreamWriter()
  { }

  /** 
   * Write fundamental types.
   *
   * @param T_data any fundamental type
   */
  template <typename T>
  typename std::enable_if<std::is_fundamental<T>::value>::type
  save(const T & T_data)
  {
    write_type(T_data);
    write_data(T_data);
  }

  /** 
   * Write string data
   *
   * @param string_data given string
   */
  void save(const std::string & string_data)
  {
    write_type(string_data);
    write_data(string_data);
  }

  /** 
   * Write char* data, interpreting it as a C-style string
   *
   * @param cstring_data given char*
   */
  void save(const char* cstring_data)
  {
    write_type(cstring_data);
    write_data(cstring_data);
  }
  
  /** 
   * For a class, there is no `write_data'. Writing of the members
   * depends on the user-defined `serialize' function.
   *
   * @param T_data An object of class type
   */
  template <typename T>
  typename std::enable_if<std::is_class<T>::value>::type
  save(const T & T_data)
  {
    write_type(T_data);
  }

  /** 
   * For polymorphic types, writing of the type is mandatory.
   *
   * @param T_data Pointer to a polymorphic type
   */
  template <typename T>
  typename std::enable_if<std::is_polymorphic<T>::value>::type
  save(T* T_data)
  {
    write_type(T_data);
  }

  /** 
   * For arrays, format is: <length><elements>
   *
   * @param T_data an array
   */  
  template <typename T>
  typename std::enable_if<std::is_array<T>::value>::type
  save(const T & T_data)
  {
    // Get the first dimension
    size_t length = std::extent<T>::value;

    *this<<length;
    for (size_t i = 0; i < length; ++i)
	*this<<T_data[i];
  }
  
private:
  /** 
   * Get the name of the type obtained by calling typeid.name.
   * DEPRECATED - since types are no longer written for every object,
   * and typeid is not portable.
   *
   * @return type name followed by a space
   */
  template <class T>
  std::string type_and_delim(const T &)
  {
    return std::string(typeid(T).name()) + " ";
  }

  /** 
   * Write the type of a given polymorphic object.  The type is found
   * from the global map of known types, where the key represents the
   * type name.
   *
   * @param T_data a given object of the required type
   */
  template <class T>
  typename std::enable_if<std::is_polymorphic<T>::value>::type
  write_type(T* & T_data)
  {
    // Get the matching Info object from the list (must have been
    // registered by the user first) and write its key.
    // Throws an exception if the correct Info class was not found.
    std::string type_key(InfoList<TextStreamWriter>::get_matching_type(T_data)->key());
    *this<<type_key;
  }

  // Make it do nothing - will not write type for non-polymorphics
  template <class T>
  void write_type(const T & T_data)
  {
    return;
  }

  /** 
   * For any type in general,
   * <data><newline>
   *
   * @param T_data object to serialize
   */  
  template <class T>
  void write_data(const T & T_data)
  {
    *stream<<T_data<<std::endl;
  }

  /** 
   * Write a given char*, assuming it to be a C-style string.
   * Format: <length><one space><cstring data>
   *
   * @param cstring_data given char* whose data is to be written
   */
  void write_data(const char* cstring_data)
  {
    *stream<<strlen(cstring_data)<<" "<<cstring_data<<std::endl;
  }
  
  /** 
   * For strings, format is: <length><one space><string data>
   *
   * @param string_data string to be serialized
   */
  void write_data(const std::string& string_data)
  {
    *stream<<string_data.size()<<" "<<string_data<<std::endl;
  }
};

#endif // TEXT_STREAMWRITER_HPP
