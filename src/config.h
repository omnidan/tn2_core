/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  Configuration file
 *
 *        Version:  0.2
 *        Created:  27/01/13 18:26:58
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

// User settings (change if you want to)
#define PORT		1337
#define REQUEST_TIMEOUT	5

// System settings/limits (do not change if you don't know what you're doing)
#define LISTENQ		100
#define MAX_REQ_LINE	1024
#define VERSION		"TNREQ v0.2"


// Switch settings (can be turned on/off)
// Debug setting, comment to disable debug messages, uncomment to enable.
#define DEBUG

#endif /* CONFIG_H */
