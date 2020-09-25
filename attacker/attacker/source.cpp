#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <sstream>
#include <map>

using namespace std;


const int ALPHABET_SIZE = 26; //���ĺ� ����
const double ALPHABET_FREQUENCY[ALPHABET_SIZE] = { //���� ���ĺ� �� �� ��� (%)
	8.167,
	1.492,
	2.782,
	4.253,
	12.702,
	2.228,
	2.015,
	6.094,
	6.966,
	0.153,
	0.772,
	4.025,
	2.406,
	6.749,
	7.507,
	1.929,
	0.095,
	5.987,
	6.327,
	9.056,
	2.758,
	0.978,
	2.361,
	0.150,
	1.974,
	0.074
};



//���ڰ� �빮�� A~Z����
bool isCaptital(char c) {
	if (c >= 'A' && c <= 'Z') {
		return true;
	}
	else return false;
}

// indexToChar(0, true) -> 'A'
char indexToChar(int index, bool isCapital) {
	return isCapital ? (char)(index + 65) : (char)(index + 97);
}

//charToIndex('A') -> 0, charToIndex('a') -> 0
int charToIndex(char c) {
	return toupper(c) % 65;
}


//ã�� int�� ������ -1, ������ �ش� index ��ȯ
int find(int* arr, int arrSize, int intToFind) {
	for (int i = 0; i < arrSize; i++) {
		if (arr[i] == intToFind) {
			return i;
		}
	}

	return -1;
}

void printProbability(double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	char c = 'a';
	cout << endl << "       ";
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		printf("%2d %c    ", i, c++);
	}
	cout << endl << endl;

	c = 'A';
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		printf("%2d %c  ", i, c++);
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			printf("%6.2f  ", prob[i][j]);
		}
		cout << endl << endl;
	}
}

void openEncryptedFile(string fileName, string* encryptedText) {
	ifstream openEncryptedFile(fileName.data());
	if (openEncryptedFile.is_open()) {
		string line;
		while (getline(openEncryptedFile, line)) {
			//cout << line << endl;
			*encryptedText += line + "\n";
		}
		openEncryptedFile.close();
	}
}

void stringToUpper(string* str) {
	for (int i = 0; i < (*str).size(); i++) {
		(*str)[i] = toupper((*str)[i]);
	}
}

//���� �� ����
void countAlphabet(string str, int freq[ALPHABET_SIZE], int* total, string filePathToWrite) {
	for (int i = 0; i < str.size(); i++) {
		if (isCaptital(str[i])) {
			freq[charToIndex(str[i])]++;
			(*total)++;
		}
	}

	//�� �� txt ���
	ofstream writeFrequencyFile(filePathToWrite.data());
	if (writeFrequencyFile.is_open()) {
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			writeFrequencyFile << freq[i] << endl;
		}
	}
	writeFrequencyFile.close();
}

void calculateSimpleProbability(int freq[ALPHABET_SIZE], double prob[ALPHABET_SIZE][ALPHABET_SIZE], int total) {
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		double percent = (double)freq[i] / (double)total * 100.0; //ex) 11.600%
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			prob[i][j] = 100 - abs(ALPHABET_FREQUENCY[j] - percent); //ex) 98.898%
		}
	}
}

//ex) ��ȣȭ�� K�� ������ � ���ĺ����� ���� ���� Ȯ���� ���ĺ����� ����
int findHighestProbabilityPlainAlphabetOfEncryptedAlphabet(int j, double prob[ALPHABET_SIZE][ALPHABET_SIZE], int keys[ALPHABET_SIZE]) {
	int maxIndex = 0; //ex) ���� a�� Ȯ���� ���� ���� ��ȣ ���ĺ�
	double maxProbability = 0.0; //ex) �� ��ȣ���� a�� Ȯ�� �߿� ���� ���� Ȯ�� ��

	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (maxProbability < prob[i][j]) {
			maxIndex = i;
			maxProbability = prob[i][j];
		}
	}

	int found = find(keys, ALPHABET_SIZE, maxIndex);
	if (found == -1) {
		keys[j] = maxIndex;
	}
	else {
		if (prob[maxIndex][found] < prob[maxIndex][j]) {
			keys[j] = maxIndex;
			prob[maxIndex][found] = 0.0;
			findHighestProbabilityPlainAlphabetOfEncryptedAlphabet(found, prob, keys);
		}
		else {
			prob[maxIndex][j] = 0.0;
			findHighestProbabilityPlainAlphabetOfEncryptedAlphabet(j, prob, keys);
		}
	}
	

	return maxIndex;
}

