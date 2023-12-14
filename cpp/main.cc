#include <iostream>
using namespace std;


// case 1
class Reverse {
public:
  	void rever(string &s) {
      int n = s.size();
      int l = 0, r = n-1;
      while (l <= r) {
        if (s[l] != '-' && s[r] != '-' && s[l] != '!' && s[r] != '!') {
          swap(s[l], s[r]);
        }
        if (s[l] == '!' && s[r] != '!') {
            l++;
        } else if (s[r] == '!' && s[l] != '!') {
            r--;
        } else {
            l++;
            r--;
        }
      }
    //   cout << s << endl;
    }
};

int main(){
	Reverse re;
  	// string in = "ab-cd";
  	string in = "a-bC-dEf-ghIj";
  	// string in = "Test1ng-Leet=code-Q!";
  	cout << in << endl;
  	re.rever(in);
  	cout << in << endl;
    return 0;
}
