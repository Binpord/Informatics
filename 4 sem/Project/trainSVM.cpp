#include "trainSVM.h"

std::vector<char> labels;
std::vector<std::string> trainingFiles;

// Trains cv::CvSVM from pictures, laying labeled in PicDir directory.
void trainSVM(std::string dir)
{
	GetTrainingFiles(dir);

	cv::HOGDescriptor hog(
			cv::Size(28, 28),	// winSize = imageSize
			cv::Size(14, 14),	// blockSize
			cv::Size(7, 7),		// blockStride
			cv::Size(14, 14),	// cellSize
			9,			// nbins
			// default values except last
			1,
			-1,
			0,
			0.2,
			1,
			64,
			1
			);
	int HOG_size = 81;		// defined by hog constructor parameters
	std::vector<float> hog_features;
	cv::Mat trainingMat(trainingFiles.size(), HOG_size, CV_32FC1);

	for(unsigned long i = 0; i < trainingFiles.size(); i++)
	{
		cv::Mat image = cv::imread(trainingFiles.at(i), cv::IMREAD_GRAYSCALE);
		hog.compute(image, hog_features);

		// filling training mat
		for(unsigned long j = 0; j < hog_features.size(); j++)
			trainingMat.at<float>(i, j) = hog_features.at(j);
	}

	int labelsArr[labels.size()];
	for(unsigned long i = 0; i < labels.size(); i++)
	{
		labelsArr[i] = labels.at(i);
	}

	cv::Mat labelsMat(labels.size(), 1, CV_32S, labelsArr);

	// trainig svm
	cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::RBF);
	svm->setC(12.5);
	svm->setGamma(0.50625);
	
	svm->train(trainingMat, cv::ml::ROW_SAMPLE, labelsMat);
	svm->save("svm.yml");
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

#ifdef __DEBUG
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
#endif
