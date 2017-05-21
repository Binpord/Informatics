#include "precompiled_headers.h"
#include "WxConvertCv.h"

std::vector<cv::Mat>* DivideSumbols(const wxImage&);
wxString GetExpression(wxBitmap* input);
double Evaluate(const wxString& expr);
wxString PerformRecognition(const cv::Mat&);
int CountWhiteCols(const wxImage&, int);
wxRect CountTopAndBot(const wxImage&, int, int);
void SwitchColors(wxImage*);
