#ifndef _TYPES_HPP
#define _TYPES_HPP

#include "common.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <typeinfo>
#include <map>

class StreamReader;
class StreamWriter;
class InfoBase;

using namespace std;

/*
static std::map<std::string,InfoBase*> info_class_name_map;
static std::map<std::string,InfoBase*> info_typeid_name_map;
*/

#define REGISTER_TYPE(writer,type)					\
  register_type(writer,new Info< type > (string(#type)) );		\

class InfoBase
{
public:
  InfoBase(const string & _key): m_key{_key} { }
  
  virtual bool is_same_type(const std::type_info& id_info)
  {
    NOT_IMPLEMENTED("is_same_type!\n");
  }
  
  virtual void* construct() = 0;

  virtual std::string key() { return m_key; }

private:
  string m_key;
};

template <class T>
class Info: public InfoBase
{
public:
  Info(const string &_key): InfoBase(_key) { }

  virtual bool is_same_type(const std::type_info& id_info)
  {
    return (typeid(T) == id_info);
  }

  template <class Other>
  T cast(Other other) { return dynamic_cast<T>(other); }
  
  virtual void* construct()
  {
    return new T;
  }
};

/*
template <class X>
InfoBase* get_matching_type(X obj)
{
  for (size_t i = 0; i < vinfo.size(); ++i)
    if (vinfo[i]->is_same_type(obj, typeid(*obj)))
      {
	std::cout<<"typeid of obj is: "<<typeid(*obj).name()<<std::endl;
	return vinfo[i];
      }
  return NULL;
}

inline InfoBase* get_type_from_key(std::string type_key)
{
  for (size_t i = 0; i < vinfo.size(); ++i)
    if (vinfo[i]->key() == type_key)
      {
	std::cout<<"Returning object for key: "<<type_key<<std::endl;
	return vinfo[i];
      }
  return NULL;
}
*/

// Initialize the vector
template <class Writer>
class TiedInfoBase
{
public:
  TiedInfoBase() { }
  
  virtual ~TiedInfoBase() { }

  string key() { return get_key(); }
  
  bool is_same_type(void* other, const std::type_info & id_info)
  {
    cout<<"In is_same_type. Given type name: "<<id_info.name()<<"\n";
    return check_if_same_type(other, id_info);
  }
  
  void call_serialize(Writer & writer, void* other)
  {
    cast_and_call_serialize(writer, other);
  }

private:

  virtual string get_key()
  {
    NOT_IMPLEMENTED("not implemented get_key() called!"); return "";
  }
  
  virtual void cast_and_call_serialize(Writer & writer, void* other)
  {
    NOT_IMPLEMENTED("Not implemented cast_and_call_serialize called!\n");
  }
  
  virtual bool check_if_same_type(void* other, const std::type_info & id_info)
  {
    NOT_IMPLEMENTED("Not implemented check_if_same_type called!\n");
  }
};

template <class Writer,class InfoType>
class TiedInfo: public TiedInfoBase<Writer>
{
public:
  TiedInfo(Info<InfoType> _info): TiedInfoBase<Writer>(), m_info(_info) { }

private:

  virtual string get_key() { return m_info.key(); }
  
  virtual void cast_and_call_serialize(Writer & writer, void* other)
  {
    // call save on the pointer so we can specialize
    writer.save(static_cast<InfoType*>(other));
    // call regular serialize
    serialize(writer, *static_cast<InfoType*>(other));
  }

  virtual bool check_if_same_type(void* other, const std::type_info & id_info)
  {
    return m_info.is_same_type(id_info);
  }

  Info<InfoType> m_info;
};

template <class Writer>
struct InfoList
{
  using ptr_type = TiedInfoBase<Writer>*;

  static vector<ptr_type> info_list;

  static void add_type(ptr_type tied_info)
  {
    // check if type exists already first
    info_list.push_back(tied_info);
  }

  template <class GivenType>
  static ptr_type get_matching_type(GivenType obj)
  {
    /*auto found_item = find_if(info_list.begin(), info_list.end(),
			      [&obj] (ptr_type elem) {
				return elem->is_same_type(obj, typeid(*obj));
				});*/

    for (size_t i = 0; i < info_list.size(); ++i)
      {
	if (info_list[i]->is_same_type(obj,typeid(*obj)))
	  return info_list[i];
      }

    return nullptr;

    /*if (found_item == info_list.end())
      return nullptr;
      return *found_item;*/
  }
};

template <class Writer>
vector<TiedInfoBase<Writer>*> InfoList<Writer>::info_list {};

#endif // _TYPES_HPP
