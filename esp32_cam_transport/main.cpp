#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/time.h>

#include <opencv2/opencv.hpp>

#define MAX_CHACE 1430

int main()
{

	char recv_buff[1500];

        struct sockaddr_in saddr;
        struct sockaddr_in presaddr;
        socklen_t len;

        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        bzero(&saddr, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons( 2334 );
        if( bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0 ){
                std::cout << "bind failed ..."<<std::endl;
        }
	std::cout<<"Init the Udp Server !"<<std::endl;

	cv::Mat image;
	
	while(1){
		memset( recv_buff, 0, sizeof( recv_buff ) );
		int ret = recvfrom( sockfd, recv_buff, sizeof( recv_buff ), 0, ( struct sockaddr* )&presaddr, &len );
		if( ret > 0 ){
			std::cout<<"recv len : "<<ret<<std::endl;
		}
		if( recv_buff[0] == 'L' && recv_buff[1] == 'E' && recv_buff[2] == 'N' && recv_buff[3] == 'G' ){
			int data_len = 0;
			memcpy( &data_len, &recv_buff[4], 4 );
			std::cout<<"a frame of data len = "<<data_len<<std::endl;
			char *image_buff = new char[data_len];			

			int times = data_len / MAX_CHACE;
			int extra = data_len % MAX_CHACE;	
			
			int i = 0;
			for( ; i < times; i ++ ){
				memset( recv_buff, 0, sizeof( recv_buff ) );
				ret = recvfrom( sockfd, recv_buff, sizeof( recv_buff ), 0, ( struct sockaddr* )&presaddr, &len );
				if( ret != MAX_CHACE ){
					std::cerr<<"recv error : "<<ret<<std::endl;
					break;
				}
				memcpy( &image_buff[i * MAX_CHACE], recv_buff, MAX_CHACE );
			}
			memset( recv_buff, 0, sizeof( recv_buff ) );
                        ret = recvfrom( sockfd, recv_buff, sizeof( recv_buff ), 0, ( struct sockaddr* )&presaddr, &len );
			if( ret > 0 ){
				memcpy( &image_buff[i * MAX_CHACE], recv_buff, ret );	
			}
	
			// genereate a image
			image = cv::imdecode(cv::Mat(1, data_len, CV_8UC1, image_buff), CV_LOAD_IMAGE_UNCHANGED);
			cv::imshow("iamge", image);
			cv::waitKey(10);
	
			delete[] image_buff;
		}
	}

	return 0;
}
