#include<iostream>
#include<vector>
using namespace std;
int main() {
    int n;
    cout << "start, input a num:" << endl;
    while (cin >> n) {
        cout << "input:" << n << endl;
        vector<int> gym(n);
        vector<int> work(n);
        for (int i = 0; i < n; i++) cin >> work[i];
        cout << "work" << endl;
        for (int i = 0; i < n; i++) cin >> gym[i];
        cout << "gym" << endl;
        int result = 0;

        // 处理逻辑

        cout << "result:" << result << endl;
        cout << "end, input a num:" << endl;
    }
    return 0;
}
// /usr/bin/g++ -fdiagnostics-color=always -g /root/project/dev/leetcode/leetcode-master/src/demo/acm.cpp -o /root/project/dev/leetcode/leetcode-master/src/demo/acm