//java�� split�� ������ ���� �ϴ� �Լ�
//��ó: https://twinw.tistory.com/220 [����ǲ�]
vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str);
	string temp;

	while (getline(ss, temp, delimiter)) {
		internal.push_back(temp);
	}

	return internal;
}

//���ڿ����� Ư�� ���ڵ��� �ٸ� ���ڷ� ġȯ
//��ó: https://wendys.tistory.com/8 [������ �⹦�� �̾߱�]
string replace_all(__in const std::string &message, __in const std::string &pattern, __in const std::string &replace) {
	std::string result = message;
	std::string::size_type pos = 0;
	while ((pos = result.find(pattern)) != std::string::npos)
	{
		result.replace(pos, pattern.size(), replace);
	}
	return result;
}

string findHighestCountStringInVector(map<string, int> m) {
	string highestStr = m.begin()->first;
	int highestVal = m.begin()->second;
	
	for (auto i = m.begin(); i != m.end(); i++) {
		if (highestVal < i->second) {
			highestStr = i->first;
			highestVal = i->second;
		}
	}

	return highestStr;
}

//A�� ��ȣ���̰� �빮��, a�� ���̰� �ҹ��� (������), weight�� ����ġ (0 ~ 100 ���� ����, 100�� ��� 100%, 0�� ��� Ȯ�� �״��)
void increaseProbability(char A, char a, int weight, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	int indexOfA = charToIndex(A);
	int indexOfa = charToIndex(a);

	prob[indexOfA][indexOfa] += (100.0 - prob[indexOfA][indexOfa]) / 100.0 * weight;
}

//A�� ��ȣ���̰� �빮��, a�� ���̰� �ҹ��� (������), weight�� ����ġ (0 ~ 100 ���� ����, 100�� ��� 0%, 0�� ��� Ȯ�� �״��)
void decreaseProbability(char A, char a, int weight, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	int indexOfA = charToIndex(A);
	int indexOfa = charToIndex(a);

	prob[indexOfA][indexOfa] -= prob[indexOfA][indexOfa] / 100.0 * weight;
}

void copyProbabilityTable(double newProb[ALPHABET_SIZE][ALPHABET_SIZE], double originProb[ALPHABET_SIZE][ALPHABET_SIZE]) {
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			newProb[i][j] = originProb[i][j];
		}
	}
}

char findHighestProbabilityCharacter(char c, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	double highest = 0.0;
	int highestRow = 0;
	char highestChar;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (highest < prob[i][charToIndex(c)]) {
			highest = prob[i][charToIndex(c)];
			highestChar = indexToChar(i, true);
			highestRow = i;
		}
	}

	return highestChar;
}

char findResonableCharacterRecursiver(char c, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	double highest = 0.0;
	int highestRow = 0;
	char highestChar;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (highest < prob[i][charToIndex(c)]) {
			highest = prob[i][charToIndex(c)];
			highestChar = indexToChar(i, true);
			highestRow = i;
		}
	}

	if (highest == 0.0) {
		return '\0';
	}

	for (int j = 0; j < ALPHABET_SIZE; j++) {
		if (highest < prob[highestRow][j]) {
			prob[highestRow][charToIndex(c)] = 0.0;
			return findResonableCharacterRecursiver(c, prob);
		}
	}

	return highestChar;
}

//ex) ���� ���� c ���ɼ��� ���ٰ� �����Ǵ� ���� �ո����� ���ڸ� ã���ش�
char findResonableCharacter(char c, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	double newProb[ALPHABET_SIZE][ALPHABET_SIZE];
	copyProbabilityTable(newProb, prob);

	char ch = findResonableCharacterRecursiver(c, newProb);
	return (ch != '\0') ? ch : findHighestProbabilityCharacter(c, prob);
}

