/**
 * @file   binary_streamreader.hpp
 * @author Shreyas Vinayakumar
 * @date   Tue Dec  4 17:39:19 2012
 *
 * @brief  Classes/methods to deserialize the binary data written by BinaryStreamWriter
 * Expected to be used for native data i.e. currently non-portable. Will be extended to portable also in next revision
 */

#ifndef BINARY_STREAMREADER_HPP
#define BINARY_STREAMREADER_HPP
#include "streamreader.hpp"
#include "stl_serialize.hpp"
#include <cstddef>
#include <type_traits>
#include <string>

/**
 * Inherits from StreamReader; constructed from an input stream
 * Reads binary data from the stream into objects according to the predefined format
 */
class BinaryStreamReader: public StreamReader
{
public:
  BinaryStreamReader(std::istream& m_stream): StreamReader(m_stream) {
  }

  ~BinaryStreamReader() {
  }

  /**
   * Reads types that are not user-defined
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
   * Dummy load function for classes.
   * The functionality for classes is actually in serialize
   */

  template <typename T>
  typename std::enable_if<std::is_class<T>::value>::type
  load(T & T_data)
  {
    read_and_check_types(T_data);
    // Reading of data left to `deserialize'
  }

  /**
   * Reads an STL string.
   * Treated specially here because this type is used to store identifiers for some classes.
   */
  void load(std::string & string_data)
  {
    read_and_check_types(string_data);
    read_data(string_data);
  }

private:

  /**
   * Not checking types now; just a stub
   */
  template <class T>
  bool read_and_check_types(const T &)
  {
      return true;
    }

  /**
   * This template covers all fundamental types(signed/unsignedint, float, char.
   * They are basically serialized by just dumping the binary representations
   * This makes code non-portable as diff conventions(number of bytes, endianness) may hold
   * One workaround could be to store each type in a standard format
   * and write a header file that performs necessary conversions for each platform
   *
   */
  template <class T>
  typename std::enable_if<std::is_fundamental<T>::value,void>::type
  read_data(T & T_data)
  {
    stream->read(reinterpret_cast<char*>(&T_data), sizeof(T_data));
    checkandthrowBasicException(stream);    
  }

  /**
   * Handles reading C-style arrays
   * The array must be static viz. its size must be known at compile time
   * Non-static extensions such as those provided by GCC are not supported
   * @param templated arg T_array_data, array that can hold objects of type T
   * @throw SizeInsufficient exception if the provided array is not large enough
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
   * Read data from stream into a std::string
   * @param a string
   */
  void read_data(std::string & string_data)
  {
    size_t len;
    stream->read(reinterpret_cast<char*>(&len),sizeof(len));

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
    stream->read(reinterpret_cast<char*>(&len),sizeof(len));

    char* s = new char[len + 1];
    stream->read(s,len);
    s[len] = '\0';
    cstring_data = s;

    checkandthrowBasicException(stream);
  }

};


#endif
