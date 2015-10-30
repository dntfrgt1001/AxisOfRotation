//
//  process.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/22.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "process.hpp"

void calibrateProcess(Configure& conf, cv::Mat& cameraMatrix, cv::Mat& distCoeffs)
{
    const cv::Size frameSize(conf.getConfInt("universal.frameWidth"),
                             conf.getConfInt("universal.frameHeight"));
    const cv::string dataPath(conf.getConfString("universal.dataPath"));
    
    const std::string checkerPrefix
        (conf.getConfString("calibration.checkerPrefix"));
    const std::string checkerSuffix
        (conf.getConfString("calibration.checkerSuffix"));
    const int checkerNum = conf.getConfInt("calibration.checkerNum");
    const cv::Size checkerSize(conf.getConfInt("calibration.checkerWidth"),
                               conf.getConfInt("calibration.checkerHeight"));
    
    cv::vector<cv::Mat> checkerImgs;
    cv::vector<cv::vector<cv::Point3f>> worldPoints(checkerNum);
    cv::vector<cv::vector<cv::Point2f>> imagePoints(checkerNum);
    
    cv::TermCriteria criteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.001);
    
    cv::vector<cv::Mat> rotationVectors;
    cv::vector<cv::Mat> translationVectors;
    
    for(int i=0; i<checkerNum; i++){
        std::stringstream stream;
        stream << checkerPrefix << i+1 << checkerSuffix;
        std::string fileName = stream.str();
        cv::Mat tmp = cv::imread(dataPath + fileName, 0);
        cv::resize(tmp, tmp, frameSize);
        checkerImgs.push_back(tmp);
        std::cout << "load checker image: " << fileName << std::endl;
    }
    
    cv::namedWindow("Source", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    for(int i=0; i<checkerNum; i++){
        std::cout << "find corners from image " << i+1;
        if(cv::findChessboardCorners(checkerImgs[i], checkerSize,
                                     imagePoints[i])){
            std::cout << " ... all corners found." << std::endl;
            cv::cornerSubPix(checkerImgs[i], imagePoints[i], cv::Size(5, 5),
                             cv::Size(-1, -1), criteria);
            cv::drawChessboardCorners(checkerImgs[i], checkerSize,
                                      (cv::Mat)(imagePoints[i]), true);
            cv::imshow("Source", checkerImgs[i]);
            cv::waitKey(200);
        }else{
            std::cout << " ... at least 1 corner not found." << std::endl;
            cv::waitKey(0);
            exit(-1);
        }
    }
    cv::destroyWindow("Source");
    
    for(int i=0; i<checkerNum; i++){
        for(int j=0; j<checkerSize.area(); j++){
            worldPoints[i].
                push_back(cv::Point3f(static_cast<float>(j%checkerSize.width*10),
                                      static_cast<float>(j/checkerSize.width*10),
                                      0.0));
        }
    }
    
    cv::calibrateCamera(worldPoints, imagePoints, frameSize, cameraMatrix,
                        distCoeffs, rotationVectors, translationVectors);
    
    std::cout << "camera matrix" << std::endl;
    std::cout << cameraMatrix << std::endl;
    std::cout << "dist coeffs" << std::endl;
    std::cout << distCoeffs << std::endl;
    
}
