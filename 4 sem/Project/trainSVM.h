#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/objdetect.hpp>
#include <wx/string.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>

void trainSVM(std::string);
void GetTrainingFiles(std::string);
std::vector<std::string> GetListFiles(std::string);
void Print();
