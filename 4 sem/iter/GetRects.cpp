#include "std.h"
#include "RCList.h"
#include <wx/string.h>		// for wxString
#include <wx/image.h>		// for wxImage
#include <wx/gdicmn.h>		// for wxRect and wxPoint
#include <wx/colour.h>		// for wxColour

#ifndef __DEBUG
	#define NDEBUG		// for assert to stop
#endif

void GetRects(const wxImage&, const wxRect&, CList<Image>*, const wxColour&);
wxRect* AddRect(const wxImage&, const wxPoint&, CList<Image>*, const wxColour&);

int main()
{
	wxInitAllImageHandlers();
	// Getting input filename and opening image. Note that I open only jpeg files.
	std::cout << "Please, enter path to your image. Note that this programm assumes that it is a png image." << std::endl;
	std::string filename;
	std::cin >> filename;
	wxImage in_image(filename, wxBITMAP_TYPE_PNG);

	// Finding rects in the image. Assuming that they are not intercepting and neither located one under the other.
	CList<Image> storage;
	// TODO: if will have time - make histogramm of colors and use output to choose background. Don't use constant values!!!
	wxColour Background(255, 255, 255);
	GetRects(in_image, wxRect(0, 0, in_image.GetWidth(), in_image.GetHeight()), &storage, Background);
	// TODO: handler + rects usage
	
	return 0;
}

// start_x, start_y, Width and Height defining of the image part the function will scan
void GetRects(const wxImage& image, const wxRect& part, CList<Image>* storage, const wxColour& Background)
{
	wxRect* AddStatus;
	int pWidth = part.GetWidth();
	int pHeight = part.GetHeight();
	int part_left = part.GetLeft();
	int part_top = part.GetTop();

	// horizontal slice
	for(int x = part_left; x < pWidth; x++)
	{
		// vertical slice
		for(int y = part_top; y < pHeight; y++)
		{
			if(image.GetRed(x, y) != Background.Red() || image.GetGreen(x, y) != Background.Green() || image.GetBlue(x, y) != Background.Blue())
			{
				wxPoint(x, y);
				AddStatus = AddRect(image, wxPoint(x, y), storage, Background);
#ifdef __DEBUG
				if(AddStatus == NULL)
				{
					std::cerr << "Sorry, error in adding/detecting rect occupied." << std::endl;
					assert(false);
				}
#endif
				// Check, whether was rects above the one we added.
				// Using wxRect(const wxPoint& topLeft, const wxPoint& bottomRight)
				GetRects(image, wxRect(wxPoint(x, part.GetTop()), wxPoint(x + AddStatus->GetWidth(), y + AddStatus->GetHeight())), storage, Background);
				// Check, whether was rects beneath the one we added.
				GetRects(image, wxRect(wxPoint(x, y), wxPoint(x + AddStatus->GetWidth(), part.GetBottom())), storage, Background);
				x += AddStatus->GetWidth();
				delete AddStatus;
				// The recursion will stop when there will be no rects in the part.
				break;			// Going forward in horizontal slice.
			}
		}
	}
}

wxRect* AddRect(const wxImage& image, const wxPoint& bottomLeft, CList<Image>* storage, const wxColour& Background) 
{
	// TODO: handle lines with width. Is it necessary (?) . No if you don't handle intercepting rects.
	// Supposing that all lines have same width.
	int Height = 0, Width = 0;

	for(int x = bottomLeft.x; image.GetRed(x, bottomLeft.y) != Background.Red() 
					&& image.GetGreen(x, bottomLeft.y) != Background.Green() 
					&& image.GetBlue(x, bottomLeft.y) != Background.Blue(); x++)
	{
		Width++;
	}

	for(int y = bottomLeft.y; image.GetRed(bottomLeft.x, y) != Background.Red()
					&& image.GetGreen(bottomLeft.x, y) != Background.Green()
					&& image.GetBlue(bottomLeft.x, y) != Background.Blue(); y++)
	{
		Height++;
	}

	// TODO: add different colours.
	storage->add(Image(Width, Height, wxColour(0, 0, 0)));

	wxRect *Res = new wxRect(bottomLeft.x, bottomLeft.y, Width, Height);
	return Res;
}
