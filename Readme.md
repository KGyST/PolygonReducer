# Polygon Reducer for ArchiCAD
This is an ArchiCAD add-on that decimates 2D polygons in a geometrically correct way, optimized for creating simplified door/windows profiles.
Currently supports ArchiCAD 27/SE 2024.

## Usage of this project
- if not got using `git clone --recurse-submodules https://github.com/KGyST/PolygonReducer.git` then update submodules:
`git submodule update --init --recursive`

## This project uses
- Windows SDK (Windows 10) 
- Archicad Api Devkit 27
- Boost
	- Version to be updated
- TODO

## TODOs
- Postprocess
	- Skipped segment connectings
	- Arc to polyline conversion
- Coding style mods/refactoring
	- One row between functions, two between classes
	- 2 char tabs
	- Pascal case function names
	- std::optional return values with errorcode pointers defaulted nullptr
		-	rather back to exception-throwing
	- AC19 removal

## (Later)
- Boost.Test
- S::Array<Type>::begin()

