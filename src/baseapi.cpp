/*
 * =====================================================================================
 *
 *       Filename:  baseapi.cpp
 *
 *    Description:  BaseAPI sources
 *
 *        Version:  1.1
 *        Created:  02/02/13 17:47:34
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

// Headers
#include "baseapi.h"

/* init: Initialise the API, called by RequestHandler */
std::string BaseAPI::init(Json::Value jVal, char *cip) {
 jRoot = jVal;
 cPeer = (std::string)cip;
 return handle();
}

/* handle: Prototype for API */
std::string BaseAPI::handle() {
 return "{\"type\": \"error\", \"msg\": \"Invalid API.\"}";
}

/* get: Get a value from the request */
Json::Value BaseAPI::get(std::string key) {
 if (jRoot.isMember(key)) return jRoot.get(key, jRoot);
 else return (Json::Value)false;
}

/* peer: Get the IP address of the peer */
std::string BaseAPI::peer() {
 return cPeer;
}
