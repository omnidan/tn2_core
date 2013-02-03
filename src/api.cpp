/*
 * =====================================================================================
 *
 *       Filename:  api.cpp
 *
 *    Description:  API sources
 *
 *        Version:  1.0
 *        Created:  01/02/13 20:32:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

// Headers
#include "api.h"

/* API: Constructor */
API::API(Json::Value jVal) {
 initJSON(jVal);
 if (get("test") != false) setResult("{'test': true}");
 else setResult("{}"); // Set result, the requesthandler will get this.
}
