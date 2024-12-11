#include "Yarrow-160.hpp"

YarrowGenerator::YarrowGenerator(uint64_t initial_seed)
{
	n = 64; k = 64;
	P1 = 10, P2 = 5;
	sigma1 = P1; sigma2 = P2;
	C = 0; counter = 0;
	KEY = ""; entropy = "";
	seed = initial_seed;
	collect_entroty();
}

YarrowGenerator::~YarrowGenerator()
{
	n = 0; k = 0; P1 = 0; P2 = 0;
	sigma1 = 0; sigma2 = 0; C = 0; counter = 0;
	KEY.clear();
	entropy.clear();
}

uint32_t YarrowGenerator::generate()
{
	if (sigma1 == 0) {
		updateKey();
		sigma1 = P1;
	}

	if (sigma2 == 0) {
		updateKeyCounter();
		sigma2 = P2;
	}

	uint32_t random_number = counter ^ seed;
	random_number *= 1103515245;
	random_number += 12345;
	random_number &= 0xFFFFFFFFFFFFFFFFULL;

	DES des;
	string pass = "YarrowGenetrator";
	string plainText = to_string(random_number);

	string block = des.encrypt(plainText, pass)[0];

	sigma1--;
	sigma2--;

	counter++;

	uint32_t tmpuint = 0;

	for (auto it = block.begin(); it < block.end(); it++) {
		if (*it == '1') {
			tmpuint += pow(2, (block.length() - 1) - distance(block.begin(), it));
		}
	}

	return tmpuint;
}

void YarrowGenerator::collect_entroty()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 255);

	auto now = chrono::duration_cast<chrono::milliseconds>(
		chrono::system_clock::now().time_since_epoch()
	).count();

	ostringstream ss;
	ss << static_cast<int>(dis(gen)) << hex << now << dec << _getpid();

	entropy = "";
	entropy = ss.str();
}

void YarrowGenerator::updateKey()
{
	counter = (counter + 1) % (1ULL << 27);
	KEY = encrypt(to_string(counter));
}

void YarrowGenerator::updateKeyCounter()
{
	SHA1 hash;
	string tmp = entropy + KEY;

	hash.update(tmp);
	KEY = hash.getHash();

	counter = (counter + 1) % (1ULL << 27);
}

string YarrowGenerator::encrypt(string str)
{
	DES des;
	string pass = "YarrowKey";
	return des.encrypt(str, pass)[0];
}
