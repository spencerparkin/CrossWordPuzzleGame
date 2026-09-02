#include "Frame.h"

Frame::Frame(const wxSize& size) : wxFrame(nullptr, wxID_ANY, "Cross-World Puzzle Game", wxDefaultPosition, size)
{
}

/*virtual*/ Frame::~Frame()
{
}