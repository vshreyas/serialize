#include "serialize.hpp"

/** Functions to pack/unpack a double into a portable machine independent format
 *  Courtesy Beej "Brian Jorgensen" Hall in Beej's guide to Network Programming
 *  Takes as argument the number of mantissa and exponent bits and stores
 *  Modified slightly to handle NaN and Infinity
*/
uint64_t pack754(long double f, unsigned bits, unsigned expbits)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (f == 0.0) return 0; // get this special case out of the way

    // check sign and begin normalization
    if (f < 0) { sign = 1; fnorm = -f; }
    else { sign = 0; fnorm = f; }

    // get the normalized form of f and track the exponent
    shift = 0;
    while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL<<significandbits) + 0.5f);

    // get the biased exponent
    exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

    // return the final answer
    return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
}

long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0) return 0.0;

    // pull the significand
    result = (i&((1LL<<significandbits)-1)); // mask
    result /= (1LL<<significandbits); // convert back to float
    result += 1.0f; // add the one back on

    // deal with the exponent
    bias = (1<<(expbits-1)) - 1;
    shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
    while(shift > 0) { result *= 2.0; shift--; }
    while(shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (i>>(bits-1))&1? -1.0: 1.0;

    return result;
}

StreamReader::StreamReader() {}
StreamReader::StreamReader(std::istream& is)
{
    this->stream = &is;
}
StreamReader::~StreamReader()
{
}

StreamWriter::StreamWriter() {}
StreamWriter::StreamWriter(std::ostream& os)
{
    this->stream = &os;
}
StreamWriter::~StreamWriter() {}

BinaryStreamReader:: BinaryStreamReader(std::istream& is)
{
    this->stream = &is;
}

BinaryStreamReader:: ~BinaryStreamReader()
{

}

BinaryStreamReader& BinaryStreamReader::operator>>(char & c)
{
    stream->read(&c, sizeof(c));
    return *this;
}

BinaryStreamReader& BinaryStreamReader::operator>>(int & i)
{
    stream->read((char*)&i, sizeof(i));
    return *this;
}

BinaryStreamReader& BinaryStreamReader::operator>>(unsigned int & ui)
{
    stream->read(reinterpret_cast<char*>(&ui), sizeof(ui));
    return *this;
}

/** @brief reads a 32-bit IEEE 756 floating point
  * from binary stream
  */
BinaryStreamReader & BinaryStreamReader::operator>>(float& f)
{
    stream->read(reinterpret_cast<char*>(&f), sizeof(f));
    return *this;
}


BinaryStreamReader& BinaryStreamReader::operator>>(double& d)
{
    stream->read(reinterpret_cast<char*>(&d), sizeof(d));
    return *this;
}

BinaryStreamReader& BinaryStreamReader::operator>>(std::string &s)
{
    unsigned int size;
    *this>>size;
    std::vector<char> temp(size);
    stream->read(&temp[0], size);
    s.assign(&temp[0], size);
}


BinaryStreamWriter::BinaryStreamWriter()
{

}

BinaryStreamWriter::BinaryStreamWriter(std::ostream& os)
{
    this->stream = &os;
}

BinaryStreamWriter::~BinaryStreamWriter()
{

}

BinaryStreamWriter& BinaryStreamWriter::operator<<(const char & c)
{
    stream->write(reinterpret_cast<const char*>(&c), sizeof(c));
    return *this;
}
BinaryStreamWriter& BinaryStreamWriter::operator<<(const int & i)
{
    stream->write(reinterpret_cast<const char*>(&i), sizeof(i));
    return *this;
}

BinaryStreamWriter& BinaryStreamWriter::operator<<(const unsigned int & ui)
{
    stream->write(reinterpret_cast<const char*>(&ui), sizeof(ui));
    return *this;
}

BinaryStreamWriter& BinaryStreamWriter::operator<<(const double &d)
{
    stream->write(reinterpret_cast<const char*>(&d), sizeof(d));
    return *this;
}

BinaryStreamWriter& BinaryStreamWriter::operator<<(const std::string &s)
{
    *this << s.size();
    stream->write(s.c_str(), s.size());
    return *this;
}
