#include "ParamsDialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/valgen.h>
#include <wx/valnum.h>
#include <wx/button.h>

ParamsDialog::ParamsDialog(wxWindow* parent, Puzzle::Params* params) : wxDialog(parent, wxID_ANY, "Puzzle Parameters")
{
	this->params = params;

	// Note that if the dimension are too big, we can get a stack overflow.  50x50 seems fine.
	// It might be a good idea to write a non-recursive version of the generator.
	// That way, we can just provide a cancel button if it's taking too long.

	// STPTODO: If the maximum word length is too long, we'll hang.  10 seems fine.
	//          In any case, there should probably be a cancel button if generation is taking too long.

	wxTextCtrl* numRowsCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxIntegerValidator<int>(&this->params->numRows, 3, 50));
	wxTextCtrl* numColsCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxIntegerValidator<int>(&this->params->numCols, 3, 50));
	wxTextCtrl* minWordLengthCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxIntegerValidator<int>(&this->params->minWordLength, 3, 10));
	wxTextCtrl* maxWordLengthCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxIntegerValidator<int>(&this->params->maxWordLength, 3, 10));
	wxTextCtrl* randomNumSeedCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxIntegerValidator<int>(&this->params->randomNumberSeed, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));
	wxCheckBox* symmetryCtrl = new wxCheckBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&this->params->symmetric));

	wxStaticText* numRowsLabel = new wxStaticText(this, wxID_ANY, "Rows:");
	wxStaticText* numColsLabel = new wxStaticText(this, wxID_ANY, "Columns:");
	wxStaticText* minWordLengthLabel = new wxStaticText(this, wxID_ANY, "Min. Word Length:");
	wxStaticText* maxWordLengthLabel = new wxStaticText(this, wxID_ANY, "Max. Word Length:");
	wxStaticText* randomNumSeedLabel = new wxStaticText(this, wxID_ANY, "Random Seed:");
	wxStaticText* symmetryLabel = new wxStaticText(this, wxID_ANY, "Symmetry:");

	wxFlexGridSizer* gridSizer = new wxFlexGridSizer(6, 2, 2, 2);
	gridSizer->Add(numRowsLabel);
	gridSizer->Add(numRowsCtrl, 1, wxGROW);
	gridSizer->Add(numColsLabel);
	gridSizer->Add(numColsCtrl, 1, wxGROW);
	gridSizer->Add(minWordLengthLabel);
	gridSizer->Add(minWordLengthCtrl, 1, wxGROW);
	gridSizer->Add(maxWordLengthLabel);
	gridSizer->Add(maxWordLengthCtrl, 1, wxGROW);
	gridSizer->Add(randomNumSeedLabel);
	gridSizer->Add(randomNumSeedCtrl, 1, wxGROW);
	gridSizer->Add(symmetryLabel);
	gridSizer->Add(symmetryCtrl, 1, wxGROW);
	gridSizer->AddGrowableCol(1, 1);	

	wxButton* okayButton = new wxButton(this, wxID_ANY, "Okay");
	okayButton->Bind(wxEVT_BUTTON, &ParamsDialog::OnOkayButtonPushed, this);

	wxButton* cancelButton = new wxButton(this, wxID_ANY, "Cancel");
	cancelButton->Bind(wxEVT_BUTTON, &ParamsDialog::OnCancelButtonPushed, this);

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->AddStretchSpacer();
	buttonSizer->Add(okayButton);
	buttonSizer->Add(cancelButton);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(gridSizer, 1, wxGROW | wxALL, 5);
	mainSizer->Add(buttonSizer, 0, wxGROW | wxALL, 5);
	this->SetSizer(mainSizer);

	this->TransferDataToWindow();
}

/*virtual*/ ParamsDialog::~ParamsDialog()
{
}

void ParamsDialog::OnOkayButtonPushed(wxCommandEvent& event)
{
	this->TransferDataFromWindow();

	this->EndModal(wxID_OK);
}

void ParamsDialog::OnCancelButtonPushed(wxCommandEvent& event)
{
	this->EndModal(wxID_CANCEL);
}