void improveProbabilityBy_the(string enctxt, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	map<string, int> threeStringCount;

	replace_all(enctxt, "\n", " "); //���͸� ����� ����

	//���ڿ����� ���� ���� ������ 3�ڸ� �ܾ ã�´� -> the�� ���ɼ��� ���� -> ���� t, h, e�� Ȯ���� ������Ų��
	vector<string> splitedText = split(enctxt, ' ');
	for (int i = 0; i < splitedText.size(); i++) {
		if (splitedText[i].size() == 3) {
			threeStringCount[splitedText[i]]++;
		}
	}

	if (threeStringCount.size() > 0) {
		string maybe_the = findHighestCountStringInVector(threeStringCount); //�Ƹ� the�� ������ �����Ǵ� string
		char maybe_e = findResonableCharacter('e', prob);

		if (maybe_e == maybe_the[2]) { //ex) ���� ���� ���� ���ڰ� K�ε� ���� ���� ���� 3���� �ܾ��� �������� K�̸� -> e ���� Ȯ�ǽ�, ���� �� ���ڵ� t, h Ȯ�ǽ�
			increaseProbability(maybe_the[0], 't', 90, prob);
			increaseProbability(maybe_the[1], 'h', 90, prob);
			increaseProbability(maybe_the[2], 'e', 90, prob);
		}
	}
}

void improveProbabilityBy_i_and_a(string enctxt, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	map<string, int> singleStringCount;

	replace_all(enctxt, "\n", " "); //���͸� ����� ����

	vector<string> splitedText = split(enctxt, ' ');
	for (int i = 0; i < splitedText.size(); i++) {
		if (splitedText[i].size() == 1) {
			singleStringCount[splitedText[i]]++;
		}
	}

	if (singleStringCount.size() > 0) {
		//���ڿ����� ���� ���� ������ 1�ڸ� �ܾ ã�´� -> a, i�� ���ɼ� �����ش�
		string most1 = findHighestCountStringInVector(singleStringCount);
		if (prob[charToIndex(most1[0])]['a'] > prob[charToIndex(most1[0])]['i']) {
			increaseProbability(most1[0], 'a', 90, prob);

			singleStringCount[most1] = -1;
			string most2 = findHighestCountStringInVector(singleStringCount);
			increaseProbability(most2[0], 'i', 90, prob);
		}
		else {
			increaseProbability(most1[0], 'i', 90, prob);

			singleStringCount[most1] = -1;
			string most2 = findHighestCountStringInVector(singleStringCount);
			increaseProbability(most2[0], 'a', 90, prob);
		}
	}
}

void improveProbabilityBy_on_of_or_ok(string enctxt, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	map<string, int> twoStringCount;

	replace_all(enctxt, "\n", " "); //���͸� ����� ����

	vector<string> splitedText = split(enctxt, ' ');
	char maybe_o = findResonableCharacter('o', prob);
	for (int i = 0; i < splitedText.size(); i++) {
		if (splitedText[i].size() == 2 && splitedText[i][0] == maybe_o) { //o ������ ���� ���� n, f, r, k�� ���ɼ� ����
			char followingChar = splitedText[i][1];
			increaseProbability(followingChar, 'n', 50, prob);
			increaseProbability(followingChar, 'f', 60, prob);
			increaseProbability(followingChar, 'r', 50, prob);
			increaseProbability(followingChar, 'k', 20, prob);
		}
	}
}

