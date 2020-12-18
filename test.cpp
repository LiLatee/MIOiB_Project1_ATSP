#include <iostream>

using namespace std;

int main()
{
    int * a = new int[3];
    a[0] = 0;
    a[1] = 2;
    a[2] = 1;
    int * b =new int[3];
    cout << &b << endl;
    copy(a, a+9, b);
    cout << &a << endl;
    cout << &b << endl;
    delete[] a;
    // cout << a[0] << endl;
    // cout << &a[0] << endl;
    // cout << a[8] << endl;
    // cout << &a[8] << endl;
    cout << b << endl;
    cout << &b << endl;
    cout << b[2] << endl;
    cout << &b[2] << endl;

    return 0;
}