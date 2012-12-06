/**
 * @file   binary_streamwriter.hpp
 * @author Shreyas Vinayakumar
 * @date   Tue Dec  4 17:39:19 2012
 *
 * @brief  Classes/methods to serialize binary data in a compact format
 * Expected to be used for native data i.e. currently non-portable. Will be extended to portable also in next revision
 */

#ifndef BINARY_STREAMWRITER_HPP
#define BINARY_STREAMWRITER_HPP
#include "streamwriter.hpp"
#include "stl_serialize.hpp"
#include <cstddef>
#include <type_traits>


class BinaryStreamWriter: public StreamWriter
{
public:
  /**
   * Constructed from an output stream which could be any sequential access
   */
  BinaryStreamWriter(std::ostream&m_stream): StreamWriter(m_stream)  {

  }
  ~BinaryStreamWriter();

  template <typename T>
  typename std::enable_if<std::is_fundamental<T>::value>::type
  save(const T & T_data)
  {
    write_data(T_data);
  }

  void save(const std::string & string_data)
  {
    write_data(string_data);
  }

  template <typename T>
  typename std::enable_if<std::is_class<T>::value>::type
  save(const T & T_data)
  {
    //serialize(*this, T_data);
  }

  template <typename T>
  typename std::enable_if<std::is_polymorphic<T>::value>::type
  save(T* T_data)
  {
    write_type(T_data);
    //write_type(T_data);
    //serialize(*this, T_data);
  }

  template <typename T>
  typename std::enable_if<std::is_array<T>::value>::type
  save(const T & T_data)
  {
    // number of elements
    size_t length = std::extent<T>::value;
    *this<<length;
    for (size_t i = 0; i < length; ++i)
        *this<<T_data[i];
  }

private:
  template <class T>
  void write_data(const T & T_data)
  {
    stream->write(reinterpret_cast<const char*>(&T_data), sizeof(T_data));
  }

  /** 
   * Write a given char*, assuming it to be a C-style string.
   * Format: <length><one space><cstring data>
   *
   * @param cstring_data given char* whose data is to be written
   */
  void write_data(const char* cstring_data)
  {
    size_t slen = 0;
    while (cstring_data[slen] != '\0')
      slen++;
    
    stream->write(reinterpret_cast<const char*>(&slen), sizeof(slen));
    stream->write(cstring_data, slen);
  }

  void write_data(const std::string& string_data)
  {
    size_t slen = string_data.size();
    stream->write(reinterpret_cast<const char*>(&slen), sizeof(slen));
    stream->write(string_data.c_str(), string_data.size());
  }

  /**
   * Serialize pointers to polymorphic objects
   * Must write the type key also as there is no other way to know what object it represents at runtime
   * @param T* pointer to polymorphic base class
   */
  template <class T>
  typename std::enable_if<std::is_polymorphic<T>::value>::type
  write_type(T* & T_data)
  {
    std::string type_key(InfoList<BinaryStreamWriter>::get_matching_type(T_data)->key());
    *this<<type_key;
  }
};

BinaryStreamWriter::~BinaryStreamWriter()
{
}

#endif
