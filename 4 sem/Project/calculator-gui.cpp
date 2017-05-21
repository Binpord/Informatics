#include "precompiled_headers.h"
#include "handwritten-recognition.h"
#include "trainSVM.h"

/*
 * =====================================
 * GUI classes
 * =====================================
 */

class DrawPane: public wxPanel
{
protected:
	wxBitmap* input;
	wxPoint last_pos;
	bool is_listing_result;
public:
	DrawPane(wxFrame* parent);

	void ResultOut(const wxString&);
	void ClearPane();
 
	// events
	void paintEvent(wxPaintEvent& evt);
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void resizeEvent(wxSizeEvent& event);
	void onSpaceDown(wxKeyEvent& event);
 
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DrawPane, wxPanel)

	EVT_MOTION(DrawPane::mouseMoved)
	EVT_LEFT_DOWN(DrawPane::mouseDown)
	EVT_RIGHT_DOWN(DrawPane::rightClick)
	EVT_PAINT(DrawPane::paintEvent)
	EVT_SIZE(DrawPane::resizeEvent)
	EVT_KEY_DOWN(DrawPane::onSpaceDown)
 
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

	if(wxFileExists(wxT("svm.yml")) == false)
	{
		wxMessageBox(wxT("Oops. Seems that you don't have ready svm configuration. Programm will try to create it."));
		trainSVM("training_files");
	}
	
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
	input = new wxBitmap(parent->GetClientSize());
	wxMemoryDC memdc(*input);
	memdc.SetBackground(*wxWHITE_BRUSH);
	memdc.Clear();	// sets input bitmap with black color
	is_listing_result = false;
}
 
void DrawPane::paintEvent(wxPaintEvent& evt)
{
	wxMemoryDC memdc(*input);
	wxPaintDC dc(this);
	int width, height;
	this->GetClientSize(&width, &height);
	dc.Blit(0, 0, width, height, &memdc, 0, 0);
}

// Draw on left-button click and drag
void DrawPane::mouseMoved(wxMouseEvent& event)
{
	if(event.Dragging() == true)
	{
		wxClientDC dc(this);
		wxMemoryDC memdc(*input);
		memdc.SetPen(wxPen(wxColor(0, 0, 0), 10));
		wxPoint pos = event.GetLogicalPosition(dc);
		memdc.DrawLine(last_pos, pos);
		Refresh();

		last_pos = pos;
	}
}

void DrawPane::mouseDown(wxMouseEvent& event)
{
	if(is_listing_result == true)
	{
		delete input;
		input = new wxBitmap(this->GetClientSize());
		wxMemoryDC memdc(*input);
		memdc.SetBackground(*wxWHITE_BRUSH);
		memdc.Clear();	// sets input bitmap with white color
		is_listing_result = false;
	}

	wxClientDC dc(this);
	wxMemoryDC memdc(*input);
	memdc.SetPen(wxPen(wxColor(0, 0, 0), 10));
	wxPoint pos = event.GetLogicalPosition(dc);
	memdc.DrawPoint(pos);
	Refresh();
	
	last_pos = pos;
}

// Refresh on right-button click
void DrawPane::rightClick(wxMouseEvent& event)
{
	ClearPane();

	Refresh();
}

void DrawPane::ClearPane()
{
	delete input;
	input = new wxBitmap(this->GetClientSize());
	wxMemoryDC memdc(*input);
	memdc.SetBackground(*wxWHITE_BRUSH);
	memdc.Clear();	// sets input bitmap with white color
}

void DrawPane::resizeEvent(wxSizeEvent& event)
{
	wxBitmap* tmp_input = new wxBitmap(event.GetSize());
	wxMemoryDC src_dc(*input);
	wxMemoryDC dst_dc(*tmp_input);

	dst_dc.SetBackground(*wxWHITE_BRUSH);
	dst_dc.Clear();

	dst_dc.Blit(0, 0, input->GetWidth(), input->GetHeight(), &src_dc, 0, 0);
	delete input;
	input = tmp_input;

	Refresh();
}

void DrawPane::onSpaceDown(wxKeyEvent& event)
{
	// evaluate on space
	if(event.GetKeyCode() == ' ')
	{
		wxString expr = GetExpression(input);
		double result = 0;
		try
		{
			result = Evaluate(expr);
		}
		catch(...)
		{
			ClearPane();
			Refresh();
			return;
		}
		expr += " = ";
		expr << result;
		ResultOut(expr);

		Refresh();
		is_listing_result = true;
	}
}

// -------------------------------------
// Other functions
// -------------------------------------

void DrawPane::ResultOut(const wxString& result)
{
	ClearPane();
	wxMemoryDC memdc(*input);

	memdc.DrawText(result, 0, 0);
}

double Evaluate(const wxString& expr)
{
	exprtk::expression<double> expression;
	exprtk::parser<double> parser;
	std::string expression_str = expr.ToStdString();

	if(parser.compile(expression_str, expression) == 0)
	{
		wxString error_message;
		error_message << "Error in expression \"" << expression_str << "\":\n";
		error_message << parser.error().c_str();
		wxMessageBox(error_message);
		throw;
	}

	return expression.value();
}
