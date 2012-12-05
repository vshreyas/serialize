#ifndef BINARY_STREAMWRITER_HPP
#define BINARY_STREAMWRITER_HPP

#include "streamwriter.hpp"
#include <cstddef>
#include <type_traits>
#include <vector>
#include <utility>
#include <map>

class BinaryStreamWriter: public StreamWriter
{
public:
  //TextStreamWriter();
  BinaryStreamWriter(std::ostream&m_stream): StreamWriter(m_stream)  {

  }
  ~BinaryStreamWriter();

  template<typename T>
  friend void serialize(BinaryStreamWriter& w, const std::vector<T>& vec_data);

  template<typename T1, typename T2>
  friend void serialize(BinaryStreamWriter& w, const std::pair<T1, T2>& pair_data);

  template<typename T1, typename T2>
  friend void serialize(BinaryStreamWriter& w, const std::map<T1, T2>& map_data);

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

  void write_data(const std::string& string_data)
  {
    size_t slen = string_data.size();
    stream->write(reinterpret_cast<const char*>(&slen), sizeof(slen));
    stream->write(string_data.c_str(), string_data.size());
  }

  template <class T>
  typename std::enable_if<std::is_polymorphic<T>::value>::type
  write_type(T* & T_data)
  {
//    std::string type_key(InfoList<TextStreamWriter>::get_matching_type(T_data)->key());
//    *this<<type_key;
  }
};

BinaryStreamWriter::~BinaryStreamWriter()
{
}

/** @brief serializes any STL vector
  * by writing the size first
  * and then the vector elements
*/
template <typename T>
 void serialize(BinaryStreamWriter& w, const std::vector<T> & vec_data) {
      size_t vec_size = vec_data.size();
      w.stream->write(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
      for(auto it = vec_data.begin();it !=vec_data.end();++it) {
        w<<*it;
      }
  }

/**
 *
 *
*/
template<typename T1, typename T2>
void serialize(BinaryStreamWriter& w, const std::pair<T1, T2>& pair_data) {
      w<<pair_data.first<<pair_data.second;
  }

/**
 *
 *
*/
template<typename T1, typename T2>
void serialize(BinaryStreamWriter& w, const std::map<T1, T2>& map_data) {
    size_t map_size = map_data.size();
    w<<map_size;
    for(auto it = map_data.begin();it !=map_data.end();++it) {
        w<<(*it).first<<(*it).second;
    }
  }

#endif
