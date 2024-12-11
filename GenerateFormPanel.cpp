#include "GenerateFormPanel.hpp"

GenerateFormPanel::GenerateFormPanel(wxWindow* parent, wxCommandEventFunction function) :
    wxScrolledWindow(parent, wxID_ANY)
{
    this->SetScrollRate(20, 20);
	this->SetBackgroundColour(wxColor("#EBEBEB"));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* internalSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* label = new wxStaticText(this, wxID_ANY, wxT("Генерация последовательности"),
		wxDefaultPosition, wxDefaultSize);

	sequenceLengthCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(550, 50));
	radioBox = new wxRadioBox(this, wxID_ANY, wxT("выберите генератор"), wxDefaultPosition, wxDefaultSize, generators, 1);

	wxWindow* window = dynamic_cast<wxWindow*>(this);
	while (window && !window->IsTopLevel()) {
		window = window->GetParent();
	}

	wxButton* continueBtn = new wxButton(this, wxID_ANY, wxT("продолжить"),
		wxDefaultPosition, wxSize(250, 50), wxBORDER_NONE, wxDefaultValidator, wxT("continue_btn"));
	continueBtn->Bind(wxEVT_BUTTON, function, window);

	radioBox->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));
	label->SetFont(wxFont(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));
	sequenceLengthCtrl->SetFont(wxFont(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));
	continueBtn->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));

	internalSizer->Add(label, 0, wxALIGN_CENTER);
	internalSizer->AddSpacer(50);
	internalSizer->Add(radioBox, 0, wxALIGN_CENTER | wxTOP, 20);
	internalSizer->AddSpacer(50);
	internalSizer->Add(sequenceLengthCtrl, 0, wxALIGN_CENTER | wxTOP, 20);
	internalSizer->Add(continueBtn, 0, wxALIGN_CENTER | wxTOP, 20);

	mainSizer->Add(internalSizer, 1, wxALIGN_CENTER);
	this->SetSizerAndFit(mainSizer);
}

wxString GenerateFormPanel::getLength()
{
	sequenceLengthCtrl->GetValue();
}

wxString GenerateFormPanel::getGenerator()
{
	return radioBox->GetStringSelection();
}
