#pragma once

#include <wx/wx.h>

#include "MainFrame.h"

class App : public wxApp
{
private:
	MainFrame* mainFrame;

public:
	App();

	bool OnInit();
};

wxIMPLEMENT_APP(App);