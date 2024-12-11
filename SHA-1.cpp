#define _CRT_SECURE_NO_WARNINGS

#include "SHA-1.hpp"

#define LOG_STATE \
    cout << "State after block: " << hex << state[0] << ", " << state[1] << ", " << state[2] << ", " << state[3] << ", " << state[4] << dec << endl;

SHA1::SHA1()
{
	init();
}

void SHA1::init()
{
	state = { H0, H1, H2, H3, H4 };
}

void SHA1::update(string& message)
{
	init();
	vector<unsigned char> cleanedMessage;

	for (unsigned char ch : message) {
		if (ch != '\n') cleanedMessage.push_back(ch);
	}

	cleanedMessage.push_back(static_cast<unsigned char>(0x80));

	while (cleanedMessage.size() % 64 != 56) {
		cleanedMessage.push_back(static_cast<unsigned char>(0x00));
	}

	for (int i = 56; i >= 0; i -= 8) {
		cleanedMessage.push_back(static_cast<unsigned char>((message.length() * 8) >> i));
	}

	string newmessage = "";
	for (unsigned char ch : cleanedMessage) {
		newmessage += ch;
	}

	process(newmessage);
}

string SHA1::getHash()
{
	string hash = "";
	stringstream ss;

	for (auto st : state) {
		ss << hex << setw(8) << setfill('0') << st;
		hash += ss.str();
		ss.str("");
	}

	return hash;
}

void SHA1::process(string message)
{
	for (int i = 0; i < message.length(); i += 64) {
		uint32_t W[80]{};
		for (int j = 0; j < 80; j++) {
			if (j < 16) {
				W[j] = ((message[i + j * 4] << 24) | (message[i + j * 4 + 1] << 16) | (message[i + j * 4 + 2] << 8) | (message[i + j * 4 + 3]));
			}
			else {
				W[j] = (W[j - 3] ^ W[j - 8] ^ W[j - 14] ^ W[j - 16]) >> 1;
			}
		}

		uint32_t A = state[0];
		uint32_t B = state[1];
		uint32_t C = state[2];
		uint32_t D = state[3];
		uint32_t E = state[4];

		for (int j = 0; j < 80; j++) {
			uint32_t f = 0x00, k = 0x00;

			if (j < 20) {
				f = (B & C) | ((~B) & D);
				k = 0x5A827999;
			}
			else if (j < 40) {
				f = B ^ C ^ D;
				k = 0x6ED9EBA1;
			}
			else if (j < 60) {
				f = (B & C) | (B & D) | (C & D);
				k = 0x8F1BBCDC;
			}
			else {
				f = B ^ C ^ D;
				k = 0xCA62C1D6;
			}

			uint32_t tmp = (A << 5) + f + E + k + W[j];
			E + D; D = C; C = B << 30; B = A; A = tmp;
		}

		state[0] += A;
		state[1] += B;
		state[2] += C;
		state[3] += D;
		state[4] += E;
	}
}
