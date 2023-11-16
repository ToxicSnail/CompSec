#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class String 
{
    char* str;
    int len;
public:
    String(int l = 0) 
    {
        str = new char[l + 1]; 
        len = l; 
        str[0] = '\0';
    };

    String(const char* a) 
    {
        str = new char[strlen(a) + 1];
        strcpy(str, a);
        len = strlen(a);
    };

    String(String& a) {
        str = new char[a.len + 1];
        len = a.len;
        strcpy(str, a.str);
    };

    ~String() 
    {
        if (str) delete[]str;
        str = NULL;
    }

    int Len();
    String& operator=(const String&);
    bool operator==(String&);
    bool operator!=(String&);
    char& operator[](int);
    String operator+(const String&);
    String& operator+=(const String&);
    friend istream& operator>>(istream&, String&);
    friend ostream& operator<<(ostream&, String&);
    int BMH_search(String&);
};

int String::Len() 
{
    return len;
}

String& String::operator=(const String& a) 
{
    if (this != &a) {
        delete[] str;
        str = new char[a.len + 1];
        strcpy(str, a.str);
        len = a.len;
    }
    return *this;
}

bool String::operator==(String& a) 
{
    if (strcmp(str, a.str)) return false;
    return true;
}

bool String::operator!=(String& a) 
{
    if (strcmp(str, a.str)) return true;
    return false;
}

char& String::operator[](int a) {
    if (a < 0 || a >= len) exit(1);
    return str[a];
}

String String::operator+(const String& a) 
{
    String z(len + a.len + 1);
    z.len = len + a.len;
    strcpy(z.str, str);
    strcat(z.str, a.str);
    return z;
}

String& String::operator+=(const String& a) 
{
    char* t = new char[len + 1];
    strcpy(t, str);
    delete[] str;
    len = len + a.len;
    str = new char[len + 1];
    strcpy(str, t);
    strcat(str, a.str);
    delete[] t;
    return *this;
}

istream& operator>>(istream& in, String& a) 
{
    cout<<"(cin)Set len, then write str."<<endl; 
    in >> a.len; 
 
    delete[] a.str; 
     
    a.str = new char[500]; 
    for(int i = 0; i < a.len; i++) 
    { 
        in >> a.str[i]; 
    } 
    return in;  

    // in >> a.str;
    // a.len = strlen(a.str);
    // return in;
}

ostream& operator<<(ostream& out, String& str) 
{
    for(int i = 0; i<str.len; i++) 
        out << str.str[i]; 
    return out; 

}

int String::BMH_search(String& a) 
{//БМ поиска
    int table[256];
    fill_n(table, 256, a.len);// len obraz
    for (int i = 0; i < a.len - 1; i++) 
    { //tablicha char
        table[a.str[i]] = a.len - 1 - i;
    }
    
    int i = a.len - 1;
    int j = a.len - 1;
    while (i < len && j >= 0) 
    {
        int k = i;
        j = a.len - 1;
        while (j >= 0) 
        {
            if (str[k] != a.str[j]) 
            {
                i = i + table[str[i]];
                j = a.len - 1;
                break;
            }
            else
            {
                j--;
                k--;
            }
        }
    }
    if (j >= 0)
        return -1;
    else 
        return i + 1 - a.len;
}


int main() {
    String a, b("Hello");
    String c("World");
    a = b + c;
    cout << a << endl;
    a += c;
    b = "World";
    if (c == b) 
        cout << c << " (equal) " << b << endl;
    b = a;
    if (c != b) 
        cout << c << " (not equal) " << b << "\n\n";


    a = ("aabbc");
    b = ("abb");
    int first_entry = a.BMH_search(b);
    cout << "string1=" << a << "\nstring2=" << b << endl << "char_start_string2=" << first_entry << endl;
    cout << "\n";
    
    return 0;
}

// https://www.programiz.com/cpp-programming/online-compiler/ 