void improveProbabilityBy_that_that_then_they_them(string enctxt, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	map<string, int> fourStringCount;

	replace_all(enctxt, "\n", " "); //���͸� ����� ����

	vector<string> splitedText = split(enctxt, ' ');
	for (int i = 0; i < splitedText.size(); i++) {
		if (splitedText[i].size() == 4) {
			char maybe_t = findResonableCharacter('t', prob);
			char maybe_h = findResonableCharacter('h', prob);
			char maybe_e = findResonableCharacter('e', prob);

			if (splitedText[i][0] == maybe_t && splitedText[i][1] == maybe_h && splitedText[i][3] == maybe_t) { //that
				increaseProbability(splitedText[i][2], 'a', 99, prob);
			}
			else if (splitedText[i][0] == maybe_t && splitedText[i][1] == maybe_h && splitedText[i][2] == findResonableCharacter('a', prob) && splitedText[i][3] != maybe_t) { //than
				increaseProbability(splitedText[i][3], 'n', 80, prob);
				increaseProbability(splitedText[i][3], 'w', 20, prob);
				increaseProbability(splitedText[i][3], 'i', 20, prob);
			}
			else if (splitedText[i][0] == maybe_t && splitedText[i][1] == maybe_h && splitedText[i][2] == maybe_e) {
				if (prob[charToIndex(splitedText[i][3])][charToIndex('n')] >= prob[charToIndex(splitedText[i][3])][charToIndex('y')] && prob[charToIndex(splitedText[i][3])][charToIndex('n')] >= prob[charToIndex(splitedText[i][3])][charToIndex('m')]) { //then
					increaseProbability(splitedText[i][3], 'n', 90, prob);
				}
				else if(prob[charToIndex(splitedText[i][3])][charToIndex('y')] >= prob[charToIndex(splitedText[i][3])][charToIndex('m')]) { //they
					increaseProbability(splitedText[i][3], 'y', 90, prob);
				}
				else { //them
					increaseProbability(splitedText[i][3], 'm', 90, prob);
				}
			}
		}
	}
}

void improveProbabilityBy_im_nt_s(string enctxt, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	int index = -1;
	replace_all(enctxt, "\n", " "); //���͸� ����� ����
	
	while ((index = enctxt.find("'", index + 1)) != string::npos) { //���� ����ǥ�� string���� ã����
		if (enctxt[index - 1] >= 'A' && enctxt[index - 1] <= 'Z' && enctxt[index + 1] >= 'A' && enctxt[index + 1] <= 'Z' && enctxt[index - 2] == ' ' && enctxt[index + 2] == ' ') { // ?'? ���·� ' �յڿ� 1����, �� �յڿ� ������ �� i'm
			increaseProbability(enctxt[index - 1], 'i', 80, prob);
			increaseProbability(enctxt[index + 1], 'm', 80, prob);
		}
		else if (enctxt[index - 1] >= 'A' && enctxt[index - 1] <= 'Z' && enctxt[index + 1] >= 'A' && enctxt[index + 1] <= 'Z' && enctxt[index - 2] != ' ') { // ...'? ����
			if (prob[charToIndex(enctxt[index + 1])][charToIndex('t')] > prob[charToIndex(enctxt[index + 1])][charToIndex('s')]) { //'s���� 't�� ���ɼ��� ������
				increaseProbability(enctxt[index - 1], 'n', 70, prob); //'t�̴ϱ� n't�� Ȯ�� ����
				increaseProbability(enctxt[index + 1], 't', 80, prob);

				if (enctxt[index - 4] == findResonableCharacter('a', prob) && enctxt[index - 2] == findResonableCharacter('e', prob) && enctxt[index - 5] == ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //aRen't
					increaseProbability(enctxt[index - 3], 'r', 80, prob);
				}
				else if (prob[charToIndex(enctxt[index - 3])][charToIndex('i')] > prob[charToIndex(enctxt[index - 3])][charToIndex('w')] && prob[charToIndex(enctxt[index - 3])][charToIndex('i')] > prob[charToIndex(enctxt[index - 3])][charToIndex('c')] && enctxt[index - 4] == ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //iSn't
					increaseProbability(enctxt[index - 2], 's', 80, prob);
				}
				else if (prob[charToIndex(enctxt[index - 3])][charToIndex('i')] < prob[charToIndex(enctxt[index - 3])][charToIndex('w')] && prob[charToIndex(enctxt[index - 3])][charToIndex('w')] > prob[charToIndex(enctxt[index - 3])][charToIndex('c')] && enctxt[index - 4] == ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //WOn't
					increaseProbability(enctxt[index - 3], 'w', 50, prob);
					increaseProbability(enctxt[index - 2], 'o', 50, prob);
				}
				else if (enctxt[index - 7] == ' ' && enctxt[index - 6] != ' ' && enctxt[index - 5] != ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //wouldn't, shouldn't, couldn't
					increaseProbability(enctxt[index - 5], 'o', 80, prob);
					increaseProbability(enctxt[index - 4], 'u', 80, prob);
					increaseProbability(enctxt[index - 3], 'l', 80, prob);
					increaseProbability(enctxt[index - 2], 'd', 80, prob);
					increaseProbability(enctxt[index - 1], 'n', 80, prob);
				}
				else if (enctxt[index - 2] == findResonableCharacter('a', prob) && enctxt[index - 1] == findResonableCharacter('n', prob) && enctxt[index - 4] == ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //Can't
					increaseProbability(enctxt[index - 3], 'c', 90, prob);
				}
				else if (enctxt[index - 3] == findResonableCharacter('a', prob) && enctxt[index - 5] == ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //WaSn't
					increaseProbability(enctxt[index - 4], 'w', 50, prob);
					increaseProbability(enctxt[index - 2], 's', 50, prob);
				}
				else if (enctxt[index - 4] == findResonableCharacter('e', prob) && enctxt[index - 2] == findResonableCharacter('e', prob) && enctxt[index - 6] == ' ' && enctxt[index - 5] != ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //WeRen't
					increaseProbability(enctxt[index - 5], 'w', 90, prob);
					increaseProbability(enctxt[index - 3], 'r', 90, prob);
				}
				else if (enctxt[index - 4] == ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //DOn't
					increaseProbability(enctxt[index - 3], 'd', 90, prob);
					increaseProbability(enctxt[index - 2], 'o', 90, prob);
				}
				else if (enctxt[index - 5] == ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 4] == enctxt[index - 2] && enctxt[index - 1] != ' ') { //DIDn't
					increaseProbability(enctxt[index - 2], 'd', 95, prob);
					increaseProbability(enctxt[index - 3], 'i', 95, prob);
				}
				else if (enctxt[index - 6] == ' ' && enctxt[index - 5] != ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ') { //DOeSn't
					increaseProbability(enctxt[index - 5], 'd', 90, prob);
					increaseProbability(enctxt[index - 4], 'o', 90, prob);
					increaseProbability(enctxt[index - 2], 's', 90, prob);
				}
			}
		}
	}
}

