memdb
=====

Overview
--------

memdb provides in-memory ad-hoc database apis for two particular table schemas:

1. <id>, <object>
2. <frame_id>, <object_id>, <object>

This package is mainly used in point tracking and structure from motion to 
manage a large set of points/cameras.


Requirements
------------

- Microsoft Visual Studio 2005 or newer (only for development)


Complie
-------

Since all sources are written in C++ templates, no complilation is required. 
However, if you want to compile samples or tests, you can follow the instructions 
as below.

### Visual Studio 2005

visual stuido 2005 project files can be found in _make/vs2005.


Tutorial
--------


Authors
-------

- Jin Zhou ferryzhou@gmail.com
