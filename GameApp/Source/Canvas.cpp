#include "Canvas.h"
#include "App.h"
#include "Puzzle.h"
#include <gl/GLU.h>

using namespace HappyMath;

int Canvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

Canvas::Canvas(wxWindow* parent) : wxGLCanvas(parent, wxID_ANY, attributeList), timer(this)
{
	this->context = new wxGLContext(this);

	this->Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
	this->Bind(wxEVT_SIZE, &Canvas::OnResize, this);
	this->Bind(wxEVT_MOTION, &Canvas::OnMouseMotion, this);
	this->Bind(wxEVT_LEFT_DOWN, &Canvas::OnMouseClick, this);
	this->Bind(wxEVT_MOUSEWHEEL, &Canvas::OnMouseWheel, this);
	this->Bind(wxEVT_KEY_DOWN, &Canvas::OnKeyDown, this);
	this->Bind(wxEVT_TIMER, &Canvas::OnTimer, this);

	bool fontSysInit = this->fontSystem.Initialize();
	wxASSERT(fontSysInit);

	this->fontSystem.SetFontBaseDir(R"(D:\git_repos\CrossWordPuzzleGame\GameApp)");		// STPTODO: Fix this.
	this->fontSystem.SetFont("OpenSans-Regular.ttf");

	this->adjustedWorldRect.MakeInvalid();
	this->hoverRect.MakeInvalid();
	this->selectedRect.MakeInvalid();

	this->hoverOrienation = CrossWord::WordOrientation::ACROSS;
	this->letterIndex = 0;

	this->timer.Start(60);
}

/*virtual*/ Canvas::~Canvas()
{
	delete this->context;

	this->fontSystem.Finalize();
}

void Canvas::OnTimer(wxTimerEvent& event)
{
	this->Refresh();
}

void Canvas::OnPaint(wxPaintEvent& event)
{
	this->SetCurrent(*this->context);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	const Puzzle* puzzle = wxGetApp().puzzle.get();
	if (puzzle)
	{
		if (!this->adjustedWorldRect.IsValid())
		{
			puzzle->GetWorldRect(this->worldRect);

			if (this->worldRect.GetArea() > 0)
			{
				GLint viewport[4] = { 0, 0, 0, 0 };
				glGetIntegerv(GL_VIEWPORT, viewport);

				double aspectRatio = double(viewport[2]) / double(viewport[3]);

				this->adjustedWorldRect = this->worldRect;
				this->adjustedWorldRect.ExpandToMatchAspectRatio(aspectRatio);
			}
		}

		if (this->adjustedWorldRect.IsValid())
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(
				this->adjustedWorldRect.minCorner.x,
				this->adjustedWorldRect.maxCorner.x,
				this->adjustedWorldRect.minCorner.y,
				this->adjustedWorldRect.maxCorner.y);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			puzzle->Render(&this->fontSystem);

			if (this->hoverRect.IsValid())
				this->RenderRectOutline(this->hoverRect, 1.0, 0.0, 0.0);

			if (this->selectedRect.IsValid())
			{
				this->RenderRectOutline(this->selectedRect, 0.0, 0.0, 1.0);

				CrossWord::Location location = this->selectedLocation.GetLocationAt(this->letterIndex);

				HappyMath::Rectangle letterRect;
				Puzzle::LocationToRect(location, letterRect);

				this->RenderRectOutline(letterRect, 0.0, 1.0, 0.0);
			}
		}
	}

	glFlush();

	this->SwapBuffers();
}

void Canvas::RenderRectOutline(const HappyMath::Rectangle& rect, double r, double g, double b)
{
	glLineWidth(4.0f);
	glBegin(GL_LINE_LOOP);
	glColor3d(r, g, b);

	glVertex2d(rect.minCorner.x, rect.minCorner.y);
	glVertex2d(rect.maxCorner.x, rect.minCorner.y);
	glVertex2d(rect.maxCorner.x, rect.maxCorner.y);
	glVertex2d(rect.minCorner.x, rect.maxCorner.y);

	glEnd();
}

void Canvas::OnResize(wxSizeEvent& event)
{
	this->SetCurrent(*this->context);

	wxSize size = event.GetSize();
	glViewport(0, 0, size.x, size.y);

	this->adjustedWorldRect.MakeInvalid();
}

