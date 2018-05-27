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
    const char* test6(const char*);
    void test7(const std::string &);
    std::string test8();
    bool test9(bool);
    double test10(float);
    static int test1_static(int);

	int GetProperty1();
	int GetProperty2();
	void SetProperty2(int a);
    
    static int GetProperty3();
    static void SetProperty3(int a);
};
#endif
