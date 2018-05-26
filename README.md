# EasyPythonExport
Provide an easy way to export logic code to python script

I have worked with python and C++ for three years, and one regular operation is
to export logic code writen in C++ to Python which is invoked in script. The 
frameworks for exporting C++ to python I was using may be boostpython or something
writen by someone in the same company. Boostpython provides a C++ version of python
which is a little complicated and takes time to learn. The other kind of framework
writen by one of our own provides an easy way to export C++, but still makes the logic
code dirty, which means we can still smell some python in the logic.

My purpose is to provide gentle way and easy way to export C++ to python, which means
as little as code to writen, and decoupling logic and export.

With EasyPythonExport, you can write such code to export C++ Python


//main.cpp
#include "EasyPythonExport.hpp"
// excute this in some function to export class to module
// this will not change anymore
INIT_EXPORT_PYTHON(module, class);



#include "EasyPythonExport.hpp"
// some other file
// export define. export test1-7 functions
BEGIN_EXPORT_PYTHON_CLASS(module, class)
	ADD_METHOD(test1)
	ADD_METHOD(test2)
	ADD_METHOD(test3)
	ADD_METHOD(test4)
	ADD_METHOD(test5)
	ADD_METHOD(test6)
	ADD_METHOD(test7)// and new functions test7
END_EXPORT_PYTHON_CLASS(module, class)


