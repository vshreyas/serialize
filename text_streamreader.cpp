#include "text_streamreader.hpp"

using namespace std;

TextStreamReader::TextStreamReader(istream & m_stream): StreamReader(m_stream)
{
}

TextStreamReader::~TextStreamReader()
{
}
