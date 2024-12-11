#include "ControlPanel.hpp"

ControlPanel::ControlPanel(wxWindow* parent) :
    wxScrolledWindow(parent, wxID_ANY)
{
    this->SetScrollRate(20, 20);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxVERTICAL);

    for (const pair<string, bool>& info : button_info) {
        wxButton* btn = new wxButton(this, wxID_ANY, info.first, wxDefaultPosition, wxSize(150, 50));
        btn->SetBackgroundColour(this->GetBackgroundColour());
        btn->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));
        btn->Enable(info.second);

        if (info.first == "выход") {
            sizer->Add(buttons_sizer, 0, wxEXPAND);
            sizer->AddStretchSpacer();
            sizer->Add(btn, 0, wxALIGN_CENTER | wxBOTTOM | wxLEFT | wxRIGHT, 20);
        }
        else {
            buttons_sizer->Add(btn, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 20);
        }
    }

    this->SetSizerAndFit(sizer);
}

void ControlPanel::BindButtons(vector<wxCommandEventFunction>& functions)
{
    wxWindow* top = this->GetParent();
    while (!top->IsTopLevel()) {
        top = top->GetParent();
    }

    for (size_t i = 0; i < functions.size(); i++) {
        wxButton* btn = dynamic_cast<wxButton*>(this->GetChildren()[i]);
        btn->Bind(wxEVT_BUTTON, functions[i], top);
    }
}

void ControlPanel::EnableButtonsSaveAndTest()
{
    for (wxWindow* child : this->GetChildren()) {
        wxButton* btn = dynamic_cast<wxButton*>(child);
        if (btn->GetLabel().ToStdString()._Equal("сохранить") || btn->GetLabel().ToStdString()._Equal("тестировать")) {
            btn->Enable();
        }
    }
}

void ControlPanel::EnableLastBtn()
{
    for (wxWindow* child : this->GetChildren()) {
        wxButton* btn = dynamic_cast<wxButton*>(child);
        if (btn->GetLabel().ToStdString()._Equal("сравнить")) {
            btn->Enable();
        }
    }
}
