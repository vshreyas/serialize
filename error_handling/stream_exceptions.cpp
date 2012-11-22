#include "stream_exception.hpp"
#include <exception>
using namespace std;

const char* StreamException::what() const throw()
{
    return "Stream exception";
} 

virtual const char* EndOfFileException::what() const throw()
{
	return "End of file reached.";
}

virtual const char* FailBitException::what() const throw()
{
	return "Fail bit exception.";
}

virtual const char* InvalidTypeException::what() const throw()
{
	return "Invalid type exception.";
}

