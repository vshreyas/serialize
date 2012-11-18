#include "text_streamreader.hpp"
#include "text_streamwriter.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>

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
  
  friend void serialize(TextStreamWriter& writer, const myclass& cls);
  friend void deserialize(TextStreamReader& reader, myclass& cls);
};

void serialize(TextStreamWriter& writer, const myclass& cls)
{
  writer<<cls.int_mem;
  writer<<cls.string_mem;
}

void deserialize(TextStreamReader& reader, myclass& cls)
{
  reader>>cls.int_mem;
  reader>>cls.string_mem;
}

class d_myclass: public myclass
{
private:
  float float_mem;
  char char_mem;
public:
  d_myclass(): myclass(), float_mem(2124.35), char_mem('A') { }
  void set_float_mem(float f) { float_mem = f; }
  void set_char_mem(char c) { char_mem = c; }

  friend void serialize(TextStreamWriter& writer, const d_myclass& cls);
  friend void deserialize(TextStreamReader& writer, d_myclass& cls);
};

void serialize(TextStreamWriter& writer, const d_myclass& cls)
{
  writer<<static_cast<const myclass>(cls);
  writer<<cls.float_mem;
  writer<<cls.char_mem;
}

void deserialize(TextStreamReader& reader, d_myclass& cls)
{
  reader>>*static_cast<myclass*>(&cls);
  reader>>cls.float_mem;
  reader>>cls.char_mem;
}

int main()
{
  char char_data = 'C';
  int int_data = 225;
  double double_data = 351.258935;
  int int_array[10] = {1,2,3,4,5,6,7,8,9,10};
  string str_array[] = {"abc", "another one", "third\none", "fourth"};
  //string string_data = "this is a string.";
  string string_data = "saf\ndfew";
  myclass cls;
  cls.set_int_mem(314);
  cls.set_string_mem("fde");
  d_myclass d_cls;
  d_cls.set_int_mem(3512);
  d_cls.set_string_mem("dfnwej");
  
  ofstream os("out.txt");
  TextStreamWriter writer(os);
  writer<<char_data<<int_data<<double_data<<string_data<<int_array<<str_array<<cls<<d_cls;
  os.close();

  char char_read;
  int int_read;
  double double_read;
  string string_read;
  int int_array_read[10];
  string str_array_read[4];
  myclass cls_read;
  d_myclass d_cls_read;
  
  ifstream is("out.txt");
  TextStreamReader reader(is);
  reader>>char_read>>int_read>>double_read>>string_read>>int_array_read>>str_array_read>>cls_read>>d_cls_read;
  cout<<"int mem of derived = "<<d_cls_read.int_mem<<endl;
  is.close();
  
  if (char_read != char_data)
    cout<<"Read char: "<<char_read<<" | Expected char: "<<char_data<<endl;
  if (int_read != int_data)
    cout<<"Read int: "<<int_read<<" | Expected int: "<<int_data<<endl;
  if (double_read != double_data)
    cout<<"Read double: "<<double_read<<" | Expected double: "<<double_data<<endl;

  if (string_read != string_data)
    cout<<"Read string: "<<string_read<<" | Expected string: "<<string_data<<endl;


  //cout<<"Type of myclass is: "<<typeid((myclass<int>*) new myclass<int>()).name()<<endl;
  cout<<"type of myclass:"<<typeid(myclass).name()<<endl;
  cout<<"Type of int* is: "<<typeid(int*).name()<<endl;
  return 0;
}
