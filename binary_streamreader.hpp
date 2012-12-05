#ifndef BINARY_STREAMREADER_HPP
#define BINARY_STREAMREADER_HPP
#include "streamreader.hpp"
#include <cstddef>
#include <type_traits>
#include <string>
#include <vector>
#include <map>

class BinaryStreamReader: public StreamReader
{
public:
  BinaryStreamReader(std::istream& m_stream): StreamReader(m_stream) {
  }

  ~BinaryStreamReader() {
  }

  template <typename T>
  friend void deserialize(BinaryStreamReader& r, std::vector<T>& vec_data);

  template <typename T1, typename T2>
  friend void deserialize(BinaryStreamReader& r, std::pair<T1, T2>& pair_data);

  template <typename T>
  typename std::enable_if<!std::is_class<T>::value>::type
  load(T & T_data)
  {
    read_and_check_types(T_data);
    read_data(T_data);
  }

  template <typename T>
  typename std::enable_if<std::is_class<T>::value>::type
  load(T & T_data)
  {
    read_and_check_types(T_data);
    // Reading of data left to `deserialize'
  }

  void load(std::string & string_data)
  {
    read_and_check_types(string_data);
    read_data(string_data);
  }

private:
  template <class T>
  bool read_and_check_types(const T &)
  {
      return true;
    }

  template <class T>
  typename std::enable_if<std::is_fundamental<T>::value,void>::type
  read_data(T & T_data)
  {
    stream->read(reinterpret_cast<char*>(&T_data), sizeof(T_data));
  }

  template <class T>
  typename std::enable_if<std::is_array<T>::value,void>::type
  read_data(T & T_array_data)
  {
    size_t element_size = sizeof(T_array_data[0]);
    size_t array_size = sizeof(T_array_data)/element_size;
    size_t stored_array_size;

    stream->read(reinterpret_cast<char*>(&stored_array_size), sizeof(stored_array_size));
    if (stored_array_size != array_size)
      {
	std::cout<<"Stored array size = "<<stored_array_size
		 <<", expected array size = "<<array_size;
	return;
      }

    for (size_t i = 0; i < array_size; ++i)
	*this>>T_array_data[i];
  }

  void read_data(std::string & string_data)
  {
    unsigned int size;
    stream->read(reinterpret_cast<char*>(&size),sizeof(size));
    std::vector<char> temp(size);
    stream->read(&temp[0], size);
    string_data.assign(&temp[0], size);
  }
};

template <typename T>
void deserialize(BinaryStreamReader& r, std::vector<T>& vec_data) {
      size_t vec_size_read;
      r.stream->read(reinterpret_cast<char*>(&vec_size_read), sizeof(vec_size_read));
      // check vec_data size here and throw an exception if it is not 0(ie not empty)
      int i;
      T t;
      for(i = 0; i<vec_size_read;i++){
        r.stream->read(reinterpret_cast<char*>(&t), sizeof(t));
        vec_data.push_back(t);
      }
}

template <typename T1, typename T2>
void deserialize(BinaryStreamReader& r, std::pair<T1, T2>& pair_data) {
    r>>pair_data.first>>pair_data.second;
}

template<typename T1, typename T2>
void deserialize(BinaryStreamReader& r, const std::map<T1, T2>& map_data) {
    size_t map_size_read;
    r>>map_size_read;
    //check size
    std::pair<T1, T2> p;
    bool ret = true;
    int i;
    for(i = 0; i<map_size_read;i++){
        r>>p;
        ret = map_data.insert(p);
        if(ret == false) {
            //throw exception here
        }
    }
}

#endif
