#include "ShowTestsPanel.hpp"

ShowTestPanel::ShowTestPanel(wxWindow* parent, vector<TestResult>& results, const wxString& labelTest) :
	wxScrolledWindow(parent, wxID_ANY)
{
	this->SetBackgroundColour(wxColor("#EBEBEB"));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* displayListSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* label = new wxStaticText(this, wxID_ANY, labelTest,
		wxDefaultPosition, wxDefaultSize);
	label->SetFont(wxFont(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));

	listView = new wxListView(this);
	listView->AppendColumn("название теста");
	listView->AppendColumn("номер теста");
	listView->AppendColumn("результат теста");
	listView->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));

	for (int i = 0; i < results.size(); i++) {
		AddSingleDataToLIst(listView, results[i].testName, results[i].testNumber, results[i].result);
	}

	displayListSizer->Add(listView, 1, wxEXPAND);

	mainSizer->Add(label, 0, wxTOP | wxLEFT | wxRIGHT, 10);
	mainSizer->Add(displayListSizer, 1, wxALL | wxEXPAND, 10);

	this->SetSizerAndFit(mainSizer);
}

void ShowTestPanel::SetColumnWidths()
{
	wxSize size = this->GetSize();
	listView->SetColumnWidth(0, size.GetWidth() * 0.56);
	listView->SetColumnWidth(1, size.GetWidth() * 0.18);
	listView->SetColumnWidth(2, size.GetWidth() * 0.18);
}

void ShowTestPanel::AddSingleDataToLIst(wxListView* listView, string testName, int testNumber, bool result)
{
	int index = listView->GetItemCount();
	listView->InsertItem(index, testName);
	listView->SetItem(index, 1, to_string(testNumber));
	listView->SetItem(index, 2, result ? "успешно" : "провалено");
}
