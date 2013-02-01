/*
 * =====================================================================================
 *
 *       Filename:  api.h
 *
 *    Description:  API headers
 *
 *        Version:  0.1
 *        Created:  01/02/13 20:30:49
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#ifndef API_H
#define	API_H

#include "config.h"

// Standard headers
#include <iostream>

// Internal headers
#include "json/json.h"

class API {
public:
 API(Json::Value);
 std::string result;
};

#endif /* API_H */