void Canvas::OnMouseWheel(wxMouseEvent& event)
{
	int delta = event.GetWheelDelta() / 120;
	if (delta % 2 == 1)
	{
		switch (this->hoverOrienation)
		{
		case CrossWord::WordOrientation::ACROSS:
			this->hoverOrienation = CrossWord::WordOrientation::DOWN;
			break;
		case CrossWord::WordOrientation::DOWN:
			this->hoverOrienation = CrossWord::WordOrientation::ACROSS;
			break;
		}
	}

	const Puzzle* puzzle = wxGetApp().puzzle.get();
	if (!puzzle)
		return;

	HappyMath::Vector2 worldPos = this->MousePosToWorldPos(event.GetPosition());

	puzzle->GetWordLocation(worldPos, this->hoverOrienation, this->hoverLocation, this->hoverRect);
}

HappyMath::Vector2 Canvas::MousePosToWorldPos(const wxPoint& mousePos) const
{
	GLint viewport[4] = { 0, 0, 0, 0 };
	glGetIntegerv(GL_VIEWPORT, viewport);

	HappyMath::Rectangle viewportRect;
	viewportRect.minCorner.x = double(viewport[0]);
	viewportRect.maxCorner.x = double(viewport[0]) + double(viewport[2]);
	viewportRect.minCorner.y = double(viewport[1]);
	viewportRect.maxCorner.y = double(viewport[1]) + double(viewport[3]);

	HappyMath::Vector2 point(mousePos.x, viewport[3] - mousePos.y);
	HappyMath::Vector2 uvs = viewportRect.PointToUVs(point);
	point = this->adjustedWorldRect.PointFromUVs(uvs);

	return point;
}

void Canvas::OnMouseMotion(wxMouseEvent& event)
{
	const Puzzle* puzzle = wxGetApp().puzzle.get();
	if (!puzzle)
		return;

	HappyMath::Vector2 worldPos = this->MousePosToWorldPos(event.GetPosition());

	puzzle->GetWordLocation(worldPos, this->hoverOrienation, this->hoverLocation, this->hoverRect);

	this->Refresh();
}

void Canvas::OnMouseClick(wxMouseEvent& event)
{
	const Puzzle* puzzle = wxGetApp().puzzle.get();
	if (!puzzle)
		return;

	HappyMath::Vector2 worldPos = this->MousePosToWorldPos(event.GetPosition());

	if (!puzzle->GetWordLocation(worldPos, this->hoverOrienation, this->selectedLocation, this->selectedRect))
		return;

	this->letterIndex = 0;

	for (int i = 0; i < this->selectedLocation.length; i++)
	{
		HappyMath::Rectangle rect;
		Puzzle::LocationToRect(this->selectedLocation.GetLocationAt(i), rect);
		if (rect.ContainsPoint(worldPos))
		{
			this->letterIndex = i;
			break;
		}
	}
}

void Canvas::OnKeyDown(wxKeyEvent& event)
{
	// STPTODO: It might be nicer if the arrow keys could also be used to change our word location and orientation.

	int key = event.GetKeyCode();
	switch (key)
	{
		case WXK_LEFT:
		case WXK_UP:
		{
			if (this->letterIndex > 0)
				this->letterIndex--;
			break;
		}
		case WXK_RIGHT:
		case WXK_DOWN:
		{
			if (this->letterIndex < this->selectedLocation.length - 1)
				this->letterIndex++;
			break;
		}
	}

	Puzzle* puzzle = wxGetApp().puzzle.get();
	if (!puzzle)
		return;

	if (key >= 'A' && key <= 'Z')
	{
		CrossWord::Location location = this->selectedLocation.GetLocationAt(this->letterIndex);
		unsigned char letter = (unsigned char)::tolower(key);
		puzzle->userMatrix.SetLetter(location, letter);

		if (this->letterIndex < this->selectedLocation.length - 1)
			this->letterIndex++;
	}
	else if (key == WXK_BACK)
	{
		CrossWord::Location location = this->selectedLocation.GetLocationAt(this->letterIndex);
		puzzle->userMatrix.SetLetter(location, CROSSWORD_LETTER_UNKNOWN);

		if (this->letterIndex > 0)
			this->letterIndex--;
	}
}