void decrypt() {
	ifstream is1, is2;
	ofstream os;
	is1.open("key.txt");
	is2.open("ciphertext.txt");
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
}

void improveProbabilityBy_covid19(string enctxt, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	int index = -1;
	replace_all(enctxt, "\n", " "); //���͸� ����� ����

	while ((index = enctxt.find("-19", index + 1)) != string::npos) {
		if (enctxt[index - 6] == ' ' && enctxt[index - 5] != ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ' && enctxt[index + 3] == ' ') { //covid-19
			increaseProbability(enctxt[index - 5], 'c', 80, prob);
			increaseProbability(enctxt[index - 4], 'o', 80, prob);
			increaseProbability(enctxt[index - 3], 'v', 80, prob);
			increaseProbability(enctxt[index - 2], 'i', 80, prob);
			increaseProbability(enctxt[index - 1], 'd', 80, prob);
		}
	}

	index = -1;
	while ((index = enctxt.find("19", index + 1)) != string::npos) {
		if (enctxt[index - 6] == ' ' && enctxt[index - 5] != ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] != ' ' && enctxt[index + 3] == ' ') { //covid19
			increaseProbability(enctxt[index - 5], 'c', 30, prob);
			increaseProbability(enctxt[index - 4], 'o', 30, prob);
			increaseProbability(enctxt[index - 3], 'v', 30, prob);
			increaseProbability(enctxt[index - 2], 'i', 30, prob);
			increaseProbability(enctxt[index - 1], 'd', 30, prob);
		}
		else if (enctxt[index - 7] == ' ' && enctxt[index - 6] != ' ' && enctxt[index - 5] != ' ' && enctxt[index - 4] != ' ' && enctxt[index - 3] != ' ' && enctxt[index - 2] != ' ' && enctxt[index - 1] == ' ' && enctxt[index + 3] == ' ') { //covid 19
			increaseProbability(enctxt[index - 6], 'c', 20, prob);
			increaseProbability(enctxt[index - 5], 'o', 20, prob);
			increaseProbability(enctxt[index - 4], 'v', 20, prob);
			increaseProbability(enctxt[index - 3], 'i', 20, prob);
			increaseProbability(enctxt[index - 2], 'd', 20, prob);
		}
	}
}

