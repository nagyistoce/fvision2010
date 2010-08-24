cvdraw
======

:author ferryzhou@gmail.com
:version 0.1.0

Overview
--------

cvdraw provides a set of drawer classes and functions for drawing.
the draw classes separate drawing configuration (say line type, thickness) and drawing entities (say points or lines)

Requirements
------------

- opencv 1.0
- cvutils 0.1
- Microsoft Visual Studio 2005 or newer


ShowCases
---------

points:
	PointDrawer pd("o", fvision::Colors::RED, 5)
	pd.draw(image, cvPoint(10, 20));




