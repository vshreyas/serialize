/**
 * @file   common.cpp
 * @author Shashwat Lal Das <shashwat@mashed-potato>
 * @date   Sun Nov 18 15:23:27 2012
 * 
 * @brief  Implementation of functions in common.hpp.
 * 
 */

#include "common.hpp"

#include <iostream>
#include <string>

using namespace std;

void NOT_IMPLEMENTED()
{
  cout<<"Call to a NOT IMPLEMENTED function!\n";
}

void NOT_IMPLEMENTED(string msg)
{
  cout<<"NOT IMPLEMENTED: "<<msg;
}
