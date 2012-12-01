#ifndef BINARY_STREAMWRITER_HPP
#define BINARY_STREAMWRITER_HPP

#include "streamwriter.hpp"
#include <cstddef>
#include <type_traits>
#include <vector>

class BinaryStreamWriter: public StreamWriter
{
public:
  //TextStreamWriter();
  BinaryStreamWriter(std::ostream&);
  ~BinaryStreamWriter();

  template<typename T>
  friend void serialize(BinaryStreamWriter& w, const std::vector<T>& vec_data);

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
  typename std::enable_if<std::is_array<T>::value>::type
  save(const T & T_data)
  {
    // number of elements
    size_t element_size = sizeof(T_data[0]);
    size_t array_size = sizeof(T_data)/element_size;
    stream->write(reinterpret_cast<char*>(&array_size), sizeof(array_size));
    for (size_t i = 0; i < array_size; ++i)
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

};

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

#endif
