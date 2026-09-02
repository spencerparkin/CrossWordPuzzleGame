#include "App.h"
#include "Frame.h"

wxIMPLEMENT_APP(GameApp);

GameApp::GameApp()
{
	this->frame = nullptr;
}

/*virtual*/ GameApp::~GameApp()
{
}

/*virtual*/ bool GameApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	this->frame = new Frame(wxSize(1600, 1200));
	this->frame->Show(true);

	return true;
}

/*virtual*/ int GameApp::OnExit()
{
	return 0;
}