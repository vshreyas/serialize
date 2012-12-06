/**
 * @file   exceptions.hpp
 * @author Raghdah
 * 
 * @brief Defines exception classes which may be thrown by the
 * StreamReader or StreamWriter objects.
 * 
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <iostream>
#include <string>
#include <sstream>

#include <exception>

using namespace std;

class StreamException: public exception
{
public:
   virtual const char* what() const throw(){
		return "Stream exception";
	} 
};

class EndOfFileException: public StreamException
{
public:
	virtual const char* what() const throw(){
		return "End of file reached.";
	}
}; 

class FailBitException: public StreamException
{
public:
	virtual const char* what() const throw(){
		return "Fail bit exception.";
	}
}; 

/**
 * Exception to be thrown when the size of a stored array does not
 * match size of the array trying to be read into.
 */
class SizeMismatchException: public exception
{
public:
  SizeMismatchException():
    m_expected_size{0}, m_given_size{0}
  {
  }
  
  SizeMismatchException(size_t expected_size, size_t given_size):
    m_expected_size{expected_size}, m_given_size{given_size}
  {
  }
    
  virtual const char* what() const throw(){
    if (m_given_size == m_expected_size)
      return "Size mismatch: Length of stored and receiving objects not equal.";
    
    stringstream ss (stringstream::in | stringstream::out);
    ss<<"Size mismatch!";
    ss<<"\nExpected length of object was ("<<m_expected_size;
    ss<<"), and given length is ("<<m_given_size<<").";

    return ss.str().c_str();
  }

  private:
  size_t m_expected_size;	/**< expected array size (stored size) */
  size_t m_given_size;		/**< given array size (during deserializing) */
};
#endif
