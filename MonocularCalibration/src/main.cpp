#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "MonocularCalibration.h"

#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	std::vector<cv::Mat> images;
		
	cv::Mat img;
	for( int i = 0; i < 10; i ++ ){
		std::string path = "../calib_imgs/bsd/" + std::to_string( i + 1 ) + ".jpg";
		img = cv::imread( path );
		images.push_back( img );
	}

	int board_width = 9;
	int board_height = 6;
	
   	MonocularCalibration calib(images, Size(board_width, board_height));
    	calib.onLog();
    	calib.onShowImage();
    	calib.startCalibrate();

    	return 0;
}
