//
//  RemoveNoise.h
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/09/22.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#ifndef __AxisOfRotation__RemoveNoise__
#define __AxisOfRotation__RemoveNoise__

#include <stdio.h>
#include <opencv2/opencv.hpp>

class RemoveNoise{
public:
    RemoveNoise(cv::Size frameSize, bool left, bool up,
                int threshNorm, int threshNum);
    ~RemoveNoise(){};
    void setOpticalflow(cv::vector<cv::Point2f>* start,
                        cv::vector<cv::Point2f>* end);
    void removeOutlier();
    bool isEnoughAllVector();
    bool isEnoughHalfVector();
    bool isEnoughQuarterVector();
    
private:
    cv::vector<cv::Point2f>* start;
    cv::vector<cv::Point2f>* end;
    const cv::Size frameSize;
    const bool left;
    const bool up;
    const int threshNorm;
    const int threshNum;
};

#endif /* defined(__AxisOfRotation__RemoveNoise__) */
