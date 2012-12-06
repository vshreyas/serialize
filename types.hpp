/**
 * @file   types.hpp
 * @author Shashwat Lal Das <shashwat@mashed-potato>
 * @date   Wed Dec  5 21:34:37 2012
 * 
 * @brief Defines the classes used for storing and retrieving type
 * information.  Classes from this file are useful in getting the
 * unique key for each derived class type. The key is supplied by the
 * user in some way. Here, the macro REGISTER_TYPE is provided for
 * this purpose.
 * 
 * eg.
 * Base* b = new Derived;
 * TextStreamWriter w;
 * REGISTER_TYPE(w, Derived);
 */

#ifndef _TYPES_HPP
#define _TYPES_HPP

#include "common.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <map>

class StreamReader;
class StreamWriter;
class InfoBase;

using namespace std;

// Registers a type T by creating a new Info<T> object and setting the
// key to be the literal "T", i.e., the name of the type.
#define REGISTER_TYPE(writer,type)				\
  register_type(writer,new Info< type > (string(#type)) );

/**
 * Exception to be thrown when it is found that there is no registered
 * type matching a given polymorphic object's type.
 */
class TypeNotRegisteredException: public exception
{
public:
  /** 
   * Default - do not know which type is not registered.
   */
  TypeNotRegisteredException(): m_type_key("") { }

  /** 
   * Know that a type with "type_key" is not registered. Output
   * type_key in the exception message. (occurs during
   * deserialization)
   *
   * @param type_key key of the type which is not registered.
   */
  TypeNotRegisteredException(string type_key): m_type_key(type_key) { }
  
  virtual const char* what() const throw(){
    if (m_type_key == "")
      return "A derived class type was not registered. (Can't tell which.)";
	
    stringstream ss (stringstream::in | stringstream::out);

    ss<<"A derived class type was not registered. The type key is: "<<m_type_key;

    return ss.str().c_str();
  }

private:
  string m_type_key;		// the 'key' of the type, if we know it
};


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
    return false;
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

  /** 
   * Return the `key'. The key represents a unique string identifier
   * for the type being represented by this object.
   *
   *
   * @return key, a unique string for each type represented.
   */
  string key() { return get_key(); }

  /** 
   * Check if the type represented by this object is the same as the
   * dynamic type (using argument of type type_info) of another
   * object.
   *
   * @param other pointer to the object
   * @param id_info result of typeid(*object)
   *
   * @return true if this object represents the same type
   */  
  bool is_same_type(void* other, const type_info & id_info)
  {
    return check_if_same_type(other, id_info);
  }

  /** 
   * Serializes the given object using the writer, treating the given
   * object to be of the type represented by this object.
   *
   * @param writer Writer (class template parameter) object
   * @param other given polymorphic object
   */
  void call_serialize(Writer & writer, void* other)
  {
    cast_and_call_serialize(writer, other);
  }
    
private:
  virtual string get_key()
  {
    NOT_IMPLEMENTED("not implemented get_key() called!");
    return "";
  }
  
  virtual void cast_and_call_serialize(Writer & writer, void* other)
  {
    NOT_IMPLEMENTED("Not implemented cast_and_call_serialize called!\n");
  }
  
  virtual bool check_if_same_type(void* other, const type_info & id_info)
  {
    NOT_IMPLEMENTED("Not implemented check_if_same_type called!\n");
    return false;
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
  /** 
   * Construct using a given Info<T> object. The Info<T> object has
   * functions to check whether a given polymorphic object is of type
   * T.
   *
   * @param _info Info<InfoType> object, where InfoType is a
   * template parameter of this class.
   */
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

  Info<InfoType> m_info;	/**< member whose job is to check if given object is of type T */
};

/**
 * Same as TiedInfoBase<Writer>, except that this is used for
 * deserializing.
 * 
 */
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

  /** 
   * Deserialize the given object from the stream. Construct the new
   * object (of correct type) first. Then deserialize into that
   * object. Return void* pointer to the constructed + deserialized
   * object.
   *
   * @param reader StreamReader descendant
   *
   * @return Pointer to constructed and deserialized object
   */
  void* call_deserialize(Reader & reader)
  {
    return construct_and_call_deserialize(reader);
  }
    
private:

  virtual string get_key()
  {
    NOT_IMPLEMENTED("not implemented get_key() called!");
    return "";
  }
  
  virtual void* construct_and_call_deserialize(Reader & reader)
  {
    NOT_IMPLEMENTED("Not implemented construct_and_call_deserialize called!\n");
  }
  
  virtual bool check_if_same_type(void* other, const type_info & id_info)
  {
    NOT_IMPLEMENTED("Not implemented check_if_same_type called!\n");
    return false;
  }
};

/**
 * Same as TiedInfoBase<Writer,InfoType>, except that this is used for
 * deserializing.
 * 
 */
template <class Reader,class InfoType>
class TiedInfo<Reader,InfoType,
	       typename enable_if<is_base_of<StreamReader,Reader>::value>::type>
  : public TiedInfoBase<Reader>
{
public:
  TiedInfo(Info<InfoType> _info): TiedInfoBase<Reader>(), m_info(_info) { }

private:

  virtual string get_key() { return m_info.key(); }

  /** 
   * The type of the object is assumed to be the template parameter of
   * this class, InfoType. Constructs an object of this type and
   * deserializes into that object.
   *
   * @param reader StreamReader descendant
   *
   * @return Pointer to the created object
   */  
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

  static map<string,ptr_type> info_list;

  static void add_type(ptr_type tied_info)
  {
    // check if type exists already first
    if (info_list.count(tied_info->key()))
      delete tied_info;
    else
      info_list[tied_info->key()] = tied_info;
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
    for (auto info_pair: info_list)
      {
	auto info_current = info_pair.second;
	if (info_current->is_same_type(obj,typeid(*obj)))
	  return info_current;
      }

    throw TypeNotRegisteredException();
    
    return nullptr;		// from an earlier version
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
    auto info_iter = info_list.find(_key);

    if (info_iter == info_list.end())
      {
	throw TypeNotRegisteredException(_key);
	return nullptr;
      }

    return info_iter->second;
  }
};

template <class ReaderWriter>
map<string,TiedInfoBase<ReaderWriter>*> InfoList<ReaderWriter>::info_list = { };

/** 
 * Registers a type by creating a
 * TiedInfo<StreamReader/StreamWriter,T> object from an Info<T> object.
 *
 * The Info<T> object has a key associated with it. T is the actual
 * type being represented by the Info/TiedInfo object, and the key is
 * the identifier associated with this Info/TiedInfo object.
 *
 * No two Info/TiedInfo objects with identical template parameters can
 * have different keys. The key then represents a unique identifier
 * for type T.
 *
 * @param readerwriter StreamReader/StreamWriter object
 * @param info pointer to an Info<T> object
 */
template <class ReaderWriter, typename T>
void register_type(ReaderWriter & readerwriter, Info<T>* info)
{
  InfoList<ReaderWriter>::add_type(new TiedInfo<ReaderWriter,T>(*info));
}

#endif // _TYPES_HPP
