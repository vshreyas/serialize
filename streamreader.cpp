#include <iostream>

#include "streamreader.hpp"

using namespace std;

StreamReader::StreamReader(istream& m_stream): stream(&m_stream)
{
}

StreamReader::~StreamReader()
{
}
