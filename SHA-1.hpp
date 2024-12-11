#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

class SHA1 {
private:
	static constexpr uint32_t H0 = 0x67452301;
	static constexpr uint32_t H1 = 0xEFCDAB89;
	static constexpr uint32_t H2 = 0x98BADCFE;
	static constexpr uint32_t H3 = 0x10325476;
	static constexpr uint32_t H4 = 0xC3D2E1F0;

	vector<uint32_t> state;
public:
	SHA1();
	void update(string& message);
	string getHash();
private:
	void init();
	void process(string message);
};
