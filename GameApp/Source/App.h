#pragma once

#include <wx/app.h>
#include <memory>
#include "CrossWord/WordBank.h"
#include "Puzzle.h"

class Frame;
class Puzzle;

class GameApp : public wxApp
{
public:
	GameApp();
	virtual ~GameApp();

	virtual bool OnInit() override;
	virtual int OnExit() override;

	CrossWord::WordBank* GetWordBank();
	Frame* GetFrame();

	std::shared_ptr<Puzzle> puzzle;

private:
	Frame* frame;

	CrossWord::WordBank wordBank;
};

wxDECLARE_APP(GameApp);