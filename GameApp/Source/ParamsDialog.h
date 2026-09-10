#pragma once

#include <wx/dialog.h>
#include "Puzzle.h"

class ParamsDialog : public wxDialog
{
public:
	ParamsDialog(wxWindow* parent, Puzzle::Params* params);
	virtual ~ParamsDialog();

private:
	void OnOkayButtonPushed(wxCommandEvent& event);
	void OnCancelButtonPushed(wxCommandEvent& event);

	Puzzle::Params* params;
};