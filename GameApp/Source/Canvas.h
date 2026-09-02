#pragma once

#include <wx/glcanvas.h>
#include "FontSystem.h"
#include "HappyMath/Rectangle.h"

class Canvas : public wxGLCanvas
{
public:
	Canvas(wxWindow* parent);
	virtual ~Canvas();

private:
	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnMouseMotion(wxMouseEvent& event);

	wxGLContext* context;
	static int attributeList[];
	FontSys::System fontSystem;
	HappyMath::Rectangle worldRect;
};