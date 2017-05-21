#include "handwritten-recognition.h"

wxString GetExpression(wxBitmap* input)
{
	wxImage im_input = input->ConvertToImage();
	std::vector<cv::Mat>* sumbs = DivideSumbols(im_input);
	wxString result;

	int expr_size = sumbs->size();
	for(int i = 0; i < expr_size; i++)
	{
		result << PerformRecognition((*sumbs)[i]);
	}

	return result;
}

std::vector<cv::Mat>* DivideSumbols(const wxImage& image)
{
	int width = image.GetWidth();
	int count = 0;
	std::vector<cv::Mat>* result = new std::vector<cv::Mat>;
	while(count < width)
	{
		count += CountWhiteCols(image, count);
		if(count == width)
			break;
		
		int left = count;
		for(count; CountWhiteCols(image, count) == 0 && count < width; count++);

		int right = count - 1;

		wxRect sumbol_rect = CountTopAndBot(image, left, right);
		wxImage sumbol = image.GetSubImage(sumbol_rect);
#ifdef __DEBUG
		wxInitAllImageHandlers();
		sumbol.SaveFile("test1.png", wxBITMAP_TYPE_PNG);
#endif
		SwitchColors(&sumbol);
		// need image 1x1 with indentation (10 points should be enough)
		int new_size = std::max(sumbol.GetWidth(), sumbol.GetHeight()) + 10;
		int new_x = (new_size - sumbol.GetWidth()) / 2;
		int new_y = (new_size - sumbol.GetHeight()) / 2;
		sumbol.Resize(wxSize(new_size, new_size), wxPoint(new_x, new_y), 0, 0, 0);
#ifdef __DEBUG
		sumbol.SaveFile("test2.png", wxBITMAP_TYPE_PNG);
#endif
		sumbol.Rescale(28, 28);
#ifdef __DEBUG
		sumbol.SaveFile("test3.png", wxBITMAP_TYPE_PNG);
#endif
		cv::Mat cv_sumbol;
		Wx2CvImage(sumbol, cv_sumbol);
		result->push_back(rgb2gray(cv_sumbol));
	}

	return result;
}

int CountWhiteCols(const wxImage& image, int start)
{
	int width = image.GetWidth(), height = image.GetHeight();
	int x = start;
	for(; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			if(image.GetRed(x, y) != 255 || image.GetGreen(x, y) != 255 || image.GetBlue(x, y) != 255)
				goto found_non_white;
		}
	}
found_non_white:

	return x - start;
}

wxRect CountTopAndBot(const wxImage& image, int left, int right)
{
	int height = image.GetHeight();
	int top = 0, bottom = height;

	for(int y = 0; y < height; y++)
	{
		for(int x = left; x < right; x++)
		{
			if(image.GetRed(x, y) != 255 || image.GetGreen(x, y) != 255 || image.GetBlue(x, y) != 255)
			{
				top = y;
				goto bottom;
			}
		}
	}

bottom:
	for(int y = height - 1; y >= 0; y--)
	{
		for(int x = left; x < right; x++)
		{
			if(image.GetRed(x, y) != 255 || image.GetGreen(x, y) != 255 || image.GetBlue(x, y) != 255)
			{
				bottom = y;
				goto end;
			}
		}
	}
end:

	return wxRect(wxPoint(left, top), wxPoint(right, bottom));
}

wxString PerformRecognition(const cv::Mat& sumbol)
{
#ifdef __DEBUG
	cv::imwrite("test.jpg", sumbol);
#endif
	cv::Ptr<cv::ml::SVM> svm = cv::ml::StatModel::load<cv::ml::SVM>("svm.yml");
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

	//cv::Mat testMat = sumbol.clone().reshape(1,1);
	//testMat.convertTo(testMat, CV_32F);

	hog.compute(sumbol, hog_features);
#ifdef __DEBUG
	std::cout << hog_features.size() << std::endl;
#endif
	cv::Mat predictMat(hog_features);
	predictMat = predictMat.t();
	predictMat.convertTo(predictMat, CV_32F);
	
	int predicted = 0;

	try
	{
		predicted = svm->predict(predictMat);
	}
	catch(cv::Exception ex)
	{}

	wxString res;
	res += (char)predicted;
	return res;
}

void SwitchColors(wxImage* image)
{
	wxColor black(0, 0, 0), white(255, 255, 255);
	int width = image->GetWidth(), height = image->GetHeight();
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			if(image->GetRed(x, y) != white.Red() || image->GetGreen(x, y) != white.Green() || image->GetBlue(x, y) != white.Blue())
			{
				image->SetRGB(x, y, white.Red(), white.Green(), white.Blue());
			}
			else
			{
				image->SetRGB(x, y, black.Red(), black.Green(), black.Blue());
			}
		}
	}
}
