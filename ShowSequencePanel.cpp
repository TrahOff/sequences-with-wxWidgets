#include "ShowSequencePanel.hpp"

ShowSequencePanel::ShowSequencePanel(wxWindow* parent, vector<bool> sequence) :
	wxScrolledWindow(parent, wxID_ANY)
{
	this->SetBackgroundColour(wxColor("#EBEBEB"));
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* label = new wxStaticText(this, wxID_ANY, "Последовательность:",
		wxDefaultPosition, wxDefaultSize);

	string seqAsStr = "";
	for (const auto& bit : sequence) {
		seqAsStr += (bit ? "1" : "0");
	}

	wxTextCtrl* sequenceDisplay = new wxTextCtrl(this, wxID_ANY, seqAsStr,
		wxDefaultPosition, wxDefaultSize,
		wxTE_MULTILINE | wxTE_READONLY | wxTE_WORDWRAP | wxTE_RICH | wxBORDER_NONE);

	sequenceDisplay->SetBackgroundColour(wxColor("#EBEBEB"));

	label->SetFont(wxFont(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));
	sequenceDisplay->SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));

	wxBoxSizer* displayContentSizer = new wxBoxSizer(wxHORIZONTAL);
	displayContentSizer->Add(sequenceDisplay, 1, wxEXPAND | wxALL, 10);

	mainSizer->Add(label, 0, wxTOP | wxLEFT | wxRIGHT, 20);
	mainSizer->Add(displayContentSizer, 1, wxEXPAND | wxALL, 20);

	this->SetSizer(mainSizer);
}
