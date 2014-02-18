#define _CRT_SECURE_NO_DEPRECATE



#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "Thing.h"



using namespace std;
using namespace cv;

const static int SENSITIVITY_VALUE = 10;
const static int BLUR_SIZE = 70;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

const int MIN_OBJECT_AREA = 20 * 20;




string intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}




void drawObject(vector<Thing> theThings, Mat &frame){
	
	for (int i = 0; i<theThings.size(); i++){
		cv::circle(frame, cv::Point(theThings.at(i).getXPos(), theThings.at(i).getYPos()), 10, cv::Scalar(0, 0, 255));
		cv::putText(frame, intToString(theThings.at(i).getXPos()) + " , " + intToString(theThings.at(i).getYPos()), cv::Point(theThings.at(i).getXPos(), theThings.at(i).getYPos() + 20), 1, 1, Scalar(0, 255, 0));
	}
}




void searchForMovement(Mat thresholdImage, Mat &cameraFeed){

	vector <Thing> ones;

	Mat temp;
	thresholdImage.copyTo(temp);

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	double refArea = 0;
	bool objectDetected = false;
	if (hierarchy.size() > 0) {
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {

			Moments moment = moments((cv::Mat)contours[index]);
			double area = moment.m00;

			if (area > MIN_OBJECT_AREA){

				Thing one;

				one.setXPos(moment.m10 / area);
				one.setYPos(moment.m01 / area);

				ones.push_back(one);

				objectDetected = true;}

			else objectDetected = false;

		}

		if (objectDetected == true){drawObject(ones, cameraFeed);}
		else putText(cameraFeed, "NO OBJECT DETECTED\n", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);

	}
}




int main(){

	bool objectDetected = false;
	bool debugMode = false;
	bool trackingEnabled = false;
	bool pause = false;

	Mat frame1, frame2;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage;


	VideoCapture capture;


	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	if (!capture.isOpened()){
		cout << "ERROR ACQUIRING VIDEO FEED\n";
		getchar();
		return -1;}


	while (1){


		capture.read(frame1);
		cv::cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);


		capture.read(frame2);
		cv::cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);


		cv::absdiff(grayImage1, grayImage2, differenceImage);
		cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
		

		if (debugMode == true){
			cv::imshow("Difference Image", differenceImage);
			cv::imshow("Threshold Image", thresholdImage);}
		else{
			cv::destroyWindow("Difference Image");
			cv::destroyWindow("Threshold Image");}


		cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
		cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);


		if (debugMode == true){imshow("Final Threshold Image", thresholdImage);}
		else {cv::destroyWindow("Final Threshold Image");}


		if (trackingEnabled){searchForMovement(thresholdImage, frame1);}


		imshow("Frame1", frame1);


		switch (waitKey(10)){
		case 27: //'esc' key has been pressed, exit program.
			return 0;
		case 116: //'t' has been pressed. this will toggle tracking
			trackingEnabled = !trackingEnabled;
			if (trackingEnabled == false) cout << "Tracking disabled." << endl;
			else cout << "Tracking enabled." << endl;
			break;
		case 100: //'d' has been pressed. this will debug mode
			debugMode = !debugMode;
			if (debugMode == false) cout << "Debug mode disabled." << endl;
			else cout << "Debug mode enabled." << endl;
			break;
		case 112: //'p' has been pressed. this will pause/resume the code.
			pause = !pause;
			if (pause == true){
				cout << "Code paused, press 'p' again to resume" << endl;
				while (pause == true){
					//stay in this loop until 
					switch (waitKey()){
						//a switch statement inside a switch statement? Mind blown.
					case 112:
						//change pause back to false
						pause = false;
						cout << "Code resumed." << endl;
						break;
					}
				}
			}
		}
	}
	return 0;
}