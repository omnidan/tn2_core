/*
 * =====================================================================================
 *
 *       Filename:  api.h
 *
 *    Description:  API headers
 *
 *        Version:  1.0
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

// Internal headers
#include "config.h"
#include "baseapi.h"

class API : public BaseAPI {
public:
 API(Json::Value);
};

#endif /* API_H */
