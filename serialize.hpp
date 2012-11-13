#ifndef SERIALIZE_HPP_INCLUDED
#define SERIALIZE_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <stdint.h>

class StreamReader
{
public:
    StreamReader();
    StreamReader(std::istream&);
    ~StreamReader();		// don't close stream here

protected:
    std::istream* stream;
};

// streamwriter.hpp
class StreamWriter
{
public:
    StreamWriter();
    StreamWriter(std::ostream&);
    ~StreamWriter();		// don't close stream here

protected:
    std::ostream* stream;
};

// binary_streamreader.hpp
// Read from stream and store into object
class BinaryStreamReader: public StreamReader
{
public:
    BinaryStreamReader();
    BinaryStreamReader(std::istream&);
    ~BinaryStreamReader();

    BinaryStreamReader& operator>>(char &);
    BinaryStreamReader& operator>>(unsigned int & ui);
    BinaryStreamReader&operator>>(float& f);
    BinaryStreamReader& operator>>(int &);
    BinaryStreamReader& operator>>(double &);
    BinaryStreamReader& operator>>(std::string &);
};

// No need to implement for basic types
template <class T>
BinaryStreamReader& operator>>(BinaryStreamReader&, T&);

// binary_streamwriter.hpp
// Write object to stream without modifying given object
class BinaryStreamWriter: public StreamWriter
{
private:

public:
    BinaryStreamWriter();
    BinaryStreamWriter(std::ostream&) ;
    ~BinaryStreamWriter();

    BinaryStreamWriter& operator<<(const char &);
    BinaryStreamWriter& operator<<(const int &);
    BinaryStreamWriter& operator<<(const unsigned int & ui);
    BinaryStreamWriter& operator<<(const double &);
    BinaryStreamWriter& operator<<(const std::string &);
};

// No need to implement for basic types
template <class T>
BinaryStreamWriter& operator<<(BinaryStreamWriter&, const T&);


#endif // SERIALIZE_HPP_INCLUDED
