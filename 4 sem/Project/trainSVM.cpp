#include "trainSVM.h"

std::vector<char> labels;
std::vector<std::string> trainingFiles;

// Trains cv::CvSVM from pictures, laying labeled in PicDir directory.
void trainSVM(std::string dir)
{
}

void GetTrainingFiles(std::string dir)
{
	std::vector<std::string> sumbols = GetListFiles(dir);
	for(std::string sumbol : sumbols)
	{
		std::vector<std::string> sumbol_examples = GetListFiles(dir + '/' + sumbol);
		int size = sumbol_examples.size();
		for(int i = 0; i < size; i++)
			sumbol_examples.at(i) = dir + '/' + sumbol + '/' + sumbol_examples.at(i);

		trainingFiles.insert(trainingFiles.end(), sumbol_examples.begin(), sumbol_examples.end());
		labels.insert(labels.end(), sumbol_examples.size(), sumbol.at(0));	// assuming only one-chared sumbols
	}
}

// Unfortunately, opencv since 3rd version do not support this function.
// However, using it simplifies my code, so that I decided to emulate it.
std::vector<std::string> GetListFiles(std::string dir)
{
	wxDir sumb_dir(dir);
	if(sumb_dir.IsOpened() != true)
	{
		wxMessageBox(wxT("Failed to open directory."));
		return std::vector<std::string>();
	}

	wxString file;
	std::vector<std::string> result;

	bool has_file = sumb_dir.GetFirst(&file);
	while(has_file)
	{
		result.push_back(file.ToStdString());
		has_file = sumb_dir.GetNext(&file);
	}

	return result;
}


void Print()
{
	GetTrainingFiles("test_dir");
	for(char i : labels)
		std::cout << i << ' ';

	std::cout << std::endl;
	
	for(std::string i : trainingFiles)
		std::cout << i << ' ';
	std::cout << std::endl;
}
