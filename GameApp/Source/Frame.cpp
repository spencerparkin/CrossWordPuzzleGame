#include "Frame.h"
#include "Canvas.h"
#include "App.h"
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/aboutdlg.h>
#include <wx/msgdlg.h>

Frame::Frame(const wxSize& size) : wxFrame(nullptr, wxID_ANY, "Crossword Puzzle Game", wxDefaultPosition, size)
{
	wxMenu* puzzleMenu = new wxMenu();
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_NewPuzzle, wxT("New Puzzle"), wxT("Generate a new cross-word puzzle.")));
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_SolvePuzzle, wxT("Solve Puzzle"), wxT("Reveal the solution to the current puzzle.")));
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_ResetPuzzle, wxT("Reset Puzzle"), wxT("Reset the puzzle to its initial state.")));
	puzzleMenu->AppendSeparator();
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_Exit, wxT("Exit"), wxT("Go do something else with your life.")));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(new wxMenuItem(helpMenu, ID_About, wxT("About"), wxT("Show the about box.")));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(puzzleMenu, wxT("Puzzle"));
	menuBar->Append(helpMenu, wxT("Help"));
	this->SetMenuBar(menuBar);

	wxStatusBar* statusBar = new wxStatusBar(this);
	this->SetStatusBar(statusBar);

	this->canvas = new Canvas(this);

	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(this->canvas, 1, wxGROW | wxALL, 0);
	this->SetSizer(boxSizer);

	this->Bind(wxEVT_MENU, &Frame::OnNewPuzzle, this, ID_NewPuzzle);
	this->Bind(wxEVT_MENU, &Frame::OnSolvePuzzle, this, ID_SolvePuzzle);
	this->Bind(wxEVT_MENU, &Frame::OnResetPuzzle, this, ID_ResetPuzzle);
	this->Bind(wxEVT_MENU, &Frame::OnAbout, this, ID_About);
	this->Bind(wxEVT_MENU, &Frame::OnExit, this, ID_Exit);
	this->Bind(wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_SolvePuzzle);
	this->Bind(wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_ResetPuzzle);
}

/*virtual*/ Frame::~Frame()
{
}

void Frame::OnNewPuzzle(wxCommandEvent& event)
{
	wxGetApp().puzzle = std::make_shared<Puzzle>();

	Puzzle::Params params;
	// STPTODO: Show dialog here to configure parameters.

	if (!wxGetApp().puzzle->Regenerate(params))
	{
		wxMessageBox("Failed to generate puzzle!", "Error!", wxICON_ERROR | wxOK, this);
		wxGetApp().puzzle = nullptr;
	}

	this->canvas->Refresh();
}

void Frame::OnSolvePuzzle(wxCommandEvent& event)
{
	if (wxGetApp().puzzle.get())
	{
		wxGetApp().puzzle->ShowSolution();

		this->canvas->Refresh();
	}
}

void Frame::OnResetPuzzle(wxCommandEvent& event)
{
	if (wxGetApp().puzzle.get())
	{
		wxGetApp().puzzle->Reset();

		this->canvas->Refresh();
	}
}

void Frame::OnExit(wxCommandEvent& event)
{
	this->Close(true);
}

void Frame::OnUpdateUI(wxUpdateUIEvent& event)
{
	switch (event.GetId())
	{
		case ID_SolvePuzzle:
		case ID_ResetPuzzle:
		{
			event.Enable(wxGetApp().puzzle.get() != nullptr);
			break;
		}
	}
}

void Frame::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.SetName(wxT("Crossword Puzzle Game"));
	info.SetVersion(wxT("1.0.0"));
	info.SetDescription(wxT("This is a basic cross-word puzzle application."));
	info.SetCopyright("(C) 2026 Spencer T. Parkin <spencer.parkin@proton.me>");

	wxAboutBox(info);
}