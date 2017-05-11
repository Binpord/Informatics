#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
 
// Was forced to use global varible, as I need to initialize it before first DrawPane emergence.
// Need this because DrawPane(wxFrame*) will be allocating memory and, hence, need to delete it,
// 	if it was there.
wxBitmap* input = NULL;

/*
 * =====================================
 * GUI classes
 * =====================================
 */

class DrawPane: public wxPanel
{
public:
	DrawPane(wxFrame* parent);
 
	// events
	void paintEvent(wxPaintEvent& evt);
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
 
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DrawPane, wxPanel)

	EVT_MOTION(DrawPane::mouseMoved)
	EVT_LEFT_DOWN(DrawPane::mouseDown)
	EVT_RIGHT_DOWN(DrawPane::rightClick)
	EVT_PAINT(DrawPane::paintEvent)
 
END_EVENT_TABLE()
 
class MyApp: public wxApp
{
	bool OnInit();
 
	wxFrame *frame;
	DrawPane * drawPane;
};
 
/*
 * =====================================
 * MyApp part
 * =====================================
 */

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	frame = new wxFrame((wxFrame*)NULL, -1,  wxT("Calculator"), wxPoint(50, 50), wxSize(800, 600));
 
	drawPane = new DrawPane((wxFrame*)frame);
	sizer->Add(drawPane, 1, wxEXPAND);
 
	frame->SetSizer(sizer);
	frame->SetAutoLayout(true);
 
	frame->Show();
	return true;
} 

/*
 * =====================================
 * DrawPane part
 * =====================================
 */

// -------------------------------------
// Events
// -------------------------------------

// Called by wxWidgets when the panel needs to be redrawn.
// Can also be triggeres by calling Refresh()/Update().
DrawPane::DrawPane(wxFrame* parent):wxPanel(parent)
{
	if(input)
		delete input;

	input = new wxBitmap(parent->GetClientSize());
	wxMemoryDC dc(*input);
	dc.SetBackground(*wxBLACK_BRUSH);
	dc.Clear();	// sets input bitmap with black color
}
 
void DrawPane::paintEvent(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
}

// Draw on left-button click and drag
void DrawPane::mouseMoved(wxMouseEvent& event)
{
	if(event.Dragging() == true)
	{
		// semimultaniously drawing in wxBitmap and on screen
		wxClientDC dc(this);
		wxMemoryDC memdc(*input);
		dc.SetPen(wxPen(wxColor(0, 0, 0), 10));
		memdc.SetPen(wxPen(wxColor(255, 255, 255), 10));
		wxPoint pos = event.GetLogicalPosition(dc);
		dc.DrawPoint(pos);
		memdc.DrawPoint(pos);
	}
}

void DrawPane::mouseDown(wxMouseEvent& event)
{
	// semimultaniously drawing in wxBitmap and on screen
	wxClientDC dc(this);
	wxMemoryDC memdc(*input);
	dc.SetPen(wxPen(wxColor(0, 0, 0), 10));
	memdc.SetPen(wxPen(wxColor(255, 255, 255), 10));
	wxPoint pos = event.GetLogicalPosition(dc);
	dc.DrawPoint(pos);
	memdc.DrawPoint(pos);
}

// Refresh on right-button click
void DrawPane::rightClick(wxMouseEvent& event)
{
	Refresh();
}
