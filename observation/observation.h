#ifndef __OBSERVATION_H
#define __OBSERVATION_H

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>


namespace aruco
{

template<typename T>
struct Marker
{
	using DataType = T;
	using Vector3 = typename Eigen::Matrix<DataType, 3, 1>;

	Marker()
	{

	}

	~Marker()
	{

	}

	Marker( const int id, const Vector3& pose ) : id_( id ), pose_( pose )
	{

	}

	Marker( const int id, const DataType x, const DataType y, const DataType theta ) : id_( id ), pose_( Vector3( x, y, theta ) )
	{

	}

	int id_ = 0;
	Vector3 pose_ = Vector3::Zero();
};

template<typename T>
class Observation
{
public:
	using DataType = T;

	Observation()
	{
		dictionary_ = cv::aruco::getPredefinedDictionary( cv::aruco::DICT_4X4_100 );
	}

	Observation( const cv::Mat& K, const cv::Mat& dist, const DataType marker_length ) : K_( K ),	
											     dist_(dist),
											     marker_length_( marker_length )
	{
		dictionary_ = cv::aruco::getPredefinedDictionary( cv::aruco::DICT_4X4_100 );	
	}

	~Observation()
	{

	}

	const std::vector<Marker<DataType>>& detectMarker( const cv::Mat& image )
	{
		
		std::vector<std::vector<cv::Point2f>> marker_corners;
		std::vector<int> aruco_ids;
		std::vector<cv::Vec3d> rvs, tvs;

		cv::aruco::detectMarkers(image, dictionary_ , marker_corners, aruco_ids);
		cv::aruco::estimatePoseSingleMarkers(marker_corners, marker_length_, K_, dist_, rvs, tvs);		

		// draw all the markers
		marker_img_ = image.clone();
		cv::aruco::drawDetectedMarkers(marker_img_, marker_corners, aruco_ids);
		// for every marker
		for( size_t i = 0; i < aruco_ids.size(); i ++ ){
			cv::aruco::drawAxis(marker_img_, K_, dist_, rvs[i], tvs[i], 0.14);
			std::cout<<"rotation vector : "<<std::endl<<rvs[i]<<std::endl;
			std::cout<<"transport vector : "<<std::endl<<tvs[i]<<std::endl;
			
			// caculate the pose
			cv::Mat camPoseMatrix, camVecMatrix;
			cv::Mat vect = ( cv::Mat_<double>( 3, 1 ) << 0, 0, 1 );

			cv::Mat rMatrix, tMatrix;
			cv::Rodrigues( rvs[i], rMatrix );
			cv::transpose( tvs[i], tMatrix );
			cv::transpose( tMatrix, tMatrix );
			std::cout<<"rMatrix.cols = "<<rMatrix.cols<<", rMatrix.rows = "<<rMatrix.rows<<std::endl;
			std::cout<<"tMatrix.cols = "<<tMatrix.cols<<", tMatrix.rows = "<<tMatrix.rows<<std::endl;
			
			camPoseMatrix = rMatrix.inv() * ( -tMatrix );
			camVecMatrix = rMatrix.inv() * vect;

			std::cout<<"Camera Position : "<<camPoseMatrix.t()<<"\n Camera Direction : "<<camVecMatrix.t()<<std::endl;
		}	
		
		cv::imshow("marked image", marker_img_);
		return detected_markers;
	}

private:
	// camera's parameters
	cv::Mat K_, dist_; 
	
	// aruco dictionary
	cv::Ptr<cv::aruco::Dictionary> dictionary_;
	
	cv::Mat marker_img_;
	
	// aruco parameters
	DataType marker_length_ = 0;
	
	// detected result
	std::vector<Marker<DataType>> detected_markers;
};

}



#endif
