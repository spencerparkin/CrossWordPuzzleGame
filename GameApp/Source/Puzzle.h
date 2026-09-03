#pragma once

#include <wx/app.h>
#include "CrossWord/PuzzleMatrix.h"
#include "HappyMath/Rectangle.h"
#include "FontSystem.h"
#include <unordered_map>

class Puzzle
{
public:
	Puzzle();
	virtual ~Puzzle();

	struct Params
	{
		Params();

		int numRows;
		int numCols;
		int randomNumberSeed;
		int minWordLength;
		int maxWordLength;
		bool symmetric;
	};

	bool Regenerate(const Params& params);
	void GetWorldRect(HappyMath::Rectangle& worldRect) const;
	void Render(FontSys::System* fontSystem) const;
	void ShowSolution();
	void Reset();
	bool GetWordHint(const std::string& word, std::string& hint) const;
	void SetWordHint(const std::string& word, const std::string& hint);
	bool SetWordHintFromJson(const std::string& word, const std::string& jsonText);

	CrossWord::PuzzleMatrix userMatrix;
	CrossWord::PuzzleMatrix solvedMatrix;
	std::vector<CrossWord::WordLocation> wordLocationArray;
	std::unordered_map<std::string, std::string> wordHintMap;
};