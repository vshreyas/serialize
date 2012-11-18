#include <iostream>

#include "streamwriter.hpp"

using namespace std;

StreamWriter::StreamWriter(ostream& m_stream): stream(&m_stream)
{
}

StreamWriter::~StreamWriter()
{
}
