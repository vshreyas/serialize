// text_streamwriter.hpp
// Write object to stream without modifying given object
#ifndef TEXT_STREAMWRITER_HPP
#define TEXT_STREAMWRITER_HPP

#include <iostream>
#include <string>
#include <typeinfo>

#include "streamwriter.hpp"

class TextStreamWriter: public StreamWriter
{
public:
  //TextStreamWriter();
  TextStreamWriter(std::ostream&);
  ~TextStreamWriter();
  
  template <typename T>
  typename std::enable_if<std::is_fundamental<T>::value>::type
  save(const T & T_data)
  {
    write_type(T_data);
    write_data(T_data);
  }

  void save(const std::string & string_data)
  {
    write_type(string_data);
    write_data(string_data);
  }
  
  template <typename T>
  typename std::enable_if<std::is_class<T>::value>::type
  save(const T & T_data)
  {
    write_type(T_data);
    //serialize(*this, T_data);
  }

  template <typename T>
  typename std::enable_if<std::is_polymorphic<T>::value>::type
  save(T* T_data)
  {
    write_type(*T_data);
    write_type(*T_data);
    //serialize(*this, T_data);
  }
  
  template <typename T>
  typename std::enable_if<std::is_array<T>::value>::type
  save(const T & T_data)
  {
    write_type(T_data);
    // number of elements
    *this<<(sizeof(T_data)/sizeof(T_data[0]));
    for (size_t i = 0; i < sizeof(T_data)/sizeof(T_data[0]); ++i)
      *this<<T_data[i];
  }
  
private:
  template <class T>
  std::string type_and_delim(const T &)
  {
    return std::string(typeid(T).name()) + " ";
  }

  template <class T>
  void write_type(const T & T_data)
  {
    // Assuming no type name contains a word/string terminator, no
    // need to store length
    std::string type_as_string(typeid(T_data).name());
    *stream<<type_as_string<<" ";
  }

  template <class T>
  void write_data(const T & T_data)
  {
    *stream<<T_data<<std::endl;
  }

  void write_data(const std::string& string_data)
  {
    *stream<<string_data.size()<<" "<<string_data<<std::endl;
  }
};

#endif // TEXT_STREAMWRITER_HPP

/*
  [http://www.parashift.com/c++-faq-lite/serialize-text-format.html]
  
  Serializing in text format:
  ---------------------------
  
  Before you read this, make sure to evaluate all the tradeoffs between
  human-readable and non-human-readable formats. The tradeoffs are
  non-trivial, so you should resist a knee-jerk reaction to do it the
  way you did it on the last project — one size does not fit all.

  After you have made an eyes-open decision to use human-readable
  ("text") format, you should remember these keys:

  You probably want to use iostream's >> and << operators rather
  than its read() and write() methods. The >> and << operators are
  better for text mode, whereas read() and write() are better for
  binary mode.
    
  When storing numbers, you'll probably want to add a separator to
  prevent items from running together. One simple approach is to
  always add a space (' ') before each number, that way the number 1
  followed by the number 2 won't run together and look like a
  12. Since the leading space will automatically get soaked up by
  the >> operator, you won't have to do anything explicit to extract
  the leading space in the code that reads things back in.
    
  String data is tricky because you have to unambiguously know when
  the string's body stops. You can't unambiguously terminate all
  strings with a '\n' or '"' or even '\0' if some string might
  contain those characters. You might want to use C++ source-code
  escape-sequences, e.g., writing '\' followed by 'n' when you see a
  newline, etc. After this transformation, you can either make
  strings go until end-of-line (meaning they are deliminated by
  '\n') or you can delimit them with '"'.
    
  If you use C++-like escape-sequences for your string data, be sure
  to always use the same number of hex digits after '\x' and '\u'. I
  typically use 2 and 4 digits respectively. Reason: if you write a
  smaller number of hex digits, e.g., if you simply use stream <<
  "\\x" << hex << unsigned(theChar), you'll get errors when the next
  character in the string happens to be a hex digit. E.g., if the
  string contains '\xF' followed by 'A', you should write "\x0FA",
  not "\xFA".

  If you don't use some sort of escape sequence for characters like
  '\n', be careful that the operating system doesn't mess up your
  string data. In particular, if you open a std::fstream without
  std::ios::binary, some operating systems translate end-of-line
  characters.

  Another approach for string data is to prefix the string's data
  with an integer length, e.g., to write "now is the time" as 15:now
  is the time. Note that this can make it hard for people to
  read/write the file, since the value just after that might not
  have a visible separator, but you still might find it useful.

  Please remember that these are primitives that you will need to use
  in the other FAQs in this section.
*/
