#pragma once

#include <wx/frame.h>

class Canvas;

enum
{
	ID_NewPuzzle,
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

private:
	void OnNewPuzzle(wxCommandEvent& event);
	void OnSolvePuzzle(wxCommandEvent& event);
	void OnResetPuzzle(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnUpdateUI(wxUpdateUIEvent& event);

	Canvas* canvas;
};