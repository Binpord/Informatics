#include "wx/wx.h"
#include "wx/sizer.h"
 
/*
 * =====================================
 * GUI classes
 * =====================================
 */

class DrawPane : public wxPanel
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
		wxClientDC dc(this);
		dc.SetPen(wxPen(wxColor(0, 0, 0), 10));
		wxPoint pos = event.GetLogicalPosition(dc);
		dc.DrawPoint(pos);
	}
}

void DrawPane::mouseDown(wxMouseEvent& event)
{
	wxClientDC dc(this);
	dc.SetPen(wxPen(wxColor(0, 0, 0), 10));
	wxPoint pos = event.GetLogicalPosition(dc);
	dc.DrawPoint(pos);
}

// Refresh on right-button click
void DrawPane::rightClick(wxMouseEvent& event)
{
	Refresh();
}
