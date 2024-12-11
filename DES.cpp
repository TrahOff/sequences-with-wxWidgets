#include "DES.hpp"

// инициализация констатных таблиц шифрования
// используются в процессе шифрования

// таблица начальной перестановки внутри 64-битного блока
const array<int, 64> DES::initialPermutationTable
{
	58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7
};

// перестановка битов 64 битного ключа, для создания 56 битного
const array<int, 56> DES::parityBitDropTable
{
	57, 49, 41, 33, 25, 17,  9,  1,
	58, 50, 42, 34, 26, 18, 10,  2,
	59, 51, 43, 35, 27, 19, 11,  3,
	60, 52, 44, 36, 63, 55, 47, 39,
	31, 23, 15,  7, 62, 54, 46, 38,
	30, 22, 14,  6, 61, 53, 45, 37,
	29, 21, 13,  5, 28, 20, 12,  4
};

// перестановка битов 56 битного ключа, для создания 48 битного
const std::array<int, 48> DES::keyCompressionTable
{
	14, 17, 11, 24,  1,  5,  3, 28,
	15,  6, 21, 10, 23, 19, 12,  4,
	26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40,
	51, 45, 33, 48, 44, 49, 39, 56,
	34, 53, 46, 42, 50, 36, 29, 32
};

// таблица сдвигов ключа влево, в зависимости от итерации шифрования
const array<int, 16> DES::LeftMoveTable
{
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// применяется в функции Фейстеля, для расширения 32 битного блока в 48 битный
const std::array<int, 48> DES::expansionPermutationTalbe
{
   32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
   12, 13, 14, 15, 16, 17,
   16, 17, 18, 19, 20, 21,
   20, 21, 22, 23, 24, 25,
   24, 25, 26, 27, 28, 29,
   28, 29, 30, 31, 32,  1
};

// блоки преобразования S1-S8 для функции Фейстеля
const int DES::sAll[8][4][16] =
{
	{ /* S1 */
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	},
	{ /* S2 */
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	},
	{ /* S3 */
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	},
	{ /* S4 */
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	},
	{ /* S5 */
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	},
	{ /* S6 */
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	},
	{ /* S7 */
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	},
	{ /* S8 */
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	}
};

// конечная перестановка в функции Фейстеля
const array<int, 32> DES::straightPermutationTable
{
	16,  7, 20, 21, 29, 12, 28, 17,
	 1, 15, 23, 26, 05, 18, 31, 10,
	 2,  8, 24, 14, 32, 27,  3,  9,
	19, 13, 30,  6, 22, 11,  4, 25
};

// используется для финальной перестановки блока текста
const array<int, 64> DES::finalPermutationTable
{
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

DES::DES() {}

DES::~DES()
{
	const static array<int, 64> initialPermutationTable;
	const static array<int, 16> LeftMoveTable;
	const static array<int, 56> parityBitDropTable;
	const static array<int, 48> keyCompressionTable;
	const static array<int, 48> expansionPermutationTalbe;
	const static array<int, 32> straightPermutationTable;
	const static array<int, 64> finalPermutationTable;

	const static int sAll[8][4][16];
}

vector<string> DES::encrypt(string& PlainText, string& password)
{
	vector<string> blocks = GetBlocksFromText(PlainText);
	string key = Create64BitKey((password.length() != 8 ? password : generatePassword8Byte(password, 0)));
	vector<string> CypheredText;

	for (string& block : blocks) {
		string strAsBin = TextToBin(block);
		strAsBin = initialPermutation(strAsBin);

		string LeftPart = strAsBin.substr(0, 32), RightPart = strAsBin.substr(32, 32);
		string key56 = Create56BitKey(key);

		for (int i = 0; i < 16; i++) {
			string newKey56 = GenerateKeyI(key56, i);
			string key48 = Create48BitKey(newKey56);

			string tmp = LeftPart;
			LeftPart = RightPart;
			string FeistelRes = FeistelFunction(RightPart, key48);
			RightPart = XOR32(tmp, FeistelRes);
		}

		strAsBin = LeftPart + RightPart;

		CypheredText.push_back(FinalPermitation(strAsBin));
	}

	return CypheredText;
}

string DES::decrypt(vector<string>& CypheredText, string& password)
{
	string plainText = "";
	string key = Create64BitKey((password.length() != 8 ? password : generatePassword8Byte(password, 0)));

	for (string& str : CypheredText) {
		string StrAsBin = initialPermutation(str);

		string LeftPart = StrAsBin.substr(0, 32), RightPart = StrAsBin.substr(32, 32);
		string key56 = Create56BitKey(key);

		vector<string> keys48;

		for (int i = 0; i < 16; i++) {
			string newKey56 = GenerateKeyI(key56, i);
			string key48 = Create48BitKey(newKey56);
			keys48.push_back(key48);
		}

		for (int i = 15; i >= 0; i--) {
			string tmp = RightPart;
			RightPart = LeftPart;
			string FeistelResult = FeistelFunction(LeftPart, keys48[i]);
			LeftPart = XOR32(tmp, FeistelResult);
		}
		StrAsBin = LeftPart + RightPart;

		StrAsBin = FinalPermitation(StrAsBin);
		plainText += BinToText(StrAsBin);
	}

	return plainText;
}

vector<string> DES::DES_EEE3_encrypt(string& PlainText, string& password)
{
	vector<string> CypheredText;
	string currentText = PlainText;

	for (int i = 0; i < 3; i++) {
		string newPassword = generatePassword8Byte(password, i);
		CypheredText = encrypt(currentText, newPassword);
		if (i != 2) {
			currentText = "";
			for (auto& block : CypheredText) {
				currentText += BinToText(block);
			}
			CypheredText.clear();
		}
	}

	return CypheredText;
}

string DES::DES_EEE3_decrypt(vector<string>& CypheredText, string& password)
{
	vector<string> tmpCyphered = CypheredText;
	string currentText = "";

	for (int i = 2; i >= 0; i--) {
		string newPassword = generatePassword8Byte(password, i);
		currentText = decrypt(tmpCyphered, newPassword);
		if (i != 0) {
			tmpCyphered.clear();
			tmpCyphered = GetBlocksFromText(currentText);
			for (auto& elem : tmpCyphered) {
				elem = TextToBin(elem);
			}
			currentText = "";
		}
	}

	return currentText;
}

string DES::TextToBin(string& text)
{
	string BinStr = "";
	for (char ch : text) {
		BinStr += bitset<8>(ch).to_string();
	}
	return BinStr;
}

string DES::BinToText(string& bintext)
{
	string text = "";
	int charCount = 0;
	for (int i = 0; i < bintext.length(); i += 8) {
		string byte = bintext.substr(i, 8);
		char ch = static_cast<char>(stoi(byte, nullptr, 2));
		text += ch;
	}
	return text;
}

int DES::binToDecimal(const string& binary)
{
	int decimal = 0;
	for (auto it = binary.begin(); it < binary.end(); it++) {
		if (*it == '1') {
			decimal += pow(2, (binary.length() - 1) - distance(binary.begin(), it));
		}
	}
	return decimal;
}

string DES::generatePassword8Byte(string& password, int round)
{
	string newPassword = "";
	vector<uint8_t> elems = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
	for (int i = 0; i < password.length(); i++) {
		switch (round)
		{
		case 0:
			elems[i % 8] += static_cast<uint8_t>(password[i]);
			break;
		case 1:
			elems[i % 8] -= static_cast<uint8_t>(password[i]);
			break;
		case 2:
			elems[i % 8] ^= static_cast<uint8_t>(password[i]);
		default:
			break;
		}
	}
	for (const auto& elem : elems) {
		newPassword += static_cast<char>(elem);
	}
	return newPassword;
}

vector<string> DES::GetBlocksFromText(string& text)
{
	int start = 0, step = 8;
	vector<string> TextBlocks;

	while (text.length() % 8 != 0) {
		text += " ";
	}

	while (start < text.length()) {
		TextBlocks.push_back(text.substr(start, step));
		start += step;
	}

	return TextBlocks;
}

string DES::Create64BitKey(const string& Password)
{
	string newKey = "";
	for (char elem : Password) {
		bitset<8> BinElem(elem);
		newKey += BinElem.to_string();
	}
	return newKey;
}

string DES::Create56BitKey(const string& key64)
{
	bitset<56> key56;
	int index = 0, pos = 0;
	for (auto it = key64.begin(); it < key64.end(); it++) {
		pos = distance(key64.begin(), it) + 1;
		if (pos == 20)
			if (*it == '1' && (pos % 8 != 0)) {
				index = distance(parityBitDropTable.begin(),
					find(parityBitDropTable.begin(), parityBitDropTable.end(), pos));
				key56.set((key56.size() - 1) - index, 1);
			}
	}
	return key56.to_string();
}

string DES::Create48BitKey(const string& key56)
{
	bitset<48> key48;
	int index, pos;
	for (auto it = key56.begin(); it < key56.end(); it++) {
		pos = distance(key56.begin(), it) + 1;
		if (*it == '1' && pos != 9 && pos != 18 && pos != 22 && pos != 25 && pos != 35 && pos != 38 && pos != 43 && pos != 54) {
			index = distance(keyCompressionTable.begin(),
				find(keyCompressionTable.begin(), keyCompressionTable.end(), pos));
			key48.set(47 - index, 1);
		}
	}
	return key48.to_string();
}

string DES::GenerateKeyI(const string& key, int round)
{
	string leftKeyPart = key.substr(0, 28), rightKeyPart = key.substr(28, 28);

	rotate(leftKeyPart.begin(), leftKeyPart.begin() + LeftMoveTable[round], leftKeyPart.end());
	rotate(rightKeyPart.begin(), rightKeyPart.begin() + LeftMoveTable[round], rightKeyPart.end());

	return leftKeyPart + rightKeyPart;
}

string DES::initialPermutation(const string& Block64) const
{
	bitset<64> newBlock;
	int index = 0;
	for (auto it = Block64.begin(); it < Block64.end(); it++) {
		if (*it == '1') {
			index = distance(initialPermutationTable.begin(),
				find(initialPermutationTable.begin(), initialPermutationTable.end(), distance(Block64.begin(), it) + 1));
			newBlock.set((newBlock.size() - 1) - index, 1);
		}
	}
	return newBlock.to_string();
}

string DES::ExpansionPermutation(const string& RightPart32) const
{
	string RightPart48 = "";
	for (int i = 0; i < 48; i++) {
		RightPart48 += RightPart32[expansionPermutationTalbe.at(i) - 1];
	}
	return RightPart48;
}

string DES::straightPermutation(const string& sBox)
{
	bitset<32> sBoxPerm;
	int index;
	for (auto it = sBox.begin(); it < sBox.end(); it++) {
		if (*it == '1') {
			index = distance(straightPermutationTable.begin(),
				find(straightPermutationTable.begin(), straightPermutationTable.end(), distance(sBox.begin(), it) + 1));
			sBoxPerm.set((sBoxPerm.size() - 1) - index, 1);
		}
	}

	return sBoxPerm.to_string();
}

string DES::FinalPermitation(const string& final_str)
{
	bitset<64> finalBlock;
	int index;
	for (auto it = final_str.begin(); it < final_str.end(); it++) {
		if (*it == '1') {
			index = distance(finalPermutationTable.begin(),
				find(finalPermutationTable.begin(), finalPermutationTable.end(), distance(final_str.begin(), it) + 1));
			finalBlock.set(63 - index, 1);
		}
	}
	return finalBlock.to_string();
}

string DES::FeistelFunction(const string& RightPart32, const string& key48)
{
	string extended48BitPart = ExpansionPermutation(RightPart32);
	string xor48 = XOR48(extended48BitPart, key48);
	string Sboxes = sBox(xor48);
	string resulOfFeistel = straightPermutation(Sboxes);
	return resulOfFeistel;
}

string DES::sBox(const string& xor48)
{
	string sBox = "";
	for (int i = 0; i < 8; i++) {
		bitset<4> box;
		int row = binToDecimal(xor48.substr(i * 6, 1) + xor48.substr(i * 6 + 5, 1));
		int col = binToDecimal(xor48.substr(i * 6 + 1, 4));

		box = sAll[i][row][col];
		sBox += box.to_string();
	}
	return sBox;
}

string DES::XOR32(const string& block32, const string& otherBlock32) const
{
	bitset<32> block(block32), otherBlock(otherBlock32);
	return (block ^ otherBlock).to_string();
}

string DES::XOR48(const string& block48, const string& key48) const
{
	bitset<48> block(block48), key(key48);
	return (block ^ key).to_string();
}
