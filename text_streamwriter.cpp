#include <iostream>

#include "text_streamwriter.hpp"

using namespace std;

TextStreamWriter::TextStreamWriter(): stream(0)
{
}

TextStreamWriter::TextStreamWriter(ostream& m_stream): stream(&m_stream)
{
}

TextStreamWriter::~TextStreamWriter()
{
}
