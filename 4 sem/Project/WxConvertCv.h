#include <wx/image.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>

typedef unsigned char byte;

void Cv2WxImage(const cv::Mat& cvImg, wxImage& wxImg);
void Wx2CvImage(const wxImage& wxImg, cv::Mat& cvImg);
void Wx2CvImage(wxImage& wxImg, cv::Mat& cvImg, bool destroySrcImg);
cv::Mat convertType(const cv::Mat& srcImg, int toType, double alpha, double beta = 0);
cv::Mat rgb2gray(const cv::Mat& rgbImg);
