//
//  main.cpp
//  CaptureImage
//
//  Created by masakazu nakazawa on 2015/08/18.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, const char * argv[])
{
    int frameWidth = 640;
    int frameHeight = 480;
    
    cv::VideoCapture cap(0);
    if(!cap.isOpened()) return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);
    
    cv::namedWindow("capture", CV_WINDOW_FREERATIO|CV_WINDOW_AUTOSIZE);
    
    cv::Mat input;
    while(true){
        cap >> input;
        
        cv::imshow("capture", input);
        
        if(cv::waitKey(30) > 0){
            cv::imwrite("/Users/masakazu/Desktop/capture2.png", input);
            break;
        }
    }
    
    return 0;
}
