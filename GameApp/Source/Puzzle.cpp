#include "Puzzle.h"
#include "App.h"
#include "CrossWord/Random.h"
#include "CrossWord/PuzzleGenerator.h"
#include "JsonValue.h"
#include <wx/busycursor.h>

Puzzle::Params::Params()
{
	this->numRows = 20;
	this->numCols = 20;
	this->minWordLength = 3;
	this->maxWordLength = 9;
	this->randomNumberSeed = 0;
	this->symmetric = true;
}

Puzzle::Puzzle()
{
}

/*virtual*/ Puzzle::~Puzzle()
{
}

bool Puzzle::Regenerate(const Params& params)
{
	wxBusyCursor busyCursor;

	CrossWord::Random random;
	random.SetSeed(params.randomNumberSeed);

	this->solvedMatrix.SetSize(params.numRows, params.numCols);

	CrossWord::PuzzleGenerator puzzleGenerator;

	CrossWord::WordBank* wordBank = wxGetApp().GetWordBank();

	if (!puzzleGenerator.Generate(&this->solvedMatrix, wordBank, &random, params.minWordLength, params.maxWordLength, params.symmetric, this->wordLocationArray))
		return false;

	this->Reset();

	// Note that we do not generate the word-hint map here.
	// It is generated later as the puzzle is being used.
	// In part, this is done in an effort to overcome rate-limits
	// with free dictionary APIs.

#if 0
	// STPTODO: How can we make sure that this puzzle class instance stays in scope as long as the requests are in flight?
	for (int i = 0; i < (int)this->wordLocationArray.size(); i++)
	{
		const CrossWord::WordLocation& location = this->wordLocationArray[i];
		std::string word = this->solvedMatrix.GetWordAt(location);

		//wxString url = "https://api.quickpronounce.site/v1/dictionary/" + word;
		wxString url = "https://en.wiktionary.org/api/rest_v1/page/definition/" + word;
		
		wxWebRequest request = wxWebSession::GetDefault().CreateRequest(this, url, i);
		if (!request.IsOk())
			continue;

		request.SetMethod("GET");
		request.SetHeader("Accept", "application/json");
		request.Start();

		WordHintRequest hintRequest;
		hintRequest.request = request;
		hintRequest.word = word;

		this->requestHintArray.push_back(hintRequest);
	}
#endif

	return true;
}

bool Puzzle::GetWordHint(const std::string& word, std::string& hint) const
{
	auto pair = this->wordHintMap.find(word);
	if (pair == this->wordHintMap.end())
		return false;

	hint = pair->second;
	return true;
}

void Puzzle::SetWordHint(const std::string& word, const std::string& hint)
{
	this->wordHintMap.erase(word);
	this->wordHintMap.insert(std::pair(word, hint));
}

bool Puzzle::SetWordHintFromJson(const std::string& word, const std::string& jsonText)
{
	using namespace ParseParty;

	try
	{
		std::string parseError;
		std::shared_ptr<JsonObject> jsonRoot = std::dynamic_pointer_cast<JsonObject>(JsonValue::ParseJson(jsonText, parseError));
		if (!jsonRoot.get())
			return false;

		std::shared_ptr<JsonArray> jsonEnglishArray = jsonRoot->GetValueOrThrow<JsonArray>("en");

		std::string hint;

		for (int i = 0; i < (int)jsonEnglishArray->GetSize(); i++)
		{
			std::shared_ptr<JsonObject> jsonEntry = jsonEnglishArray->GetValueOrThrow<JsonObject>(i);

			std::shared_ptr<JsonArray> jsonDefArray = jsonEntry->GetValueOrThrow<JsonArray>("definitions");

			if (jsonDefArray->GetSize() > 0)
			{
				std::shared_ptr<JsonObject> jsonDef = jsonDefArray->GetValueOrThrow<JsonObject>(0);

				hint = jsonDef->GetValueOrThrow<JsonString>("definition")->GetValue();
			}
		}

		this->SetWordHint(word, hint);
	}
	catch (JsonException jsonExc)
	{
		wxLogError(jsonExc.errorMsg);
		return false;
	}

	return true;
}

