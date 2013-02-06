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

// User settings (change if you want to)
#define PORT		1337
#define NAME		"tn2_core"
#define VERSION		"1.1b"
#define VERSION_TAG	"BETA"

// System settings/limits (do not change if you don't know what you're doing)
#define LISTENQ			100
#define MAX_REQ_LINE		1024
#define REQUEST_TIMEOUT_RECV	3
#define REQUEST_TIMEOUT_SEND	3
#define MAX_CONNECTIONS		10000

// Switch settings (can be turned on/off)
// #define DEBUG // Debug setting, enable by compiling with -DDEBUG

#endif /* CONFIG_H */
