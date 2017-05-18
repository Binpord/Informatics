#include "WxConvertCv.h"

/**
 * Convert cvImage (cv::Mat) to wxImage
 */
void Cv2WxImage(const cv::Mat& cvImg, wxImage& wxImg)
{
	// data dimension
	int w = cvImg.cols;
	int h = cvImg.rows;
	int size = w * h * 3 * sizeof(byte);

	// allocate memory for internal wxImage data
	byte* wxData = (byte*)malloc(size);

	// the matrix stores BGR image for conversion
	cv::Mat cvRGBImg = cv::Mat(h, w, CV_8UC3, wxData);

	switch (cvImg.channels())
	{
		case 1: // 1-channel case: expand and copy
		{
			// convert type if source is not an integer matrix
			if (cvImg.depth() != CV_8U)
			{
				cv::cvtColor(convertType(cvImg, CV_8U, 255), cvRGBImg, CV_GRAY2RGB);
			}
			else
			{
				cv::cvtColor(cvImg, cvRGBImg, CV_GRAY2RGB);
			}
		}
		break;

		case 3: // 3-channel case: swap R&B channels
		{
			int mapping[] = {0,2,1,1,2,0}; // CV(BGR) to WX(RGB)
			cv::mixChannels(&cvImg, 1, &cvRGBImg, 1, mapping, 3);
		}
		break;

		default:
		{
			// wxLogError(wxT("Cv2WxImage : input image (#channel=%d) should be either 1- or 3-channel"), cvImg.channels());
		}
	}

	wxImg.Destroy(); // free existing data if there's any
	wxImg = wxImage(w, h, wxData);
}

/**
 * Convert wxImage to cvImage
 */
void Wx2CvImage(const wxImage& wxImg, cv::Mat& cvImg)
{
	int w = wxImg.GetWidth();
	int h = wxImg.GetHeight();
	int mapping[] = {0,2,1,1,2,0}; // WX(RGB) to CV(BGR)
	cv::Mat cvRGBImg(h, w, CV_8UC3, wxImg.GetData());
	cvImg = cv::Mat(h, w, CV_8UC3);
	cv::mixChannels(&cvRGBImg, 1, &cvImg, 1, mapping, 3);

	// no need to worry about cvRGBImg, its destructor will free up
	//  allocated header while keeping the image data (which is now owned by wxImg) alive
}

/**
 * Convert wxImage to cvImage and free up source image.
 */
void Wx2CvImage(wxImage& wxImg, cv::Mat& cvImg, bool destroySrcImg)
{
	Wx2CvImage(wxImg, cvImg);

	if (destroySrcImg)
	{
		wxImg.Destroy();
	}
}

/**
 * one-line conversion of cv type.
 */
cv::Mat convertType(const cv::Mat& srcImg, int toType, double alpha, double beta)
{
	cv::Mat dstImg;
	srcImg.convertTo(dstImg, toType, alpha, beta);
	return dstImg;
}

/**
 * one-line conversion of color, MATLAB-style
 */
cv::Mat rgb2gray(const cv::Mat& rgbImg)
{
	cv::Mat grayImg;
	cv::cvtColor(rgbImg, grayImg, CV_RGB2GRAY);
	return grayImg;
}
