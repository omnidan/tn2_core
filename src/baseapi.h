/*
 * =====================================================================================
 *
 *       Filename:  baseapi.h
 *
 *    Description:  BaseAPI headers
 *
 *        Version:  1.1
 *        Created:  02/02/13 16:46:33
 *       Revision:  none
 *       Compiler:  g++
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
 std::string cPeer;
public:
 std::string init(Json::Value, char *);
 virtual std::string handle(void);
 Json::Value get(std::string);
 std::string peer(void);
};

#endif /* BASEAPI_H */
