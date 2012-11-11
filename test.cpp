#include "text_streamreader.hpp"
#include "text_streamwriter.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
  char char_data = 'C';
  int int_data = 225;
  double double_data = 351.258935;
  //string string_data = "this is a string.";
  string string_data = "saf";
  
  ofstream os("out.txt");
  TextStreamWriter writer(os);
  writer<<char_data<<int_data<<double_data<<string_data;
  os.close();

  char char_read;
  int int_read;
  double double_read;
  string string_read;

  ifstream is("out.txt");
  TextStreamReader reader(is);
  reader>>char_read>>int_read>>double_read>>string_read;
  is.close();
  
  if (char_read != char_data)
    cout<<"Read char: "<<char_read<<" | Expected char: "<<char_data<<endl;
  if (int_read != int_data)
    cout<<"Read int: "<<int_read<<" | Expected int: "<<int_data<<endl;
  if (double_read != double_data)
    cout<<"Read double: "<<double_read<<" | Expected double: "<<double_data<<endl;

  if (string_read != string_data)
    cout<<"Read string: "<<string_read<<" | Expected string: "<<string_data<<endl;

  return 0;
}
