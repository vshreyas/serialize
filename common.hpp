/**
 * @file   common.hpp
 * @author Shashwat Lal Das <shashwat@mashed-potato>
 * @date   Sun Nov 18 15:22:11 2012
 * 
 * @brief Contains common constants and functions to be used by the
 * rest of the library. If this file becomes too large, it may be
 * split later.
 * 
 */

#include "types.hpp"
#include <string>

/** 
 * Call within functions which will be implemented later, or functions
 * which are not expected to be called, instead of the usual empty
 * definition.
 *
 */
void NOT_IMPLEMENTED();

/** 
 * See `NOT_IMPLEMENTED()`. Displays a user-defined message in
 * addition.
 *
 * @param msg message
 * \sa NOT_IMPLEMENTED()
 */
void NOT_IMPLEMENTED(std::string msg);
