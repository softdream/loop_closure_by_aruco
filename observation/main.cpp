#include "observation.h"
#include <iostream>

int main()
{
	std::cout<<"---------------- OBSERVATION TEST ------------------"<<std::endl;
	double fx = 480.5652071326274;
	double fy = 477.0403526209364;
	double cx = 205.9494313886895;
	double cy = 160.3158389807322;
	double k1 = 0.1499315586261269;
	double k2 = -0.7074967957061068;
	double p1 = -0.00376060291026402;
	double p2 = -0.004013804531843665;
	double k3 = 0.0;

	double marker_len = 0.097;	

	cv::Mat K = (cv::Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
        cv::Mat dist = (cv::Mat_<double>(5, 1) << k1, k2, p1, p2, k3);	

	aruco::Observation<double> obs( K, dist, marker_len );
	
	cv::Mat image = cv::imread( "./data/4.jpg" );
	obs.detectMarker( image );
	cv::waitKey(0);

	return 0;
}
