#include "image.h"

Image::Image(string file):wxImage(wxString(file.c_str(), wxConvUTF8), wxBITMAP_TYPE_PNG)
{
	if(!this->IsOk())
	{
		cout << "no file" << endl;
		exit(1);
	}

	w=GetWidth();
	h=GetHeight();
}

Image::Image(int w, int h, wxColor back):wxImage(w,h)
{
	wxPoint one(1, 1),sec(w - 1,h - 1);
	wxRect rec(one, sec);
	this->SetRGB(rec, back.Red(), back.Green(), back.Blue());   
}

void Image::setPen(wxColor a)
{
	pen = a;
}

void Image::setFill(wxColor a)
{
	fill = a;
}

void Image::ColorPoint(wxPoint p, wxColor c)
{
	wxRect rc(p,p);
	this->SetRGB(rc, c.Red(), c.Green(), c.Blue());
}

void Image::FillRec(wxPoint p, wxColor c)
{
	wxColor place(GetRed(p.x, p.y), GetGreen(p.x, p.y), GetBlue(p.x, p.y));
	wxColor check(GetRed(p.x, p.y - 1), GetGreen(p.x, p.y - 1), GetBlue(p.x, p.y - 1));
	int yh, yl, xl, xp;
	int x, y;
//	cout << (place != c) << endl;
	for(y = p.y - 1; place == check; y--)
	{
		check.Set(GetRed(p.x, y),GetGreen(p.x, y),GetBlue(p.x, y));
	}
	yh = y + 1;
	check.Set(GetRed(p.x,p.y + 1), GetGreen(p.x,p.y + 1),GetBlue(p.x,p.y + 1));
	for(y = p.y + 1; place == check; y++)
	{
		check.Set(GetRed(p.x, y), GetGreen(p.x, y), GetBlue(p.x, y));
	}
	yl = y - 1;
	check.Set(GetRed(p.x - 1, p.y), GetGreen(p.x - 1, p.y), GetBlue(p.x - 1, p.y));
	for(x = p.x-1 ; place == check; x--)
	{
		check.Set(GetRed(x, p.y), GetGreen(x, p.y), GetBlue(x, p.y));
	}
	xl = x + 1;
	check.Set(GetRed(p.x + 1, p.y), GetGreen(p.x + 1, p.y), GetBlue(p.x + 1, p.y));
	for(x = p.x + 1; place == check; x++)
	{
		check.Set(GetRed(x, p.y), GetGreen(x, p.y), GetBlue(x, p.y));
	}
	xp = x - 1;

	wxRect rec(wxPoint(xl, yh), wxPoint(xp, yl));
	this->SetRGB(rec, c.Red(), c.Green(), c.Blue()); 
}

void Image::DrawLine(wxPoint one, wxPoint sec)
{
	for(int x = one.x; x < sec.x; x++)
	{
		int y=((sec.y - one.y) * x + (sec.x * one.y - one.x * sec.y)) / (sec.x - one.x);
//		cout<<"x="<<x<<" y="<<y<<endl;
		wxPoint fr(x, y);
		wxRect rec(fr, fr);
		this->SetRGB(rec, pen.Red(), pen.Green(), pen.Blue());
	}
	for(int y = one.y; y < sec.y; y++)
	{
		int x=((sec.x - one.x) * y + (sec.y * one.x - one.y * sec.x)) / (sec.y - one.y);
//		cout<<"x="<<x<<" y="<<y<<endl;
		wxPoint fr(x, y);
		wxRect rec(fr, fr);
		this->SetRGB(rec, pen.Red(), pen.Green(), pen.Blue());
	}
}

void Image::saveToFile(string file)
{
	wxString st1(file.c_str(), wxConvUTF8);
	this->SaveFile(st1, wxBITMAP_TYPE_PNG);
}

Image::~Image()
{
	wxImage::Destroy();
}

// vadim
Image::Image():wxImage()
{
	w = 0;
	h = 0;
	// wxColor has wxColor()
}

bool Image::operator==(const Image& comp)
{
	return (this->w == comp.w && this->h == comp.h && this->pen == comp.pen && this->fill == comp.fill);
}
