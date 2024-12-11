#pragma once

#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/button.h>

#include <vector>
#include <string>

using std::vector;
using std::string;
using std::pair;

class ControlPanel : public wxScrolledWindow
{
private:
	vector<pair<string, bool>> button_info = { 
		{ "генерация", true }, { "загрузить", true }, { "сохранить", false }, 
		{ "тестировать", false }, { "сравнить", false }, { "выход", true }
	};
public:
	ControlPanel(wxWindow* parent);
	void BindButtons(vector<wxCommandEventFunction>& functions);
	void EnableButtonsSaveAndTest();
	void EnableLastBtn();
};