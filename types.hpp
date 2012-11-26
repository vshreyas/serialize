#ifndef _TYPES_HPP
#define _TYPES_HPP

#include "common.hpp"
#include <iostream>
#include <typeinfo>
#include <vector>


class StreamReader;
class StreamWriter;


class info_base
{
public:
  virtual bool is_same_type(void* other, const std::type_info& id_info)
  {
    NOT_IMPLEMENTED("is_same_type!\n");
  }
  virtual void* construct() = 0;

  virtual std::string key() = 0;

  virtual void call_serialize(void* other)
  {
    NOT_IMPLEMENTED("Called serialize of base class!\n");
  }

  virtual void* call_deserialize()
  {
    NOT_IMPLEMENTED("Called deserialize of base class!\n");
    return NULL;
  }
};

template <class Writer, class T, class Enable = void>
class info: public info_base
{
public:
  virtual void* construct() { NOT_IMPLEMENTED("generic info construct!\n"); }
  virtual std::string key() { NOT_IMPLEMENTED("generic info key!\n"); }
};

template <class Writer, class T>
class info<Writer, T,
	   typename std::enable_if<std::is_base_of<StreamWriter, Writer>::value>::type>
  : public info_base
{
public:
  info(Writer &w): writer(&w) { }
  
  Writer* writer;
  
  virtual bool is_same_type(void* other, const std::type_info& id_info)
  {
    return (typeid(T) == id_info);
  }

  virtual std::string key()
  {
    //return ((std::string) typeid(T).name());
    return "";
  }
  
  template <class Other>
  T cast(Other other) { return dynamic_cast<T>(other); }
  
  virtual void* construct()
  {
    return new T;
  }

  virtual void call_serialize(void* other)
  {
    // call save on the pointer so we can specialize
    writer->save(static_cast<T*>(other));
    // call regular serialize
    serialize(*writer, *static_cast<T*>(other));
  }
};

template <class Reader, class T>
class info<Reader, T,
	   typename std::enable_if<std::is_base_of<StreamReader, Reader>::value>::type>
  : public info_base
{
public:
  info(Reader &w): reader(&w) { }
  
  Reader* reader;
  
  virtual bool is_same_type(void* other, const std::type_info& id_info)
  {
    return (typeid(T) == id_info);
  }

  virtual std::string key()
  {
    return ((std::string) typeid(T).name());
  }
  
  template <class Other>
  T cast(Other other) { return dynamic_cast<T>(other); }
  
  virtual void* construct()
  {
    return new T;
  }

  virtual void* call_deserialize()
  {
    T* actual_obj_ptr = static_cast<T*>(construct());

    // call load on the pointer - allows us to specialize
    reader->load(*actual_obj_ptr);

    // deserialize on the object
    deserialize(*reader, *actual_obj_ptr);

    return actual_obj_ptr;
  }

};

static std::vector<info_base*> vinfo;

template <class X>
info_base* get_matching_type(X obj)
{
  for (size_t i = 0; i < vinfo.size(); ++i)
    if (vinfo[i]->is_same_type(obj, typeid(*obj)))
      {
	std::cout<<"typeid of obj is: "<<typeid(*obj).name()<<std::endl;
	return vinfo[i];
      }
  return NULL;
}

inline info_base* get_type_from_key(std::string type_key)
{
  for (size_t i = 0; i < vinfo.size(); ++i)
    if (vinfo[i]->key() == type_key)
      {
	std::cout<<"Returning object for key: "<<type_key<<std::endl;
	return vinfo[i];
      }
  return NULL;
}

#endif // _TYPES_HPP
