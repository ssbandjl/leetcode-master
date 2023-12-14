#include <iostream>
using namespace std;


// case 1
class Reverse {
public:
  	string rever(string s) {
      int n = s.size();
      for (int l = 0, r = n-1; l <= r; l++, r--) {
        if (s[l] != '-' && s[r] != '-') {
          swap(s[l], s[r]);
        }	
      }
    }
};

int main(){
	Reverse re;
  	string s = "ab-cd";
  	re.rever(s);
  	cout << s << endl;
    return 0;
}
