/*
 * =====================================================================================
 *
 *       Filename:  baseapi.h
 *
 *    Description:  BaseAPI headers
 *
 *        Version:  0.1
 *        Created:  02/02/13 16:46:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#ifndef BASEAPI_H
#define BASEAPI_H

// Standard headers
#include <iostream>

// Internal headers
#include "json/json.h"

class BaseAPI {
private:
 Json::Value jRoot;
public:
 void initJSON(Json::Value);
 Json::Value get(const char *);
 void setResult(const char *);
 std::string result;
};

#endif /* BASEAPI_H */
