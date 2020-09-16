#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
	int cmd;

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cout << "Please Enter Command (1: Generate Key, 2: Encrypt by Key, 3: Decrypt by Key) : ";
	cin >> cmd;

	if (cmd == 1) {
		cout << "Generate Key...";

		int key_table[26];

		for (int i = 0; i < 26; i++) {
			key_table[i] = i;
		}

		srand((unsigned int)time(NULL));
		for (int i = 0; i < 50; i++) {
			int a = rand() % 25;
			int b = rand() % 25;

			if (a != b) {
				swap(key_table[a], key_table[b]);
			}
		}

		ofstream os;
		os.open("key.txt");

		for (int i = 0; i < 26; i++) {
			os << key_table[i] << "\n";
		}

		os.close();

		cout << "done.\n";
	}
	else if (cmd == 2) {
		cout << "Encrypt by key...";

		ifstream is1, is2;
		ofstream os;
		is1.open("key.txt");
		is2.open("planetext.txt");
		os.open("cypertext.txt");

		int key_table[26];
		for (int i = 0; i < 26; i++) {
			is1 >> key_table[i];
		}
		is1.close();

		while (!is2.eof()) {
			string str1, str2 = "";
			getline(is2, str1);

			for (int i = 0; i < str1.length(); i++) {
				if (str1[i] >= 'A' && str1[i] <= 'Z') {
					str2 += ('a' + key_table[str1[i] - 'A']);
				}
				else if (str1[i] >= 'a' && str1[i] <= 'z') {
					str2 += ('a' + key_table[str1[i] - 'a']);
				}
				else {
					str2 += str1[i];
				}
			}

			os << str2 << "\n";
		}

		is2.close();
		os.close();

		cout << "done.\n";
	}
	else if (cmd == 3) {
		cout << "Decrypt by key...";

		ifstream is1, is2;
		ofstream os;
		is1.open("key.txt");
		is2.open("cypertext.txt");
		os.open("planetext_decrypted.txt");

		int key_table[26];
		for (int i = 0; i < 26; i++) {
			int temp;
			is1 >> temp;
			key_table[temp] = i;
		}
		is1.close();

		while (!is2.eof()) {
			string str1, str2 = "";
			getline(is2, str1);

			for (int i = 0; i < str1.length(); i++) {
				if (str1[i] >= 'a' && str1[i] <= 'z') {
					str2 += ('a' + key_table[str1[i] - 'a']);
				}
				else {
					str2 += str1[i];
				}
			}

			os << str2 << "\n";
		}

		is2.close();
		os.close();

		cout << "done.\n";
	}
	else {
		cout << "Wrong Command.\n";
	}

	return 0;
}