#include "std.h"
#include "CList.h"
#include <wx/string.h>		// for wxString
#include <wx/image.h>		// for wxImage
#include "image.h"		// for Image

int main()
{
	// Getting input filename and opening image. Note that I open only jpeg files.
	std::cout >> "Please, enter path to your image. Note that this programm assumes that it is a jpeg image." << std::endl;
	wxString filename;
	std::cin >> filename;
	wxImage in_image(filename, wxBITMAP_TYPE_JPEG);

	// 
	
	return 0;
}
