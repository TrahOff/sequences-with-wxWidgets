#include "FrameManager.hpp"

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame* frame = new MyFrame("hello world", wxDefaultPosition, wxSize(1400, 700));
	frame->Show(true);
	return true;
}