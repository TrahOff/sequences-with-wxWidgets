#pragma once

#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/splitter.h>

#include <string>
#include <vector>

using std::vector;
using std::string;
using std::to_string;

struct TestResult {
	string testName;
	int testNumber;
	bool result;
};

class ShowTestPanel : public wxScrolledWindow
{
private:
	wxListView* listView = NULL;
public:
	ShowTestPanel(wxWindow* parent, vector<TestResult>& results, const wxString& labelTest);
	void SetColumnWidths();
private:
	void AddSingleDataToLIst(wxListView* listView, string testName, int testNumber, bool result);
};