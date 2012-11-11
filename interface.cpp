// streamreader.hpp
class StreamReader
{
public:
  StreamReader();
  StreamReader(const std::istream&);
  ~StreamReader();		// don't close stream here

  void set_stream(const std::istream&);
  std::istream& get_stream();
  
protected:
  std::istream& stream;
};

// streamwriter.hpp
class StreamWriter
{
public:
  StreamWriter();
  StreamWriter(std::ostream&);
  ~StreamWriter();		// don't close stream here
  
  void set_stream(std::ostream&);
  std::ostream& get_stream();
  
protected:
  std::ostream& stream;
};

// binary_streamreader.hpp
// Read from stream and store into object
class BinaryStreamReader: public StreamReader
{
public:
  BinaryStreamReader();
  BinaryStreamReader(const std::istream&);
  ~BinaryStreamReader();

  BinaryStreamReader& operator>>(char &);
  BinaryStreamReader& operator>>(int &);
  BinaryStreamReader& operator>>(double &);
  BinaryStreamReader& operator>>(string &);
};

// No need to implement for basic types
template <class T>
BinaryStreamReader& operator>>(BinaryStreamReader&, T&);

// binary_streamwriter.hpp
// Write object to stream without modifying given object
class BinaryStreamWriter: public StreamWriter
{
public:
  BinaryStreamWriter();
  BinaryStreamWriter(std::ostream&);
  ~BinaryStreamWriter();

  BinaryStreamWriter& operator<<(const char &);
  BinaryStreamWriter& operator<<(const int &);
  BinaryStreamWriter& operator<<(const double &);
  BinaryStreamWriter& operator<<(const string &);
};

// No need to implement for basic types
template <class T>
BinaryStreamWriter& operator<<(BinaryStreamWriter&, const T&);

// test_primitive_types.cpp
// For testing purposes. If all goes well, there should be no output.
#include "binary_streamreader.hpp"
#include "binary_streamwriter.hpp"

#include <iostream>
#include <string>

using namespace std;

int main()
{
  char char_data = 'C';
  int int_data = 225;
  double double_data = 351.258935;
  string string_data = "this is a string.";
  
  ostream os("out.txt");
  BinaryStreamWriter writer(os);
  os<<char_data<<int_data<<double_data<<string_data;
  os.close();

  char char_read;
  int int_read;
  double double_read;
  string string_read;

  istream is("out.txt");
  BinaryStreamReader reader(is);
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
