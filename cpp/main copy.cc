#include <iostream>
using namespace std;

// To execute C++, please define "int main()"

// The TestCase is shown below
// Input : 1 2
// Output : 3

int main() {
	// string words = "01101";
	string words = "11";
	// cout << words << endl;
	int len = words.size();
	int h = 0;
    int t = len-1;
	int swap = 0;
	while(h < t) {
		if (words[h] == '1') {
			if (words[t] == '0') {
				//words[h] = '1';
				//words[t] = '0';
				swap++;
                // cout << swap <<endl;
                h++;
                t--;
			} else { // tail = 1
				t--;
			}
		} else { // head = 0
			h++;
		}
	}
    cout << swap <<endl;
	return swap;
  	return 0;
}

// g++ Main.cc -o Main -O2 -fno-asm -Wall -lm -static