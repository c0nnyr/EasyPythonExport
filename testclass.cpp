#include <iostream>
#include "testclass.h"
#include "EasyPythonExport.hpp"
int testclass::test1(int a)
{
	std::cout<<"test1 class print "<<a<<std::endl;
    return 1231;
}
int testclass::test2()
{
    std::cout<<"test2 class print "<<std::endl;
    return 1232;
}
void testclass::test3(int a)
{
    std::cout<<"test3 class print "<<a<<std::endl;
}
void testclass::test4()
{
    std::cout<<"test4 class print "<<std::endl;
}
void testclass::test5(const int &a)
{
    std::cout<<"test5 class print "<<a<<std::endl;
}
void testclass::test6(const char* a)
{
    std::cout<<"test6 class print "<<a<<std::endl;
}
void testclass::test7(const std::string &a)
{
    std::cout<<"test7 class print "<<a<<std::endl;
}



//apply
BEGIN_EXPORT_PYTHON_CLASS(testmodule, testclass)
    ADD_METHOD(test1)
    ADD_METHOD(test2)
    ADD_METHOD(test3)
    ADD_METHOD(test4)
    ADD_METHOD(test5)
    ADD_METHOD(test6)
    ADD_METHOD(test7)
END_EXPORT_PYTHON_CLASS(testmodule, testclass)
