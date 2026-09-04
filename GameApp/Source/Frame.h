#pragma once

#include <wx/frame.h>
#include <wx/textctrl.h>

class Canvas;

enum
{
	ID_NewPuzzle = wxID_HIGHEST,
	ID_SolvePuzzle,
	ID_ResetPuzzle,
	ID_Exit,
	ID_About
};

class Frame : public wxFrame
{
public:
	Frame(const wxSize& size);
	virtual ~Frame();

	void ShowWordHint(const std::string& hint);

private:
	void OnNewPuzzle(wxCommandEvent& event);
	void OnSolvePuzzle(wxCommandEvent& event);
	void OnResetPuzzle(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnUpdateUI(wxUpdateUIEvent& event);

	Canvas* canvas;
	wxTextCtrl* hintText;
};