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

#define REGISTER_TYPE(writer,type)					\
  register_type(writer,new Info< type > (string(#type)) );		\

/**
 * InfoBase - stores a string mapping to a type and provides a method
 * to check if the type mapped to has the same type as a given object.
 * 
 */
class InfoBase
{
public:
  /** 
   * Set the `key', a string representation for the class.
   *
   * @param _key string to represent this instance.
   */
  InfoBase(const string & _key): m_key{_key} { }
  
  /** 
   * Return if the type this instance represents is of the same type
   * as another at runtime.
   *
   * @param id_info type_info of an object (returned by typeid(object)).
   *
   * @return true if the types are the same at runtime.
   */
  virtual bool is_same_type(const type_info& id_info)
  {
    NOT_IMPLEMENTED("is_same_type!\n");
  }

  /** 
   * Construct an object of the type represented by this instance.
   *
   * @return void pointer to new object
   */
  virtual void* construct() = 0;

  /** 
   * Returns the key.
   *
   * @return key (string)
   */
  virtual string key() { return m_key; }

private:
  string m_key;
};

/**
 * Info<T> represents type T, i.e., is_same_type(typeid(T)) should
 * return true for an Info<T> object.
 *
 * The key is supplied from an external source.
 */
template <class T>
class Info: public InfoBase
{
public:
  Info(const string &_key): InfoBase(_key) { }

  virtual bool is_same_type(const type_info& id_info)
  {
    return (typeid(T) == id_info);
  }
  
  virtual void* construct()
  {
    return new T;
  }
};

// Defining necessary default (never-used) instantiations
template <class T, class Enable = void>
class TiedInfoBase
{
};

// Defining necessary default (never-used) instantiations
template <class T, class U, class Enable = void>
class TiedInfo
{
};

/**
 * TiedInfoBase - refer to InfoBase.
 *
 * This provides a way to serialize a polymorphic object (via its
 * pointer) to a given StreamWriter object in addition to the InfoBase
 * features.
 *
 * This is `tied' to a particular derived class of StreamWriter
 * through the template parameter, in contrast to InfoBase which does
 * not make use of the Stream* classes.
 */
template <class Writer>
class TiedInfoBase<Writer,
		   typename enable_if<is_base_of<StreamWriter,Writer>::value>::type>
{
public:
  TiedInfoBase() { }
  
  virtual ~TiedInfoBase() { }

  string key() { return get_key(); }
  
  bool is_same_type(void* other, const type_info & id_info)
  {
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
  
  virtual bool check_if_same_type(void* other, const type_info & id_info)
  {
    NOT_IMPLEMENTED("Not implemented check_if_same_type called!\n");
  }
};

/**
 * TiedInfo - templated on both Writer type and InfoType (essentially
 * the type represented by the TiedInfo),
 *
 * The Info<InfoType> object's member function is called to check if
 * another object is of the same type as InfoType.
 */
template <class Writer,class InfoType>
class TiedInfo<Writer, InfoType,
	       typename enable_if<is_base_of<StreamWriter,Writer>::value>::type>
  : public TiedInfoBase<Writer>
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

  virtual bool check_if_same_type(void* other, const type_info & id_info)
  {
    return m_info.is_same_type(id_info);
  }

  Info<InfoType> m_info;
};

template <class Reader>
class TiedInfoBase<Reader,
		   typename enable_if<is_base_of<StreamReader,Reader>::value>::type>
{
public:
  TiedInfoBase() { }
  
  virtual ~TiedInfoBase() { }

  string key() { return get_key(); }
  
  bool is_same_type(void* other, const type_info & id_info)
  {
    cout<<"In is_same_type. Given type name: "<<id_info.name()<<"\n";
    return check_if_same_type(other, id_info);
  }

  void* call_deserialize(Reader & reader)
  {
    return construct_and_call_deserialize(reader);
  }
    
private:

  virtual string get_key()
  {
    NOT_IMPLEMENTED("not implemented get_key() called!"); return "";
  }
  
  virtual void* construct_and_call_deserialize(Reader & reader)
  {
    NOT_IMPLEMENTED("Not implemented construct_and_call_deserialize called!\n");
  }
  
  virtual bool check_if_same_type(void* other, const type_info & id_info)
  {
    NOT_IMPLEMENTED("Not implemented check_if_same_type called!\n");
  }
};

template <class Reader,class InfoType>
class TiedInfo<Reader,InfoType,
	       typename enable_if<is_base_of<StreamReader,Reader>::value>::type>
  : public TiedInfoBase<Reader>
{
public:
  TiedInfo(Info<InfoType> _info): TiedInfoBase<Reader>(), m_info(_info) { }

private:

  virtual string get_key() { return m_info.key(); }
  
  virtual void* construct_and_call_deserialize(Reader & reader)
  {
    InfoType* derived_ptr = new InfoType;
    reader>>*derived_ptr;
    
    return derived_ptr;
  }

  virtual bool check_if_same_type(void* other, const type_info & id_info)
  {
    return m_info.is_same_type(id_info);
  }

  Info<InfoType> m_info;
};


// List of known/registered types.  It is specific to a
// StreamReader/StreamWriter class so that the corresponding TiedInfo
// objects can be used rather than the plain Info objects which do not
// have the capability to serialize or deserialize.
template <class ReaderWriter>
struct InfoList
{
  using ptr_type = TiedInfoBase<ReaderWriter>*;

  static vector<ptr_type> info_list;

  static void add_type(ptr_type tied_info)
  {
    // check if type exists already first
    info_list.push_back(tied_info);
  }

  /** 
   * Returns the TiedInfo object which represents the same type as
   * that of the given polymorphic object (assumed to be a pointer!).
   *
   * @param obj pointer to a polymorphic object
   *
   * @return Pointer to matching TiedInfoBase object in the list, or
   * null pointer if none exists.
   */
  template <class GivenType>
  static ptr_type get_matching_type(GivenType* obj)
  {
    for (size_t i = 0; i < info_list.size(); ++i)
      {
	if (info_list[i]->is_same_type(obj,typeid(*obj)))
	  return info_list[i];
      }

    return nullptr;
  }

  /** 
   * Returns the TiedInfo object which has a given key.  Useful while
   * deserializing, as the type read from the file (key) is compared
   * with the keys of the known types (types in this list).
   *
   * @param _key key to compare
   *
   * @return Pointer to TiedInfoBase object with matching key, or null
   * pointer if none exists in the list.
   */
  static ptr_type get_matching_type_by_key(string _key)
  {
    for (size_t i = 0; i < info_list.size(); ++i)
      {
	if (info_list[i]->key() == _key)
	  return info_list[i];
      }

    return nullptr;
  }
};

template <class ReaderWriter>
vector<TiedInfoBase<ReaderWriter>*> InfoList<ReaderWriter>::info_list {};

template <class ReaderWriter, typename T>
void register_type(ReaderWriter & writer, Info<T>* info)
{
  InfoList<ReaderWriter>::add_type(new TiedInfo<ReaderWriter,T>(*info));
}

#endif // _TYPES_HPP
