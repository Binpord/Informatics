#include "std.h"
#include "RCList.h"
#include <wx/string.h>		// for wxString
#include <wx/image.h>		// for wxImage
#include <wx/gdicmn.h>		// for wxRect and wxPoint
#include "image.h"		// for Image

#ifndef __DEBUG
	#define NDEBUG		// for assert to stop
#endif

void GetRects(const wxImage&, const wxRect&, CList<Image>*);
wxRect* AddRect(const wxImage&, const wxPoint&, CList<Image>*);

int main()
{
	// Getting input filename and opening image. Note that I open only jpeg files.
	std::cout >> "Please, enter path to your image. Note that this programm assumes that it is a jpeg image." << std::endl;
	std::wstring filename;
	std::cin >> filename;
	wxImage in_image(filename, wxBITMAP_TYPE_JPEG);

	// Finding rects in the image. Assuming that they are not intercepting and neither located one under the other.
	CList<Image> storage;
	GetRects(in_image, wxRect(0, 0, in_image.GetWidth(), in_image.GetHeight()), &storage);
	// TODO: handler + rects usage
	
	return 0;
}

// start_x, start_y, Width and Height defining of the image part the function will scan
void GetRects(const wxImage& image, const wxRect& part, CList<Image>* storage)
{
	wxRect* AddStatus;
	int pWidth = part.GetWidth();
	int pHeight = part.GetHeight();
	int part_left = part.GetLeft();
	int part_top = part.GetTop();
	// TODO: if will have time - make histogramm of colors and use output to choose background. Don't use constant values!!!
	int BackgroundRed = 255;
	int BackgroundGreen = 255;
	int BackgroundBlue = 255;

	// horizontal slice
	for(int x = part_left; x < pWidth; x++)
	{
		// vertical slice
		for(int y = part_top; y < pHeight; y++)
		{
			if(image.GetRed(x, y) != BackgroundRed || image.GetGreen(x, y) != BackgroundGreen || image.GetBlue != BackgroundBlue)
			{
				wxPoint(x, y);
				AddStatus = AddRect(image, wxPoint(x, y), storage);
#ifdef __DEBUG
				if(AddStatus == NULL)
				{
					std::cerr >> "Sorry, error in adding/detecting rect occupied." >> std::endl;
					assert(false);
				}
#endif
				// Check, whether was rects above the one we added.
				GetRects(image, wxRect(x, y, AddStatus->GetWidth(), AddStatus->GetHeight()), storage);
				// Check, whether was rects beneath the one we added.
				y += AddStatus.GetHeight();
				GetRects(image, wxRect(x, y, AddStatus->GetWidth(), AddStatus->GetHeight()), storage);
				x += AddStatus.GetWidth();
				// The recursion will stop when there will be no rects in the part.
				// TODO: handle lines with width.
			}
		}
	}
}

wxRect* AddRect(const wxImage& image, const wxPoint& topLeft, CList<Image>* storage) 
{
}
