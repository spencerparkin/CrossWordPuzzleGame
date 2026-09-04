#pragma once

#include <wx/glcanvas.h>
#include <wx/timer.h>
#include <wx/webrequest.h>
#include "FontSystem.h"
#include "HappyMath/Rectangle.h"
#include "CrossWord/PuzzleMatrix.h"
#include <map>

class Canvas : public wxGLCanvas
{
public:
	Canvas(wxWindow* parent);
	virtual ~Canvas();

private:
	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseClick(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnTimer(wxTimerEvent& evnet);
	void OnWebRequestState(wxWebRequestEvent& event);

	bool ConstructWordHintFromJson(const std::string& jsonText, std::string& hint);

	HappyMath::Vector2 MousePosToWorldPos(const wxPoint& mousePos) const;

	void RenderRectOutline(const HappyMath::Rectangle& rect, double r, double g, double b);

	wxTimer timer;
	wxGLContext* context;
	static int attributeList[];
	FontSys::System fontSystem;
	HappyMath::Rectangle worldRect;
	HappyMath::Rectangle adjustedWorldRect;
	HappyMath::Rectangle hoverRect;
	HappyMath::Rectangle selectedRect;
	CrossWord::WordLocation hoverLocation;
	CrossWord::WordOrientation hoverOrienation;
	CrossWord::WordLocation selectedLocation;
	int letterIndex;
	std::map<int, CrossWord::WordLocation> pendingWordHintMap;
	int nextRequestId;
};