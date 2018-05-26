#ifndef TEST_CLASS_H
#define TEST_CLASS_H
#include <string>
class testclass
{
public:
    int test1(int);
    int test2();
    void test3(int);
    void test4();
    //int& should not be used
    void test5(const int&);
    void test6(const char*);
    void test7(const std::string &);
};
#endif
