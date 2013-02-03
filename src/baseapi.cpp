/*
 * =====================================================================================
 *
 *       Filename:  baseapi.cpp
 *
 *    Description:  BaseAPI sources
 *
 *        Version:  1.0
 *        Created:  02/02/13 17:47:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

// Headers
#include "baseapi.h"

void BaseAPI::initJSON(Json::Value jValue) {
 jRoot = jValue;
}

Json::Value BaseAPI::get(const char *key) {
 if (jRoot.isMember(key)) return jRoot.get(key, jRoot);
 else return (Json::Value)false;
}

void BaseAPI::setResult(const char *msg) {
 result = (std::string)msg;
}
