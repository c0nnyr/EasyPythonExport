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
const char* testclass::test6(const char* a)
{
    std::cout<<"test6 class print "<<a<<std::endl;
    return "from test6";
}
void testclass::test7(const std::string &a)
{
    std::cout<<"test7 class print "<<a<<std::endl;
}
std::string testclass::test8()
{
    std::cout<<"test8 class print "<<std::endl;
    return "from test 8";
}
bool testclass::test9(bool a)
{
    std::cout<<"test9 class print "<<a<<std::endl;
    return true;
}
double testclass::test10(float a)
{
    std::cout<<"test10 class print "<<a<<std::endl;
    return 123.123;
}
int testclass::test1_static(int a)
{
    std::cout<<"test1_static class print "<<a<<std::endl;
    return 999;
}
int testclass::GetProperty1()
{
    std::cout<<"GetProperty1 class print "<<std::endl;
    return 987;
}
int testclass::GetProperty2()
{
    std::cout<<"GetProperty2 class print "<<std::endl;
    return 986;
}
void testclass::SetProperty2(int a)
{
    std::cout<<"SetProperty2 class print "<<a<<std::endl;
}
int testclass::GetProperty3()
{
    std::cout<<"GetProperty3 static print "<<std::endl;
    return 1986;
}
void testclass::SetProperty3(int a)
{
    std::cout<<"SetProperty3 static class print "<<a<<std::endl;
}


//apply
BEGIN_DEFINE_EXPORT_PYTHON_CLASS(testmodule, testclass)
    ADD_METHOD(test1)
    ADD_METHOD(test2)
    ADD_METHOD(test3)
    ADD_METHOD(test4)
    ADD_METHOD(test5)
    ADD_METHOD(test6)
    ADD_METHOD(test7)
    ADD_METHOD(test8)
    ADD_METHOD(test9)
    ADD_METHOD(test10)
    ADD_METHOD_STATIC(test1_static)
    ADD_PROPERTY_RO(Property1)
    ADD_PROPERTY_RW(Property2)
    ADD_PROPERTY_RW(Property3)
END_DEFINE_EXPORT_PYTHON_CLASS()
