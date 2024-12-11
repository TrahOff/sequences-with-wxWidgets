#pragma once

#include <wx/filename.h>

#include <fstream>

#include "ActionManager.hpp"
#include "ControlPanel.hpp"
#include "GenerateFormPanel.hpp"
#include "ShowSequencePanel.hpp"
#include "ShowTestsPanel.hpp"
#include "CompareResultsPanel.hpp"

using std::ios;

class MyFrame : public wxFrame
{
private:
	wxSplitterWindow* splitter = NULL;

	ControlPanel* control = NULL;
	wxPanel* content = NULL;
	GenerateFormPanel* mainGenerateForm = NULL;
	ShowSequencePanel* showSequence = NULL;
	ShowTestPanel* showTest = NULL;
	CompareResultsPanel* compareResults = NULL;

	vector<wxCommandEventFunction> main_functions = {
		wxCommandEventFunction(&MyFrame::ShowMainGenerateForm),
		wxCommandEventFunction(&MyFrame::LoadMainFromFile),
		wxCommandEventFunction(&MyFrame::SaveToFile),
		wxCommandEventFunction(&MyFrame::TestMainSequence),
		wxCommandEventFunction(&MyFrame::CompareSequences),
		wxCommandEventFunction(&MyFrame::OnClose)
	};

	vector<bool> main_sequence, second_sequence;
	vector<TestResult> main_results, second_results;
public:
	MyFrame(const wxString& title, const wxPoint& position, const wxSize& size);
private:
	void ShowMainGenerateForm(wxCommandEvent& event);
	void GenerateSequence();

	void LoadMainFromFile(wxCommandEvent& event);

	void TestMainSequence(wxCommandEvent& event);
	
	void CompareSequences(wxCommandEvent& event);

	void OnClose(wxCommandEvent& event);

	void deletePanel();
	void LoadFromFile(vector<bool>& seq, const wxString& title);
	void SaveToFile(wxCommandEvent& event);
};