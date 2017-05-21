#include <wx/wx.h>
#include <wx/bitmap.h>
#include <wx/gdicmn.h>
#include "WxConvertCv.h"
#include <opencv2/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/objdetect.hpp>

std::vector<cv::Mat>* DivideSumbols(const wxImage&);
wxString GetExpression(wxBitmap* input);
double Evaluate(const wxString& expr);
wxString PerformRecognition(const cv::Mat&);
int CountWhiteCols(const wxImage&, int);
wxRect CountTopAndBot(const wxImage&, int, int);
void SwitchColors(wxImage*);
