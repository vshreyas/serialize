#include "binary_streamreader.hpp"
#include "binary_streamwriter.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
#include <vector>

using namespace std;

//template <class T>
class myclass
{
public:
  int int_mem;
  string string_mem;
public:
  myclass(): int_mem(30),
	     string_mem("this is my string member!\n\nYou should see two new lines.")
  { }

  void set_int_mem(int i) { int_mem = i; }
  void set_string_mem(string s) { string_mem = s; }

  template <class Writer>
  friend void serialize(Writer& writer, const myclass& cls);
  template <class Reader>
  friend void deserialize(Reader& reader, myclass& cls);
};

template <class Writer>
void serialize(Writer& writer, const myclass& cls)
{
  writer<<cls.int_mem;
  writer<<cls.string_mem;
}

template <class Reader>
void deserialize(Reader& reader, myclass& cls)
{
  reader>>cls.int_mem;
  reader>>cls.string_mem;
}

class derived_myclass: public myclass
{
private:
  float float_mem;
  char char_mem;
public:
  derived_myclass(): myclass(), float_mem(2124.35), char_mem('A') { }
  void set_float_mem(float f) { float_mem = f; }
  void set_char_mem(char c) { char_mem = c; }


  template <class Writer>
  friend void serialize(Writer& writer, const derived_myclass& cls);

  //friend void deserialize(StreamReader& reader, derived_myclass& cls);

  template <class Reader>
  friend void deserialize(Reader& reader, derived_myclass& cls);
};

template <class Writer>
void serialize(Writer& writer, const derived_myclass& cls)
{
  writer<<static_cast<const myclass>(cls);
  writer<<cls.float_mem;
  writer<<cls.char_mem;
}

template <class Reader>
void deserialize(Reader& reader, derived_myclass& cls)
{
  reader>>*static_cast<myclass*>(&cls);
  reader>>cls.float_mem;
  reader>>cls.char_mem;
}

int main()
{
  char char_data = 'C';
  int int_data = 225;
  double double_data = 351258935;
  int int_array[10] = {1,2,3,4,5,6,7,8,9,10};
  string str_array[] = {"abc", "another one", "third\none", "fourth"};
  vector<int> v = {1, 2, 3};
  string string_data = "saf\ndfew";
  myclass cls;
  cls.set_int_mem(314);
  cls.set_string_mem("fde");

  derived_myclass d_cls;
  d_cls.set_int_mem(3512);
  d_cls.set_string_mem("dfnwej");

  ofstream os("out.txt", ios::out|ios::binary|ios::trunc);
  os.seekp(ios::beg);
  BinaryStreamWriter writer(os);
  writer<<char_data<<int_data<<double_data<<string_data<<int_array<<str_array<<cls<<d_cls<<v;
  os.close();

  char char_read;
  int int_read;
  double double_read;
  string string_read;
  int int_array_read[10];
  string str_array_read[4];
  vector<int> v_read;
  myclass cls_read;
 derived_myclass d_cls_read;

  ifstream is("out.txt", ios::in|ios::binary);
  BinaryStreamReader reader(is);
  reader>>char_read>>int_read>>double_read>>string_read>>int_array_read>>str_array_read>>cls_read>>d_cls_read>>v_read;
  is.close();

  if (char_read != char_data)
    cout<<"Read char: "<<char_read<<" | Expected char: "<<char_data<<endl;
  if (int_read != int_data)
    cout<<"Read int: "<<int_read<<" | Expected int: "<<int_data<<endl;
  if (double_read != double_data)
    cout<<"Read double: "<<double_read<<" | Expected double: "<<double_data<<endl;
  if (string_read != string_data)
    cout<<"Read string: "<<string_read<<" | Expected string: "<<string_data<<endl;
  for(int i=0;i<3;i++) {
    if(int_array_read[i] != int_array[i])
        cout<<"Read int array  element: "<< int_array_read[i]<<"| Expected element :"<< int_array[i]<<endl;
    if(v_read != v)
        cout<<"Read vector: "<<v_read.at(i)<<" | Expected vector: "<<v.at(i)<<endl;
  }
  if(cls_read.int_mem != cls.int_mem || cls_read.string_mem != cls.string_mem)
    cout<<"Read class: "<<cls_read.string_mem<<", "<<cls_read.int_mem<<endl;
  if(d_cls_read.int_mem != d_cls.int_mem || d_cls_read.string_mem != d_cls.string_mem)
    cout<<"Read class: "<<d_cls_read.string_mem<<", "<<d_cls_read.int_mem<<endl;
  return 0;
}
