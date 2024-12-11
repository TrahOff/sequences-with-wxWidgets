#pragma once

#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <array>

using namespace std;

class DES
{
private:
	const static array<int, 64> initialPermutationTable;
	const static array<int, 16> LeftMoveTable;
	const static array<int, 56> parityBitDropTable;
	const static array<int, 48> keyCompressionTable;
	const static array<int, 48> expansionPermutationTalbe;
	const static array<int, 32> straightPermutationTable;
	const static array<int, 64> finalPermutationTable;

	const static int sAll[8][4][16];
public:
	DES();
	virtual ~DES();

	vector<string> encrypt(string& PlainText, string& password);
	string decrypt(vector<string>& CypheredText, string& password);

	vector<string>DES_EEE3_encrypt(string& PlainText, string& password);
	string DES_EEE3_decrypt(vector<string>& CypheredText, string& password);
private:
	string TextToBin(string& text);
	string BinToText(string& bintext);
	int binToDecimal(const string& binary);

	string generatePassword8Byte(string& password, int round);

	vector<string> GetBlocksFromText(string& text);

	// блок с генерацией ключей
	string Create64BitKey(const string& Password);
	string Create56BitKey(const string& key64);
	string Create48BitKey(const string& key56);
	string GenerateKeyI(const string& key, int round);

	// блок с перестановками битов
	string initialPermutation(const string& Block64) const;
	string ExpansionPermutation(const string& RightPart32) const;
	string straightPermutation(const string& sBox);
	string FinalPermitation(const string& final_str);

	// функция Фейстеля и все вытекающие
	string FeistelFunction(const string& RightPart32, const string& key48);
	string sBox(const string& xor48);

	// функции XOR
	string XOR32(const string& block32, const string& otherBlock32) const;
	string XOR48(const string& block48, const string& key48) const;
};