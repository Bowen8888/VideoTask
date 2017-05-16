#include "opencv2\opencv.hpp"

using namespace cv;
using namespace std;


String filename;
int theObject[2] = { 0,0 };
Rect contourRect = Rect(0, 0, 0, 0);
int trial = 0, frameCount = 0, timeCount =0 ;


void searchForMovement(Mat thresholdImage, Mat &cameraFeed) {
	Mat temp;
	thresholdImage.copyTo(temp);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
	if (contours.size() > 0 && contours.size() < 6) {
		
		contourRect = boundingRect(contours.at(contours.size() - 1));

		int x = contourRect.x + contourRect.width / 2;
		int y = contourRect.y + contourRect.height / 2;

		int xpos = 0, ypos = 0;
		if (x < 1100 && y < 800) {
			if (timeCount > 35) {
				timeCount = 0;
				frameCount = 0;
				trial++;
			}
			else {
				timeCount = 0;
			}
			frameCount++;
			xpos = x;
			ypos = y;
			cout << "trial: ";
			cout << trial;
			cout << ", frame: ";
			cout << frameCount;
			cout << ", x: " << xpos;
			cout << ", y: " << ypos;
			cout << ", GOPR0173(1).avi \n";

		}
		else {
			frameCount++;

			cout << "trial: ";
			cout << trial;
			cout << ", frame: ";
			cout << frameCount;
			cout << ", x: " << theObject[0];
			cout << ", y: " << theObject[1];
			cout << ", GOPR0173(1).avi \n";
		}
		if (xpos!=0 && ypos!=0) {
			theObject[0] = xpos, theObject[1] = ypos;
		}
	}
	else{
		timeCount++;
	}

}
void main(int argv, char** argc) {
	int objectBlurSize = 10; //pen 5, pendulum 10 
	filename = "GOPR0173(1).avi";
	VideoCapture capture;
	Mat frame1, frame2, grayframe1, grayframe2, frameDifference, thresholdFrame;
	while (1) {
		capture.open(filename);

		if (!capture.isOpened()) {
			cout << "No Video \n";
		}
		while (capture.get(CV_CAP_PROP_POS_FRAMES) < capture.get(CV_CAP_PROP_FRAME_COUNT) - 1) {
			capture.read(frame1);
			capture.read(frame2);

			cvtColor(frame1, grayframe1, COLOR_BGR2GRAY);
			cvtColor(frame2, grayframe2, COLOR_BGR2GRAY);

			absdiff(grayframe1, grayframe2, frameDifference);

			threshold(frameDifference, thresholdFrame, 20, 21, THRESH_BINARY);

			blur(thresholdFrame, thresholdFrame, Size(objectBlurSize, objectBlurSize));

			threshold(thresholdFrame, thresholdFrame, 20, 255, THRESH_BINARY);
			
			searchForMovement(thresholdFrame, frame1);
		}

		capture.release();
	}
}

