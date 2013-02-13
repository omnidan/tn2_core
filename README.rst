===================
TouchNet 2.X - Core
===================

Copyright (c) 2013 Daniel Bugl, TouchLay. All rights reserved.

TouchNet 2 is a networking framework written in C/C++ which implements modules coded in interpreted languages. The modules can be dynamically loaded and managed.

INFO: Use the 1.0 branch for now: https://github.com/TouchLay/tn2_core/tree/1.0 - threads in 1.1 are still a bit unstable.

.. contents::


Description
-----------

This is the core for the TouchNet. An API can be added in src/api.cpp. The API handles the requests and gives the results.

Branches
--------

master
~~~~~~

The master branch contains the latest stable version of the TouchNet 2.X Core including the specific TN2 API.

core
~~~~

The core branch is like the core of the core, also the latest stable version, and excludes the specific TN2 API.

testing
~~~~~~~

This is the experimental branch for tn2_core. The threads branch has been deleted because I started working on threads instead of subprocesses again in testing.

forklimit
~~~~~~~~~

This was an experiment to implement limits for fork()

1.0
~~~

Version 1.0 release of tn2_core.


1.0 vs 1.1
----------

1.1 uses threads instead of forks, which makes the core much lighter. fork() is very resource heavy and slower than threads.
1.1 is about twice as fast (on a local server, without latency) because it uses threads instead of forks. TN is also slower on 1.0, because 1.0 was designed for HTTP and later, TN was added. In 1.1, the RequestHandler and Socket classes were completely rewritten to have more performance for both protocols. The send speed is about the same on both versions, because the same benchmark was used. The receive speed includes the receiving on the server, the reading from the socket, the parsing of the json request, the handling of the request by the API and finally, the receiving time from the server to the client. For all that, it is a quite short time.
On the remote server, the huge delay comes from the distance between me and the server, of course. Especially over distance, the TN protocol wins, because it uses far less data. The difference between 1.1 and 1.0 is not so big over large distances, but 1.0 also uses a lot more cpu power (since it uses forks instead of threads).
Here are the results of the benchmarks:

+-----------+---------------+---------------+---------------+---------------+
|           | v1.0 (Local)  | v1.0 (Remote) | v1.1 (Local)  | v1.1 (Remote) |
+===========+===============+===============+===============+===============+
| HTTP (TX) | 0.175ms       | 0.185ms       | 0.180ms       | 0.182ms       |
+-----------+---------------+---------------+---------------+---------------+
| TN   (TX) | 0.069ms       | 0.065ms       | 0.052ms       | 0.062ms       |
+-----------+---------------+---------------+---------------+---------------+
| HTTP (RX) | 0.984ms       | 23.243ms      | 0.591ms       | 21.153ms      |
+-----------+---------------+---------------+---------------+---------------+
| TN   (RX) | 0.949ms       | 14.579ms      | 0.302ms       | 13.390ms      |
+-----------+---------------+---------------+---------------+---------------+
| Total     | 2.177ms       | 38.072ms      | 1.125ms       | 34.787ms      |
+-----------+---------------+---------------+---------------+---------------+

License
-------

The tn2_core (TouchNet 2.X Core) source is licensed under BSD 4-clause license. For further information please consult the LICENSE file.
complcache_start_auto_complete)OB
