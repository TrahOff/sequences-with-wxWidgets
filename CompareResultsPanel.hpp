#pragma once

#include <wx/filename.h>

#include <vector>

#include "ShowTestsPanel.hpp"

using std::vector;

class CompareResultsPanel : public wxSplitterWindow
{
private:
	ShowTestPanel* showMainTest = NULL;
	ShowTestPanel* showThereTest = NULL;
public:
	CompareResultsPanel(wxWindow* parent, vector<TestResult>& main_results, vector<TestResult>& second_results);
	void setWidths();
};