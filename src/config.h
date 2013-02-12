/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  Configuration file
 *
 *        Version:  1.0
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

// System settings/limits
#define PORT			1337	// The port it listens for requests on
#define LISTENQ			100	// The amount of connections at once passed to listen()
#define BUFFER_LIMIT		1024	// The maximum size of a request
#define REQUEST_TIMEOUT_RECV	3	// The timeout when receiving a request (in seconds)
#define REQUEST_TIMEOUT_SEND	3	// The timeout when sending a request (in seconds)

// Version specific settings (do not change)
#define NAME		"tn2_core"
#define VERSION		"1.1b"
#define VERSION_TAG	"BETA"

// Switch settings (can be turned on/off)
// #define DEBUG // Debug setting, enable by compiling with -DDEBUG

#endif /* CONFIG_H */
