#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#define PI 3.1415926

//Uncomment this line at run-time to skip GUI rendering
#define _DEBUG

using namespace cv;
using namespace std;

const string CAM_PATH = "/devideo0";
const string MAIN_WINDOW_NAME = "Processed Image";
const string CANNY_WINDOW_NAME = "Canny";

const int CANNY_LOWER_BOUND = 50;
const int CANNY_UPPER_BOUND = 250;
const int HOUGH_THRESHOLD = 150;

const int REC_WIDTH = 500;
const int REC_HEIGHT = 500;


void adjust(float x1, float x2, float arc,float rhoi){

	if (arc == 0){
		//在中间,不动
		if ((abs((x1 + x2) - REC_WIDTH)) < 50){

		}
			//左偏,右转
		else if ((x1 + x2)>REC_WIDTH){

		}
			//右偏,左转
		else if ((x1 + x2) < REC_WIDTH){

		}
	}
	else {
		//车头朝右
		if (rhoi < 0){
			//车在中间,需要左转
			if (abs((x1 + x2) - REC_WIDTH)<50){

			}
				//车在左边，不动
			else if ((x1 + x2)>REC_WIDTH){

			}
				//车在右边,需要左转
			else if ((x1 + x2)<REC_WIDTH){

			}

		}
			//车头朝左
		else if (rhoi>0){
			//车在中间，需要右转
			if (abs((x1 + x2) - REC_WIDTH) < 50){

			}
				//车在左边，右转
			else if ((x1 + x2)>REC_WIDTH){

			}
				//车在右边,不动
			else if ((x1 + x2) < REC_WIDTH){

			}
		}

	}
}

int main()

{

	VideoCapture capture(CAM_PATH);
	//If this fails, try to open as a video camera, through the use of an integer param
	if (!capture.isOpened())
	{
		capture.open(atoi(CAM_PATH.c_str()));
	}


	//init初始化


	double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);			//the width of frames of the video
	double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);		//the height of frames of the video
	//采集摄像头
	cout << "摄像头 ";
	cout << dWidth + ' ' + dHeight;

	Mat image;
	while (true)
	{
		capture >> image;
		if (image.empty())
			break;

		//Set the ROI for the image
		//调整到合适的位置
		Rect roi(0, image.rows / 3, image.cols, image.rows / 3);
		Mat imgROI = image(roi);

		//Canny algorithm
		Mat contours;
		Canny(imgROI, contours, CANNY_LOWER_BOUND, CANNY_UPPER_BOUND);
#ifdef _DEBUG
		cv::imshow(CANNY_WINDOW_NAME, contours);
#endif

		vector<Vec2f> lines;
		HoughLines(contours, lines, 1, PI / 180, HOUGH_THRESHOLD);
		Mat result(imgROI.size(), CV_8U, Scalar(255));
		imgROI.copyTo(result);
		clog << lines.size() << endl;

		float maxRad = -2 * PI;
		float minRad = 2 * PI;
		//Draw the lines and judge the slope
		if (lines.size() == 2){
			float x1 = 0;
			float x2 = 0;
			int count = 0;
			float arc = 0;
			float rhoi = 0;
			for (vector<Vec2f>::const_iterator it = lines.begin(); it != lines.end(); ++it)
			{
				float rho = (*it)[0];			//First element is distance rho
				float theta = (*it)[1];		//Second element is angle theta

				//Filter to remove vertical and horizontal lines,
				//and atan(0.09) equals about 5 degrees.
				if ((theta>0.09&&theta<1.48) || (theta>1.62&&theta<3.05))
				{
					if (theta>maxRad)
						maxRad = theta;
					if (theta<minRad)
						minRad = theta;

#ifdef _DEBUG
					//point of intersection of the line with first row
					Point pt1(rho / cos(theta), 0);
					//point of intersection of the line with last row
					Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.rows);
					//Draw a line

					if (count == 0){
						x1 = (rho - result.rows*sin(theta)) / cos(theta);
						arc = theta;
						rhoi = rho;
						count++;
						cout << "\n";
						cout << "第一个点";
						cout << x1 + ' ' + theta + ' ' + rhoi + ' ';
					}
					else if (count == 1){
						x2 = (rho - result.rows*sin(theta)) / cos(theta);
						cout << "\n";
						cout << "第二个点";
						cout << x2 + ' ' + theta + ' ' + rho + ' ';
					}

					//pt1，pt2就是点坐标
					line(result, pt1, pt2, Scalar(0, 255, 255), 3, CV_AA);
#endif
				}
				adjust(x1, x2, arc,rhoi);
#ifdef _DEBUG
				clog << "Line: (" << rho << "," << theta << ")\n";
#endif
			}
		}

#ifdef _DEBUG
		stringstream overlayedText;
		overlayedText << "Lines: " << lines.size();
		cv::putText(result, overlayedText.str(), Point(10, result.rows - 10), 2, 0.8, Scalar(0, 0, 255), 0);
		cv::imshow(MAIN_WINDOW_NAME, result);
#endif

		lines.clear();
		cv::waitKey(1);
	}
	return 0;
}
