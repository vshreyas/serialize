// text_streamreader.hpp
// Read from stream and store into object
#ifndef TEXT_STREAMREADER_HPP
#define TEXT_STREAMREADER_HPP

#include <iostream>
#include <string>
#include <typeinfo>

#include "types.hpp"
#include "streamreader.hpp"

class TextStreamReader: public StreamReader
{
public:
  TextStreamReader(std::istream &);
  ~TextStreamReader();

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
  /*
  template <typename T>
  typename std::enable_if
  <!std::is_class<T>::value && std::is_polymorphic<T>::value>::type
  load(T* & T_data)
  {
    std::string type_name;
    // proper code for reading type name
    *stream>>type_name;
    info_base* actual_type_info = get_type_from_key(type_name);
    void* actual_obj = actual_type_info->construct();
    read_data(actual_obj);
    T_data = actual_obj;
  }
  */
  void load(std::string & string_data)
  {
    read_and_check_types(string_data);
    read_data(string_data);
  }
  
private:
  template <class T>
  bool read_and_check_types(const T &);

  template <class T>
  typename std::enable_if<std::is_fundamental<T>::value,void>::type
  read_data(T & T_data)
  {
    *stream>>T_data;
  }

  template <class T>
  typename std::enable_if<std::is_array<T>::value,void>::type
  read_data(T & T_array_data)
  {
    size_t element_size = sizeof(T_array_data[0]);
    size_t array_size = sizeof(T_array_data)/element_size;
    size_t stored_array_size;

    *this>>stored_array_size;
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
    size_t len;
    *stream>>len;
    stream->get();
    char* s = new char[len + 1];
    size_t chars_to_read = len;

    stream->read(s, len);
    s[len] = '\0';
    string_data = std::string(s, len);
    delete[] s;
  }
};

template <class T>
bool TextStreamReader::read_and_check_types(const T & data)
{
  std::string serialized_type_name;
  *stream>>serialized_type_name;
  //std::cout<<"Read type: "<<serialized_type_name<<", expected:"<<typeid(data).name()<<std::endl;
  if (serialized_type_name != typeid(T).name())
    {
      std::cout<<"Types: Expected: "<<typeid(T).name()<< ", and Present: "<<serialized_type_name<<" do not match!\n";
      return false;
    }
  return true;
}

#endif // TEXT_STREAMREADER_HPP