#if 0
void Puzzle::OnWebRequestState(wxWebRequestEvent& event)
{
	switch (event.GetState())
	{
		case wxWebRequest::State::State_Completed:
		{
			wxWebResponse response = event.GetRequest().GetResponse();
			if (response.GetStatus() == 200 /* OK */)
			{
				WordHintRequest& hintRequest = this->requestHintArray[event.GetId()];

				std::string jsonResponse = response.AsString().ToStdString();
				this->CaptureWordHint(hintRequest.word, jsonResponse);
			}

			break;
		}
		case wxWebRequest::State::State_Failed:
		{
			wxString error = event.GetErrorDescription();
			wxLogError(error);
			break;
		}
		case wxWebRequest::State::State_Idle:
		{
			break;
		}
		case wxWebRequest::State::State_Unauthorized:
		{
			wxLogError("Unauthorized!");
			break;
		}
		case wxWebRequest::State::State_Active:
		{
			break;
		}
		case wxWebRequest::State::State_Cancelled:
		{
			wxLogError("Canceled!");
			break;
		}
	}
}
#endif

void Puzzle::GetWorldRect(HappyMath::Rectangle& worldRect) const
{
	int numRows = this->userMatrix.GetNumRows();
	int numCols = this->userMatrix.GetNumCols();

	worldRect.minCorner.x = 0.0;
	worldRect.maxCorner.x = double(numCols);
	worldRect.minCorner.y = -double(numRows);
	worldRect.maxCorner.y = 0.0;
}

void Puzzle::Render(FontSys::System* fontSystem) const
{
	int numRows = this->userMatrix.GetNumRows();
	int numCols = this->userMatrix.GetNumCols();

	glBegin(GL_QUADS);

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			unsigned char letter = this->userMatrix.GetLetter(row, col);

			HappyMath::Rectangle letterRect;
			letterRect.minCorner.x = double(col);
			letterRect.maxCorner.x = double(col + 1);
			letterRect.minCorner.y = -double(row);
			letterRect.maxCorner.y = -double(row + 1);

			if (letter == CROSSWORD_ALWAYS_BLANK)
				glColor3f(0.0f, 0.0f, 0.0f);
			else
				glColor3f(1.0f, 1.0f, 1.0f);

			glVertex2d(letterRect.minCorner.x, letterRect.minCorner.y);
			glVertex2d(letterRect.maxCorner.x, letterRect.minCorner.y);
			glVertex2d(letterRect.maxCorner.x, letterRect.maxCorner.y);
			glVertex2d(letterRect.minCorner.x, letterRect.maxCorner.y);
		}
	}

	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);

	for (int row = 1; row < numRows; row++)
	{
		glVertex2d(0.0, -double(row));
		glVertex2d(double(numCols), -double(row));
	}

	for (int col = 1; col < numCols; col++)
	{
		glVertex2d(double(col), 0.0);
		glVertex2d(double(col), -double(numRows));
	}

	glEnd();

	fontSystem->SetLineHeight(0.8f);

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			unsigned char letter = this->userMatrix.GetLetter(row, col);

			if (letter == CROSSWORD_ALWAYS_BLANK || letter == CROSSWORD_LETTER_UNKNOWN)
				continue;

			std::string text;
			text += letter;

			GLfloat length = 0.0f;
			fontSystem->CalcTextLength(text, length);

			float x = float(col) + (1.0f - length) / 2.0f;
			float y = -float(row + 1) + 0.1f;

			fontSystem->RenderText(x, y, text, true);
		}
	}
}

void Puzzle::ShowSolution()
{
	this->userMatrix.Copy(this->solvedMatrix, true);
}

void Puzzle::Reset()
{
	this->userMatrix.Copy(this->solvedMatrix, false);
}