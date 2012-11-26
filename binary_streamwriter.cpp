#include <iostream>

#include "binary_streamwriter.hpp"

using namespace std;

BinaryStreamWriter::BinaryStreamWriter(ostream& m_stream): StreamWriter(m_stream)
{
}

BinaryStreamWriter::~BinaryStreamWriter()
{
}
