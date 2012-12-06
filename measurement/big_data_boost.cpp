/**
 * @brief Measurements using Boost on big arrays and vectors.
 * Arch Linux, kernel version: 3.6.8
 * Intel Core i5 3210M 2.5 GHz, 6 GB RAM
 * Compiler flags: g++ --std=c++11 -lboost_serialization
 */

/*
big_int_array: array of 100000 integers, each between -1000000 and 1000000
722 kb input size
big_string_vector: vector of 52880 strings (from /usr/share/dict/cracklib-small)
622 kb input size
(loaded at compile time)

executable file size: 6166891 bytes
output file:
1336053 bytes size

time taken for execution: 0.248 s
lines of user code required: 9 total
*/

#include "big_int_array.hpp"
#include "big_string_vector.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <fstream>
#include <iostream>

using namespace std;

int main()
{
  ofstream os("bigfile_boost");
  boost::archive::text_oarchive w(os);

  w<<big_int_array;
  w<<big_string_vector;
  
  os.close();
  
  int big_array_read[100000];
  vector<string> big_vector_read;
  ifstream is("bigfile_boost");
  boost::archive::text_iarchive r(is);
  r>>big_array_read;
  r>>big_vector_read;

  is.close();
  
  return 0;
}
