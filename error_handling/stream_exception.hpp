#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <iostream>
#include <string>

#include <exception>

using namespace std;

class StreamException: public exception
{
   virtual const char* what() const throw();
};

class EndOfFileException: public StreamException
{
	virtual const char* what() const throw();
}; 

class FailBitException: public StreamException
{
	virtual const char* what() const throw();
}; 

class InvalidTypeException: public StreamException
{
	virtual const char* what() const throw();
}; 

#endif