/*
 * =====================================================================================
 *
 *       Filename:  api.cpp
 *
 *    Description:  API sources
 *
 *        Version:  0.1
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
API::API(Json::Value jRoot) {
 result = "{}"; // Set result, the requesthandler will get this.
}
