#ifndef _MYSTRING_H_
#define _MYSTRING_H_

#include<iostream>

using namespace std;

class my_string {

public:
    my_string();
    my_string(const char* str);
    my_string(const my_string& other);
    ~my_string();

    my_string& operator=(const my_string& other);
    my_string& operator=(const char* str);

    char& operator[](unsigned int index);
    const char& operator[](unsigned int index)const;

    friend istream& operator>>(istream& in, my_string& s);
    friend ostream& operator<<(ostream& os, my_string& s);

    my_string& operator+=(const my_string& other);

    void display();

private:
    char* str_;
};



#endif // !_MYSTRING_H_
