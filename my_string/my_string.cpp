#include"my_string.h"


#include<iostream>
#include<string.h>


using namespace std;

my_string::my_string() {
    str_ = new char('\0');
    cout << "default construct my_string" << endl;

}

my_string::my_string(const char* str) {//深拷贝
    cout << "const char* construct my_string" << endl;
    int len = strlen(str);
    str_ = new char[len + 1];
    strcpy(str_, str);
    str_[len] = '\0';
}

my_string::my_string(const my_string& other) {//深拷贝
    cout << "const my_string construct my_string" << endl;
    int len = strlen(other.str_);
    str_ = new char[len + 1];
    strcpy(str_, other.str_);
    str_[len] = '\0';
}

my_string& my_string::operator=(const char* str) {//深拷贝
    delete[] str_;
    int len = strlen(str);
    str_ = new char[len + 1];
    strcpy(str_, str);
    str_[len] = '\0';
    return *this;
}

my_string& my_string::operator=(const my_string& other) {
    if (&other == this)
        return *this;
    int len = strlen(other.str_);
    str_ = new char[len + 1];
    strcpy(str_, other.str_);
    str_[len] = '\0';
    return *this;
}

char& my_string::operator[](unsigned int index) {
    return str_[index];
}
const char& my_string::operator[](unsigned int index)const {
    return str_[index];
}

istream& operator>>(istream& in, my_string& s) {
    char buffer[4096];
    in >> buffer;
}
ostream& operator<<(ostream& os, my_string& s);

my_string& operator+=(const my_string& other);

void display();

