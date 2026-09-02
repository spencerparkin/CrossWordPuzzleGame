#include "App.h"
#include "Frame.h"
#include <wx/progdlg.h>

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

CrossWord::WordBank* GameApp::GetWordBank()
{
	if (this->wordBank.GetNumWords() == 0)
	{
		wxProgressDialog progressDialog("Loading", "Loading word bank...", 1000, this->frame, wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT);

		// STPTODO: Don't use absolute path.
		this->wordBank.Load(R"(D:\git_repos\CrossWordPuzzleGame\GameApp\Words.txt)", [&progressDialog](double progress) -> bool
			{
				int value = int(progress * 1000.0);
				progressDialog.Update(value);
				return progressDialog.WasCancelled();
			});
	}

	return &this->wordBank;
}

/*virtual*/ int GameApp::OnExit()
{
	return 0;
}