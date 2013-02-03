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

threads
~~~~~~~

This was an experiment to implement pthreads. Since it didn't make sense to use pthreads over fork() in this case, though, it was made a seperate branch and stopped being developed on.

License
-------

The tn2_core (TouchNet 2.X Core) source is licensed under BSD 4-clause license.
