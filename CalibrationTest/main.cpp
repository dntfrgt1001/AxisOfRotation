//
//  main.cpp
//  CalibrationTest
//
//  Created by masakazu nakazawa on 2015/09/27.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include <iostream>

#include <opencv2/opencv.hpp>

int main(int argc, const char * argv[])
{
    const std::string imgPath = "/Users/masakazu/Desktop/Opt/";
    const int checkerNum = 4;
    const std::string checkerPrefix = "calib";
    
    cv::Size frameSize(480, 640);
    
    cv::vector<cv::Mat> checkerImgs;
    const cv::Size  patternSize(10, 7);
    cv::vector<cv::vector<cv::Point3f>> worldPoints(checkerNum);
    cv::vector<cv::vector<cv::Point2f>> imagePoints(checkerNum);
    
    cv::TermCriteria criteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.001);
    
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
    cv::vector<cv::Mat> rotationVectors;
    cv::vector<cv::Mat> translationVectors;
    
    for(int i=0; i<checkerNum; i++){
        std::stringstream stream;
        stream << checkerPrefix << i+1 << ".jpg";
        std::string fileName = stream.str();
        cv::Mat tmp = cv::imread(imgPath+fileName, 0);
        cv::resize(tmp, tmp, frameSize);
        checkerImgs.push_back(tmp);
        std::cout << "load checker image: " << fileName << std::endl;
    }
        
    cv::namedWindow("Checker", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    for(int i=0; i<checkerNum; i++){
        std::cout << "find corners from image " << i+1;
        if(cv::findChessboardCorners(checkerImgs[i],
                                     patternSize, imagePoints[i])){
            std::cout << " ... all corners found." << std::endl;
            cv::cornerSubPix(checkerImgs[i], imagePoints[i], cv::Size(5, 5),
                             cv::Size(-1, -1), criteria);
            cv::drawChessboardCorners(checkerImgs[i], patternSize,
                                      (cv::Mat)(imagePoints[i]), true);
            cv::imshow("Checker", checkerImgs[i]);
            cv::waitKey(100);
        }else{
            std::cout << " ... at least 1 corner not found." << std::endl;
            cv::waitKey(0);
            return -1;
        }
    }
    cv::destroyWindow("Checker");
    
    for(int i=0; i<checkerNum; i++){
        for(int j=0; j<patternSize.area(); j++){
            worldPoints[i].push_back(
                cv::Point3f(static_cast<float>(j%patternSize.width*10),
                            static_cast<float>(j/patternSize.width*10),
                            0.0));
            }
        }
        
    cv::calibrateCamera(worldPoints, imagePoints, checkerImgs[0].size(),
                        cameraMatrix, distCoeffs, rotationVectors,
                        translationVectors);
    
    std::cout << "camera matrix" << std::endl;
    std::cout << cameraMatrix << std::endl;
    std::cout << "dist coeffs" << std::endl;
    std::cout << distCoeffs << std::endl;

    float angle = 90.0;
    float scale = 1.0;
    cv::Point2f center(frameSize.width*0.5, frameSize.height*0.5);
    const cv::Mat affineMat = cv::getRotationMatrix2D(center, angle, scale);
    
    
    
    cv::namedWindow("Input", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("Undistorted", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("Rotated", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    for(int i=0; i<checkerNum; i++){
        cv::Mat undistorted;
        cv::undistort(checkerImgs[i], undistorted, cameraMatrix, distCoeffs);
        
        cv::imshow("Input", checkerImgs[i]);
        cv::imshow("Undistorted", undistorted);
        
        cv::waitKey(-1);
    }
    
    return 0;
}
