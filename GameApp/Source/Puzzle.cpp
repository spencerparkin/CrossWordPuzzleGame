#include "Puzzle.h"
#include "App.h"
#include "CrossWord/Random.h"
#include "CrossWord/PuzzleGenerator.h"
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

	return true;
}

void Puzzle::GetWorldRect(HappyMath::Rectangle& worldRect) const
{
	int numRows = this->userMatrix.GetNumRows();
	int numCols = this->userMatrix.GetNumCols();

	worldRect.minCorner.x = 0.0;
	worldRect.maxCorner.x = double(numCols);
	worldRect.minCorner.y = -double(numRows);
	worldRect.maxCorner.y = 0.0;
}

bool Puzzle::GetWordLocation(const HappyMath::Vector2& worldPos, CrossWord::WordOrientation wordOrientation, CrossWord::WordLocation& wordLocation, HappyMath::Rectangle& wordRect) const
{
	for (int i = 0; i < (int)this->wordLocationArray.size(); i++)
	{
		wordLocation = this->wordLocationArray[i];
		if (wordLocation.orientation != wordOrientation)
			continue;

		wordRect.MakeInvalid();
		for (int j = 0; j < wordLocation.length; j++)
		{
			CrossWord::Location location = wordLocation.GetLocationAt(j);
			HappyMath::Rectangle letterRect;
			LocationToRect(location, letterRect);
			wordRect.ExpandToIncludeRect(letterRect);
		}

		if (wordRect.ContainsPoint(worldPos))
			return true;
	}

	wordRect.MakeInvalid();
	return false;
}

bool Puzzle::GetWordHint(const CrossWord::WordLocation& wordLocation, std::string& hint) const
{
	std::string word = this->solvedMatrix.GetWordAt(wordLocation);
	auto pair = this->wordHintMap.find(word);
	if (pair == this->wordHintMap.end())
		return false;

	hint = pair->second;
	return true;
}

void Puzzle::SetWordHint(const CrossWord::WordLocation& wordLocation, const std::string& hint)
{
	std::string word = this->solvedMatrix.GetWordAt(wordLocation);
	this->wordHintMap.erase(word);
	this->wordHintMap.insert(std::pair(word, hint));
}

/*static*/ void Puzzle::LocationToRect(const CrossWord::Location& location, HappyMath::Rectangle& rect)
{
	rect.minCorner.x = double(location.col);
	rect.maxCorner.x = double(location.col + 1);
	rect.minCorner.y = -double(location.row + 1);
	rect.maxCorner.y = -double(location.row);
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

	glLineWidth(1.0f);

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