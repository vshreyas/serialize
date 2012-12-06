/**
 * @file   text_streamreader.hpp
 * @author Shashwat Lal Das <shashwat@mashed-potato>
 * @date   Tue Dec  4 17:39:19 2012
 * 
 * @brief  Deserializes something generated from TextStreamWriter.
 * 
 * Type-checking is not done for types other than polymorphics, and
 * the trivial read_and_check_types() is retained from an earlier
 * version, and also to demonstrate the possibility of adding
 * type-checking in future versions.
 */

#ifndef TEXT_STREAMREADER_HPP
#define TEXT_STREAMREADER_HPP

#include <iostream>
#include <string>
#include <typeinfo>

#include "streamreader.hpp"
#include "stl_serialize.hpp"

/**
 * Reads from a stream with data serialized using TextStreamWriter.
 */
class TextStreamReader: public StreamReader
{
public:

  /** 
   * Construct using an open istream
   *
   * @param m_stream open istream
   */  
  TextStreamReader(istream & m_stream): StreamReader(m_stream)
  {
  }

  /** 
   * Trivial destructor
   */
  ~TextStreamReader()
  {
  }

  /** 
   * Read objects of non-class type
   *
   * @param T_data non-class type object
   */
  template <typename T>
  typename std::enable_if<!std::is_class<T>::value>::type
  load(T & T_data)
  {
    read_and_check_types(T_data);
    read_data(T_data);
  }

  /** 
   * Deserializing objects of class type is done with preliminary
   * checks (if desired) here, and then calling the `deserialize'
   * user-defined function which actually deserializes the members.
   *
   * @param T_data object of class type to read into
   */
  template <typename T>
  typename std::enable_if<std::is_class<T>::value>::type
  load(T & T_data)
  {
    read_and_check_types(T_data);
    // Reading of data left to `deserialize'
  }

  /** 
   * Read into a string
   *
   * @param string_data string to read into
   */
  void load(std::string & string_data)
  {
    read_and_check_types(string_data);
    read_data(string_data);
  }

  /** 
   * Read into a char*, assuming it is a C-style string.
   *
   * @param cstring_data given character pointer
   */
  void load(char* & cstring_data)
  {
    read_and_check_types(cstring_data);
    read_data(cstring_data);
  }
  
private:

  /** 
   * Read type and check if it matches with given type.
   *
   * DEPRECATED since types are no longer written.
   * 
   * @return true if types are compatible.
   */
  template <class T>
  bool read_and_check_types(const T &);

  /** 
   * For fundamental types, read directly using the istream
   *
   * @param T_data fundamental type data
   */
  template <class T>
  typename std::enable_if<std::is_fundamental<T>::value,void>::type
  read_data(T & T_data)
  {
    *stream>>T_data;
    checkandthrowBasicException(stream);
  }

  /** 
   * For arrays, format: <length><elements>
   *
   * @param T_array_data array to read into
   */
  template <class T>
  typename std::enable_if<std::is_array<T>::value,void>::type
  read_data(T & T_array_data)
  {
    size_t array_size = std::extent<T>::value;
    size_t stored_array_size;

    *this>>stored_array_size;
    if (stored_array_size != array_size)
      {
	// throw exception
	throw SizeMismatchException(stored_array_size, array_size);
      }

    for (size_t i = 0; i < array_size; ++i)
	*this>>T_array_data[i];
  }

  /** 
   * Read a string. It has to be specialized by all derived classes.
   *
   * String is assumed to be stored as:
   * <length><one space><string data>
   * 
   * @param string_data string to read into
   */
  void read_data(std::string & string_data)
  {
    size_t len;
    *stream>>len;
    stream->get();		// space
    char* s = new char[len + 1];

    stream->read(s, len);
    s[len] = '\0';
    string_data = std::string(s, len);
    delete[] s;
    checkandthrowBasicException(stream);
  }

  /** 
   * Read a C-style string, assuming the given char* represents one.
   * Changes the address pointed to by the given pointer. Caller is
   * responsible for deleting it.
   *
   * @param cstring_data given char*, assumed to be a C-style string
   */
  void read_data(char* & cstring_data)
  {
    size_t len;
    *stream>>len;
    stream->get();
    char* s = new char[len + 1];

    stream->read(s,len);
    s[len] = '\0';
    cstring_data = s;

    checkandthrowBasicException(stream);
  }
};

// Trivialized since we decided to drop serializing the type
template <class T>
bool TextStreamReader::read_and_check_types(const T & data)
{
  return true;
}

#endif // TEXT_STREAMREADER_HPP
