#pragma once

#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/button.h>

class GenerateFormPanel : public wxScrolledWindow
{
private:
	wxTextCtrl* sequenceLengthCtrl = NULL;
	wxRadioBox* radioBox = NULL;
	wxArrayString generators = { "Квадратичный конгруэнтный генератор", "Генератор BBS", "Yarrow - 160" };
public:
	GenerateFormPanel(wxWindow* parent, wxCommandEventFunction function);

	wxString getLength();
	wxString getGenerator();
};