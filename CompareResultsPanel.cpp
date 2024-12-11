#include "CompareResultsPanel.hpp"

CompareResultsPanel::CompareResultsPanel(wxWindow* parent, vector<TestResult>& main_results, vector<TestResult>& second_results) :
    wxSplitterWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE)
{
    showMainTest = new ShowTestPanel(this, main_results, wxString("последовательность №1"));
    showThereTest = new ShowTestPanel(this, second_results, wxString("последовательность №2"));

    showMainTest->SetBackgroundColour(wxColor("#EBEBEB"));
    showThereTest->SetBackgroundColour(wxColor("#FDEDCC"));

    this->SplitHorizontally(showMainTest, showThereTest);
    this->SetMinimumPaneSize(10);
}

void CompareResultsPanel::setWidths()
{
    showMainTest->SetColumnWidths();
    showThereTest->SetColumnWidths();
    this->SetSashPosition(this->GetSize().GetHeight() / 2, true);
}
