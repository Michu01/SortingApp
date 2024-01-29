#include "App.h"

App::App()
{
	mainFrame = new MainFrame();
	mainFrame->SetClientSize(1200, 800);
}

bool App::OnInit()
{
	mainFrame->Show();
	
	return true;
}
