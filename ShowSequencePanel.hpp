#pragma once

#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/button.h>

#include <vector>
#include <string>

using std::vector;
using std::string;

class ShowSequencePanel : public wxScrolledWindow
{
public:
	ShowSequencePanel(wxWindow* parent, vector<bool> sequence);
};