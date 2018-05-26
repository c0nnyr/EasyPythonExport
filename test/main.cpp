//
//  main.cpp
//  testXcode
//
//  Created by conny on 2018/5/19.
//  Copyright © 2018年 conny. All rights reserved.
//

#include <iostream>
#include "EasyPythonExport.hpp"

//test python str
const char *pystr = "\
import testmodule;\
a=testmodule.testclass();\
print 'ok';\
print a.test1(1);\
print a.test2();\
print a.test3(3);\
print a.test4();\
print a.test5(5);\
print a.test6('hello test6');\
print a.test7('hello test7');\
";

int main(int argc, const char * argv[])
{
    std::cout<<"hello world"<<std::endl;
    Py_Initialize();
    PyRun_SimpleString("import sys;sys.path.append('./')");
    INIT_EXPORT_PYTHON(testmodule, testclass);
    PyRun_SimpleString(pystr);
    Py_Finalize();
    return 0;
}

