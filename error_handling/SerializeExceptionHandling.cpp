#include "SerializeExceptionHandling.hpp"
#include "stream_exception.hpp"

bool check_eof(istream& oiStream)
{
	return oiStream.eof();
}

bool check_fail(istream& oiStream)
{	
	return oiStream.fail();
}

void checkandthrowBasicException(istream& oiStream)
{
	if(!check_eof(oiStream))
	{
		EndOfFileException eofexception;
		throw eofexception;
	}
	
	if(!check_fail(oiStream))
	{
		throw FailBitException();
	}
}

std::string readString(istream& oiStream)
{
	int ipos = oiStream.tellg();
	char next = '';
	std::string str;
	while(!oiStream.eof() && !oiStream.fail() && next != '\0')
	{
		str.append(1, oiStream.get());		
	}
	
	oiStream.seekg(ipos, ios::beg);
	return str;
}

bool read_and_check_types(istream& oiStream, char &char_data)
{
	checkandthrowBasicException(istream);
	char_data = oiStream.peek();
}

bool read_and_check_types(istream& oiStream, int &int_data)
{
	checkandthrowBasicException(istream);
	//Assumption here is that the integer when writing is stored as null terminated string '1''0''0''\0'
	int ipos = oiStream.tellg();
	if(!(oiStream >> int_data))
	{
		oiStream.seekg(ipos, ios::beg);
		throw InvalidTypeException();
	}
	oiStream.seekg(ipos, ios::beg);
}

bool read_and_check_types(istream& oiStream, double &double_data)
{
	checkandthrowBasicException(istream);
	int ipos = oiStream.tellg();
	if(!(oiStream >> double_data))
	{
		oiStream.seekg(ipos, ios::beg);
		throw InvalidTypeException();
	}
	oiStream.seekg(ipos, ios::beg);
}

bool read_and_check_types(istream& oiStream, std::string &string_data)
{
	checkandthrowBasicException(istream);
	int ipos = oiStream.tellg();
	if(!(oiStream >> string_data))
	{
		oiStream.seekg(ipos, ios::beg);
		throw InvalidTypeException();
	}
	oiStream.seekg(ipos, ios::beg);
}