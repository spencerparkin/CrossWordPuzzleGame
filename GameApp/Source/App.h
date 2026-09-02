#pragma once

#include <wx/app.h>

class Frame;

class GameApp : public wxApp
{
public:
	GameApp();
	virtual ~GameApp();

	virtual bool OnInit() override;
	virtual int OnExit() override;

private:
	Frame* frame;
};

wxDECLARE_APP(GameApp);