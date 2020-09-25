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


const int ALPHABET_SIZE = 26; //알파벳 개수
const double ALPHABET_FREQUENCY[ALPHABET_SIZE] = { //영어 알파벳 빈도 수 통계 (%)
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



//문자가 대문자 A~Z인지
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


//찾는 int가 없으면 -1, 있으면 해당 index 반환
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

//글자 수 세기
void countAlphabet(string str, int freq[ALPHABET_SIZE], int* total, string filePathToWrite) {
	for (int i = 0; i < str.size(); i++) {
		if (isCaptital(str[i])) {
			freq[charToIndex(str[i])]++;
			(*total)++;
		}
	}

	//빈도 수 txt 출력
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

//ex) 암호화된 K가 실제로 어떤 알파벳일지 가장 높은 확률인 알파벳으로 선택
int findHighestProbabilityPlainAlphabetOfEncryptedAlphabet(int j, double prob[ALPHABET_SIZE][ALPHABET_SIZE], int keys[ALPHABET_SIZE]) {
	int maxIndex = 0; //ex) 실제 a일 확률이 가장 높은 암호 알파벳
	double maxProbability = 0.0; //ex) 각 암호문이 a일 확률 중에 제일 높은 확률 값

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

//java의 split과 동일한 일을 하는 함수
//출처: https://twinw.tistory.com/220 [흰고래의꿈]
vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str);
	string temp;

	while (getline(ss, temp, delimiter)) {
		internal.push_back(temp);
	}

	return internal;
}

//문자열에서 특정 문자들을 다른 문자로 치환
//출처: https://wendys.tistory.com/8 [웬디의 기묘한 이야기]
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

//A는 암호문이고 대문자, a는 평문이고 소문자 (추측값), weight는 가중치 (0 ~ 100 사이 정수, 100일 경우 100%, 0일 경우 확률 그대로)
void increaseProbability(char A, char a, int weight, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	int indexOfA = charToIndex(A);
	int indexOfa = charToIndex(a);

	prob[indexOfA][indexOfa] += (100.0 - prob[indexOfA][indexOfa]) / 100.0 * weight;
}

//A는 암호문이고 대문자, a는 평문이고 소문자 (추측값), weight는 가중치 (0 ~ 100 사이 정수, 100일 경우 0%, 0일 경우 확률 그대로)
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

//ex) 실제 문자 c 가능성이 높다고 생각되는 가장 합리적인 문자를 찾아준다
char findResonableCharacter(char c, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	double newProb[ALPHABET_SIZE][ALPHABET_SIZE];
	copyProbabilityTable(newProb, prob);

	char ch = findResonableCharacterRecursiver(c, newProb);
	return (ch != '\0') ? ch : findHighestProbabilityCharacter(c, prob);
}

void improveProbabilityBy_the(string enctxt, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	map<string, int> threeStringCount;

	replace_all(enctxt, "\n", " "); //엔터를 띄어쓰기로 변경

	//문자열에서 가장 자주 등장한 3자리 단어를 찾는다 -> the일 가능성이 높음 -> 각자 t, h, e일 확률을 증가시킨다
	vector<string> splitedText = split(enctxt, ' ');
	for (int i = 0; i < splitedText.size(); i++) {
		if (splitedText[i].size() == 3) {
			threeStringCount[splitedText[i]]++;
		}
	}

	if (threeStringCount.size() > 0) {
		string maybe_the = findHighestCountStringInVector(threeStringCount); //아마 the일 것으로 추측되는 string
		char maybe_e = findResonableCharacter('e', prob);

		if (maybe_e == maybe_the[2]) { //ex) 가장 많이 나온 문자가 K인데 가장 많이 나온 3글자 단어의 마지막도 K이면 -> e 거의 확실시, 앞의 두 글자도 t, h 확실시
			increaseProbability(maybe_the[0], 't', 90, prob);
			increaseProbability(maybe_the[1], 'h', 90, prob);
			increaseProbability(maybe_the[2], 'e', 90, prob);
		}
	}
}

void improveProbabilityBy_i_and_a(string enctxt, double prob[ALPHABET_SIZE][ALPHABET_SIZE]) {
	map<string, int> singleStringCount;

	replace_all(enctxt, "\n", " "); //엔터를 띄어쓰기로 변경

	vector<string> splitedText = split(enctxt, ' ');
	for (int i = 0; i < splitedText.size(); i++) {
		if (splitedText[i].size() == 1) {
			singleStringCount[splitedText[i]]++;
		}
	}

	if (singleStringCount.size() > 0) {
		//문자열에서 가장 자주 등장한 1자리 단어를 찾는다 -> a, i일 가능성 높여준다
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

	replace_all(enctxt, "\n", " "); //엔터를 띄어쓰기로 변경

	vector<string> splitedText = split(enctxt, ' ');
	char maybe_o = findResonableCharacter('o', prob);
	for (int i = 0; i < splitedText.size(); i++) {
		if (splitedText[i].size() == 2 && splitedText[i][0] == maybe_o) { //o 다음에 오는 문자 n, f, r, k일 가능성 높임
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

	replace_all(enctxt, "\n", " "); //엔터를 띄어쓰기로 변경

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
	replace_all(enctxt, "\n", " "); //엔터를 띄어쓰기로 변경
	
	while ((index = enctxt.find("'", index + 1)) != string::npos) { //작은 따옴표를 string에서 찾으면
		if (enctxt[index - 1] >= 'A' && enctxt[index - 1] <= 'Z' && enctxt[index + 1] >= 'A' && enctxt[index + 1] <= 'Z' && enctxt[index - 2] == ' ' && enctxt[index + 2] == ' ') { // ?'? 형태로 ' 앞뒤에 1글자, 그 앞뒤엔 공백일 때 i'm
			increaseProbability(enctxt[index - 1], 'i', 80, prob);
			increaseProbability(enctxt[index + 1], 'm', 80, prob);
		}
		else if (enctxt[index - 1] >= 'A' && enctxt[index - 1] <= 'Z' && enctxt[index + 1] >= 'A' && enctxt[index + 1] <= 'Z' && enctxt[index - 2] != ' ') { // ...'? 형태
			if (prob[charToIndex(enctxt[index + 1])][charToIndex('t')] > prob[charToIndex(enctxt[index + 1])][charToIndex('s')]) { //'s보다 't일 가능성이 높으면
				increaseProbability(enctxt[index - 1], 'n', 70, prob); //'t이니까 n't일 확률 증가
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
	replace_all(enctxt, "\n", " "); //엔터를 띄어쓰기로 변경

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

//암호문 공격
void startAttack() {
	string encryptedFilePath = "ciphertext.txt"; //공격할 암호문 txt 경로
	string frequencyFilePath = "frequency.txt"; //알파벳 빈도수 txt 경로
	string encryptedText = ""; //불러온 암호문
	int frequency[ALPHABET_SIZE] = { 0, }; //암호문의 알파벳 빈도 수
	int totalAlphabetCount = 0; //암호문에 있는 알파벳들의 총 개수
	double probability[ALPHABET_SIZE][ALPHABET_SIZE] = { 0, }; //암호문의 어떤 문자가 실제로 각 알파벳일 확률이 얼마나 되는지 정리한 2차원 테이블
	string keyFilePath = "key.txt"; //key.txt
	int keys[ALPHABET_SIZE]; for (int i = 0; i < ALPHABET_SIZE; i++) { keys[i] = -1; } //전부 -1로 초기화된 배열 선언, 추측해낸 key 값 배열


	//암호화된 파일 읽어 옴
	openEncryptedFile(encryptedFilePath, &encryptedText);

	//모두 대문자로 변환
	stringToUpper(&encryptedText);

	//빈도 수 카운트
	countAlphabet(encryptedText, frequency, &totalAlphabetCount, frequencyFilePath);



	//===== 1단계 =====//
	calculateSimpleProbability(frequency, probability, totalAlphabetCount); //만약 암호문의 K의 빈도수가 11.600%라면 12.702%인 e와 가장 유사하므로 K가 e일 확률을 100 - |12.702 - 11.600|으로 계산해서 98.898%로 책정한다

	//===== 2단계 =====//
	improveProbabilityBy_the(encryptedText, probability); //the가 제일 자주 등장하는 3글자 단어임을 이용해서 t, h, e를 찾아낸다

	//===== 3단계 =====//
	improveProbabilityBy_that_that_then_they_them(encryptedText, probability); //that, then, they, them 이용해서 a, y, n, m을 찾아낸다 (a는 확실시 y, n, m은 약간만)

	//===== 4단계 =====//
	improveProbabilityBy_i_and_a(encryptedText, probability); //a, i가 제일 자주 등장하는 1글자 단어임을 이용해서 a, i를 찾아낸다.

	//===== 5단계 =====//
	improveProbabilityBy_im_nt_s(encryptedText, probability); //i'm 's n't 이용해서 m, s, o, u, l, d, n, t를 찾아낸다

	//===== 6단계 =====//
	improveProbabilityBy_on_of_or_ok(encryptedText, probability); //o 다음에 오는 n f r k의 확률을 높인다

	//===== 7단계 =====//
	improveProbabilityBy_covid19(encryptedText, probability); //covid-19로 c v를 찾아낸다


	//printProbability(probability);


	//위에서 계산한 확률값을 통해서 결과를 추려낸다
	for (int j = 0; j < ALPHABET_SIZE; j++) {
		findHighestProbabilityPlainAlphabetOfEncryptedAlphabet(j, probability, keys);
	}

	//최종 key 출력
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