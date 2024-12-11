#pragma once

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/msgdlg.h>
#include <wx/window.h>
#include <wx/progdlg.h>

#include <vector>
#include <stdexcept>
#include <map>

using std::vector;
using std::invalid_argument;
using std::map;

#include "Yarrow-160.hpp"

class ActionManager
{
public:
	static bool CheckEnteredLength(wxString value);
	static void GenerateRandomBits(wxString value, wxString generator, vector<bool>& sequence);

	static bool FrequencyTest(vector<bool>& frequency);
	static bool TestForASequenceOfIdenticalBits(vector<bool>& frequency);
	static vector<bool> extendedTestForArbitraryDeviations(vector<bool>& frequency);
private:
	static void QuadraticCongruentGenerator(vector<bool>& sequence, int size);
	static void BBSGenerator(vector<bool>& sequence, int size);
	static void Yarrow_160(vector<bool>& sequence, int size);

	static bool PrimeNuber(long long number);
	static unsigned long long GCD(unsigned long long a, unsigned long long b);
};