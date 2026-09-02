#include "Canvas.h"
#include "App.h"
#include "Puzzle.h"
#include <gl/GLU.h>

using namespace HappyMath;

int Canvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

Canvas::Canvas(wxWindow* parent) : wxGLCanvas(parent, wxID_ANY, attributeList)
{
	this->context = new wxGLContext(this);

	this->Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
	this->Bind(wxEVT_SIZE, &Canvas::OnResize, this);
	this->Bind(wxEVT_MOTION, &Canvas::OnMouseMotion, this);

	bool fontSysInit = this->fontSystem.Initialize();
	wxASSERT(fontSysInit);

	this->fontSystem.SetFontBaseDir(R"(D:\git_repos\CrossWordPuzzleGame\GameApp)");		// STPTODO: Fix this.
	this->fontSystem.SetFont("OpenSans-Regular.ttf");
}

/*virtual*/ Canvas::~Canvas()
{
	delete this->context;

	this->fontSystem.Finalize();
}

void Canvas::OnPaint(wxPaintEvent& event)
{
	this->SetCurrent(*this->context);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	const Puzzle* puzzle = wxGetApp().puzzle.get();
	if (puzzle)
	{
		puzzle->GetWorldRect(this->worldRect);

		GLint viewport[4] = { 0, 0, 0, 0 };
		glGetIntegerv(GL_VIEWPORT, viewport);

		double aspectRatio = double(viewport[2]) / double(viewport[3]);
		HappyMath::Rectangle adjustedWorldRect(this->worldRect);
		adjustedWorldRect.ExpandToMatchAspectRatio(aspectRatio);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(
			adjustedWorldRect.minCorner.x,
			adjustedWorldRect.maxCorner.x,
			adjustedWorldRect.minCorner.y,
			adjustedWorldRect.maxCorner.y);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		puzzle->Render(&this->fontSystem);
	}

	glFlush();

	this->SwapBuffers();
}

void Canvas::OnResize(wxSizeEvent& event)
{
	this->SetCurrent(*this->context);

	wxSize size = event.GetSize();
	glViewport(0, 0, size.x, size.y);
}

void Canvas::OnMouseMotion(wxMouseEvent& event)
{
}