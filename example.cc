请使用标准输入输出(stdin，stdout) ；请把所有程序写在一个文件里
勿使用图形、文件、网络、系统相关的操作，如sys/stat.h , unistd.h , curl/curl.h , process.h
编译命令：g++ Main.cc -o Main -O2 -fno-asm -Wall -lm -static
1
#include <iostream>
2
using namespace std;
3
​
4
// To execute C++, please define "int main()"
5
​
6
// The TestCase is shown below
7
// Input : 1 2
8
// Output : 3
9
​
10
int main() {
11
    string words = "Hello, World!";
12
    cout << words << endl;
13
    int a, b;
14
    while(cin>> a >> b)
15
    cout << "Your result is : "<< a + b << endl;
16
    return 0;
17
}