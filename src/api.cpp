/*
 * =====================================================================================
 *
 *       Filename:  api.cpp
 *
 *    Description:  API sources
 *
 *        Version:  1.1
 *        Created:  01/02/13 20:32:51
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

// Headers
#include "api.h"

/* handle: Handle request */
std::string API::handle() {
 if (get("test") != false) return "{'test': true}";
 return "{}";
}
