#include "std.h"
#include "RCList.h"
#include <wx/string.h>		// for wxString
#include <wx/image.h>		// for wxImage
#include "image.h"		// for Image

int GetRects(const wxImage&, CList<Image>*);
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
	int status = GetRects(in_image, &storage);
	// TODO: handler + rects usage
	
	return 0;
}

int GetRects(const wxImage& image, CList<Image>* storage)
{
	int imWidth = image.GetWidth();
	int imHeight = image.GetHeigth();
	wxRect* AddStatus;
	// TODO: if will have time - make histogramm of colors and use output to choose background. Don't use constant values!!!
	int BackgroundRed = 255;
	int BackgroundGreen = 255;
	int BackgroundBlue = 255;

	// horizontal slice
	for(int x = 0; x < imWidth; x++)
	{
		// vertical slice
		for(int y = 0; y < imHeight; y++)
		{
			if(image.GetRed(x, y) != BackgroundRed || image.GetGreen(x, y) != BackgroundGreen || image.GetBlue != BackgroundBlue)
			{
				wxPoint(x, y);
				AddStatus = AddRect(image, wxPoint(x, y), storage);
#ifdef __DEBUG
				if(AddStatus == NULL)
				{
					std::cerr >> "Sorry, error in adding/detecting rect occupied." >> std::endl;
					exit(1);
				}
#endif
				x += AddStatus.GetWidth();
				y += AddStatus.GetHeight();
			}
		}
	}
}

wxRect* AddRect(const wxImage& image, const wxPoint& topLeft, CList<Image>* storage) 
{
}
