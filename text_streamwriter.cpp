#include <iostream>

#include "text_streamwriter.hpp"

using namespace std;

TextStreamWriter::TextStreamWriter(ostream& m_stream): StreamWriter(m_stream)
{
}

TextStreamWriter::~TextStreamWriter()
{
}