//��ȣ�� ����
void startAttack() {
	string encryptedFilePath = "ciphertext.txt"; //������ ��ȣ�� txt ���
	string frequencyFilePath = "frequency.txt"; //���ĺ� �󵵼� txt ���
	string encryptedText = ""; //�ҷ��� ��ȣ��
	int frequency[ALPHABET_SIZE] = { 0, }; //��ȣ���� ���ĺ� �� ��
	int totalAlphabetCount = 0; //��ȣ���� �ִ� ���ĺ����� �� ����
	double probability[ALPHABET_SIZE][ALPHABET_SIZE] = { 0, }; //��ȣ���� � ���ڰ� ������ �� ���ĺ��� Ȯ���� �󸶳� �Ǵ��� ������ 2���� ���̺�
	string keyFilePath = "key.txt"; //key.txt
	int keys[ALPHABET_SIZE]; for (int i = 0; i < ALPHABET_SIZE; i++) { keys[i] = -1; } //���� -1�� �ʱ�ȭ�� �迭 ����, �����س� key �� �迭


	//��ȣȭ�� ���� �о� ��
	openEncryptedFile(encryptedFilePath, &encryptedText);

	//��� �빮�ڷ� ��ȯ
	stringToUpper(&encryptedText);

	//�� �� ī��Ʈ
	countAlphabet(encryptedText, frequency, &totalAlphabetCount, frequencyFilePath);



	//===== 1�ܰ� =====//
	calculateSimpleProbability(frequency, probability, totalAlphabetCount); //���� ��ȣ���� K�� �󵵼��� 11.600%��� 12.702%�� e�� ���� �����ϹǷ� K�� e�� Ȯ���� 100 - |12.702 - 11.600|���� ����ؼ� 98.898%�� å���Ѵ�

	//===== 2�ܰ� =====//
	improveProbabilityBy_the(encryptedText, probability); //the�� ���� ���� �����ϴ� 3���� �ܾ����� �̿��ؼ� t, h, e�� ã�Ƴ���

	//===== 3�ܰ� =====//
	improveProbabilityBy_that_that_then_they_them(encryptedText, probability); //that, then, they, them �̿��ؼ� a, y, n, m�� ã�Ƴ��� (a�� Ȯ�ǽ� y, n, m�� �ణ��)

	//===== 4�ܰ� =====//
	improveProbabilityBy_i_and_a(encryptedText, probability); //a, i�� ���� ���� �����ϴ� 1���� �ܾ����� �̿��ؼ� a, i�� ã�Ƴ���.

	//===== 5�ܰ� =====//
	improveProbabilityBy_im_nt_s(encryptedText, probability); //i'm 's n't �̿��ؼ� m, s, o, u, l, d, n, t�� ã�Ƴ���

	//===== 6�ܰ� =====//
	improveProbabilityBy_on_of_or_ok(encryptedText, probability); //o ������ ���� n f r k�� Ȯ���� ���δ�

	//===== 7�ܰ� =====//
	improveProbabilityBy_covid19(encryptedText, probability); //covid-19�� c v�� ã�Ƴ���


	//printProbability(probability);


	//������ ����� Ȯ������ ���ؼ� ����� �߷�����
	for (int j = 0; j < ALPHABET_SIZE; j++) {
		findHighestProbabilityPlainAlphabetOfEncryptedAlphabet(j, probability, keys);
	}

	//���� key ���
	ofstream writeKeyFile(keyFilePath.data());
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		writeKeyFile << keys[i] << endl;
	}
	writeKeyFile.close();

	decrypt();
}

int main() {
	startAttack();

	return 0;
}