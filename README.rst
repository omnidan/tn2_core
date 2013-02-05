===================
TouchNet 2.X - Core
===================

Copyright (c) 2013 Daniel Bugl, TouchLay. All rights reserved.

TouchNet 2 is a networking framework written in C/C++ which implements modules coded in interpreted languages. The modules can be dynamically loaded and managed.

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


License
-------

The tn2_core (TouchNet 2.X Core) source is licensed under BSD 4-clause license. For further information please consult the LICENSE file.
