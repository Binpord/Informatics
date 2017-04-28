#include "std.h"
#include "RCList.h"
#include "ImageRect.h"
#include <wx/string.h>		// for wxString
#include <wx/image.h>		// for wxImage
#include <wx/gdicmn.h>		// for wxRect and wxPoint
#include <wx/colour.h>		// for wxColour

#ifndef __DEBUG
	#define NDEBUG		// for assert to stop
#endif

void GetRects(wxImage&, CList<ImageRect>*, const wxColour&);
void AddRect(wxImage&, const wxPoint&, CList<ImageRect>*, const wxColour&);

int main()
{
	wxInitAllImageHandlers();
	// Getting input filename and opening image. Note that I open only jpeg files.
	std::cout << "Please, enter path to your image. Note that this programm assumes that it is a png image." << std::endl;
	std::string filename;
	std::cin >> filename;
	wxImage image(filename, wxBITMAP_TYPE_PNG);
	wxString wxfilename = filename;

	// Finding rects in the image. Assuming that they are not intercepting and neither located one under the other.
	CList<ImageRect> storage;
	// TODO: if will have time - make histogramm of colors and use output to choose background. Don't use constant values!!!
	wxColour Background(255, 255, 255);
	GetRects(image, &storage, Background);
	// TODO: rects usage (colour them + save in the other file)
	CList<ImageRect>::Iterator i;
	for(i = storage.begin(); i != storage.end(); i++)
	{
		wxColour colour = (*i).GetFill();
		image.SetRGB(*i, colour.Red(), colour.Green(), colour.Blue());
		image.SaveFile("dbg.png", wxBITMAP_TYPE_PNG);
	}
	wxColour colour = (*i).GetFill();
	image.SetRGB(*i, colour.Red(), colour.Green(), colour.Blue());

	wxfilename = wxfilename.Mid(0, wxfilename.Find('.')) + "_result.png";
	image.SaveFile(wxfilename, wxBITMAP_TYPE_PNG);
	
	return 0;
}

// start_x, start_y, Width and Height defining of the image part the function will scan
void GetRects(wxImage& image, CList<ImageRect>* storage, const wxColour& Background)
{
	int imHeight = image.GetHeight(), imWidth = image.GetWidth();

	// horizontal slice
	for(int x = 0; x < imWidth; x++)
	{
		// vertical slice
		for(int y = 0; y < imHeight; y++)
		{
			if(image.GetRed(x, y) != Background.Red() || image.GetGreen(x, y) != Background.Green() || image.GetBlue(x, y) != Background.Blue())
			{
				wxPoint(x, y);
				AddRect(image, wxPoint(x, y), storage, Background);
			}
		}
	}
}

void AddRect(wxImage& image, const wxPoint& bottomLeft, CList<ImageRect>* storage, const wxColour& Background) 
{
	int Height = 0, Width = 0;

	for(int x = bottomLeft.x; image.GetRed(x, bottomLeft.y) != Background.Red() 
					|| image.GetGreen(x, bottomLeft.y) != Background.Green() 
					|| image.GetBlue(x, bottomLeft.y) != Background.Blue(); x++)
	{
		Width++;
	}

	for(int y = bottomLeft.y; image.GetRed(bottomLeft.x, y) != Background.Red()
					|| image.GetGreen(bottomLeft.x, y) != Background.Green()
					|| image.GetBlue(bottomLeft.x, y) != Background.Blue(); y++)
	{
		Height++;
	}

	// TODO: add different colours.
	// Note that big strange troubles in wxRect(const wxPoint& topLeft, const wxPoint& bottomRight) is forsing me to 
	// use it like wxRect(wxPoint(x, y - 1), wxPoint(x - 1, y)). Otherwise it sets width and height up by one.
	ImageRect result(wxPoint(bottomLeft.x, bottomLeft.y + Height - 1), wxPoint(bottomLeft.x + Width - 1, bottomLeft.y), wxColour(50, 50, 50));
	storage->add(result);
	image.SetRGB(result, Background.Red(), Background.Green(), Background.Blue());
	image.SaveFile("dbg.png", wxBITMAP_TYPE_PNG);
}
