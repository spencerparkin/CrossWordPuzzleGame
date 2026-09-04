#pragma once

#include <wx/app.h>
#include "CrossWord/PuzzleMatrix.h"
#include "HappyMath/Rectangle.h"
#include "HappyMath/Vector2.h"
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
	bool GetWordLocation(const HappyMath::Vector2& worldPos, CrossWord::WordOrientation wordOrientation, CrossWord::WordLocation& wordLocation, HappyMath::Rectangle& wordRect) const;
	static void LocationToRect(const CrossWord::Location& location, HappyMath::Rectangle& rect);
	bool GetWordHint(const CrossWord::WordLocation& wordLocation, std::string& hint) const;
	void SetWordHint(const CrossWord::WordLocation& wordLocation, const std::string& hint);

	CrossWord::PuzzleMatrix userMatrix;
	CrossWord::PuzzleMatrix solvedMatrix;
	std::vector<CrossWord::WordLocation> wordLocationArray;
	std::unordered_map<std::string, std::string> wordHintMap;
};