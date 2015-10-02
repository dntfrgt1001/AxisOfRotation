//
//  main.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/08/18.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/calib3d/calib3d.hpp>

int main(int argc, char* argv[])
{
    cv::Size frameSize(640, 480);
    
    cv::VideoCapture cap(0);
    if(!cap.isOpened()) return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, frameSize.width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameSize.height);
    
    const int checkerNum = 3;
    const cv::Size patternSize(10, 7);
    cv::vector<cv::Mat> checkerImgs;
    cv::vector<cv::vector<cv::Point3f>> worldPoints(checkerNum);
    cv::vector<cv::vector<cv::Point2f>> imagePoints(checkerNum);
    
    cv::TermCriteria criteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.001);
    
    cv::Mat cameraMatrix;
    cv::Mat distCoffs;
    cv::vector<cv::Mat> rotationVectors;
    cv::vector<cv::Mat> translationVectors;
    
    cv::namedWindow("checker", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    for(int i=0; i<checkerNum; i++){
        cv::Mat tmp;
        
        while(true){
            cap >> tmp;
            cv::imshow("checker", tmp);
            if(cv::waitKey(50) > 0){
                cv::Mat tmpGray = tmp.clone();
                cv::cvtColor(tmpGray, tmpGray, CV_BGR2GRAY);
                std::cout << i+1 << "th image" << std::endl;
                std::cout << "find corners from image" << std::endl;
                if(cv::findChessboardCorners(tmpGray, patternSize,
                                             imagePoints[i])){
                    std::cout << "all corners found" << std::endl;
                    cv::cornerSubPix(tmpGray, imagePoints[i],cv::Size(5, 5),
                                     cv::Size(-1, -1), criteria);
                    cv::drawChessboardCorners(tmp, patternSize,
                                              (cv::Mat)(imagePoints[i]),
                                              true);
                    cv::imshow("checker", tmp);
                    
                    checkerImgs.push_back(tmp);
                }else{
                    std::cout << "corners not found" << std::endl;
                    i--;
                }
                
                if(cv::waitKey() == 'q')
                    return 0;
                else
                    break;
            }
        }
    }
    cv::destroyWindow("checker");

    for(int i=0; i<checkerNum; i++){
        for(int j=0; j<patternSize.area(); j++){
            worldPoints[i].push_back(
                cv::Point3f(static_cast<float>(j % patternSize.width * 10),
                            static_cast<float>(j / patternSize.width * 10),
                            0.0f));
        }
    }
    
    cv::calibrateCamera(worldPoints, imagePoints, frameSize, cameraMatrix,
                        distCoffs, rotationVectors, translationVectors);
    
    std::cout << "camera matrix" << std::endl;
    std::cout << cameraMatrix  << std::endl;
    std::cout << "dist coefficients" << std::endl;
    std::cout << distCoffs << std::endl;
    
    cv::namedWindow("Input", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("Undistort", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    
    cv::Mat input;
    cv::Mat undistort;
    
    cv::Mat flowImg;
    cv::Mat prev, next;
    cv::Mat status, error;
    
    std::vector<cv::Point2f> prev_pts;
    std::vector<cv::Point2f> next_pts;
    
    
    cap >> input;
    cv::undistort(input, undistort, cameraMatrix, distCoffs);
    cv::cvtColor(undistort, next, CV_BGR2GRAY);
    while(true){
        prev = next.clone();
        cap >> input;
        cv::undistort(input, undistort, cameraMatrix, distCoffs);
        
        cv::cvtColor(undistort, next, CV_BGR2GRAY);
        
        flowImg = cv::Mat::zeros(undistort.rows, undistort.cols, CV_8UC1);
        
        cv::goodFeaturesToTrack(prev, prev_pts, 50, 0.1, 10, cv::Mat(), 3, true, 0.04);
        
        cv::calcOpticalFlowPyrLK(prev, next, prev_pts, next_pts, status, error);
        
        std::vector<cv::Point2f>::const_iterator p = prev_pts.begin();
        std::vector<cv::Point2f>::const_iterator n = next_pts.begin();
        for(; n!=next_pts.end(); p++, n++){
            cv::line(flowImg, *p, *n, cv::Scalar(255, 0, 0), 2);
            cv::circle(flowImg, *n, 5, cv::Scalar(0, 0, 255), -1, 8);
        }
        
        cv::imshow("Previous", prev);
        cv::imshow("Next", next);
        cv::imshow("OpticalFlow", flowImg);
        
        if(cv::waitKey(10) > 0) break;
    }
    
    return 0;
}