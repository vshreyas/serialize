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
private:
  int int_mem;
  string string_mem;
public:
  myclass(): int_mem(30),
	     string_mem("this is my string member!\n\nYou should see two new lines.")
  { }

  friend void write_data(TextStreamWriter& writer, const myclass& cls);
};

void write_data(TextStreamWriter& writer, const myclass& cls)
{
  writer<<cls.int_mem;
  writer<<cls.string_mem;
}

int main()
{
  char char_data = 'C';
  int int_data = 225;
  double double_data = 351.258935;
  int int_array[10] = {1,2,3,4,5,6,7,8,9,10};
  string str_array[] = {"abc", "another one", "third\none", "fourth"};
  //string string_data = "this is a string.";
  string string_data = "saf";
  myclass cls;
  
  ofstream os("out.txt");
  TextStreamWriter writer(os);
  writer<<char_data<<int_data<<double_data<<string_data<<int_array<<str_array;//<<cls;
  os.close();

  char char_read;
  int int_read;
  double double_read;
  string string_read;

  ifstream is("out.txt");
  TextStreamReader reader(is);
  reader>>char_read>>int_read>>double_read>>string_read;//>>cls;
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
  cout<<"Type of int* is: "<<typeid(int*).name()<<endl;
  return 0;
}
