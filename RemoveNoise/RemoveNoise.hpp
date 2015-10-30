//
//  RemoveNoise.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/23.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef RemoveNoise_hpp
#define RemoveNoise_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

class RemoveNoise{
public:
    RemoveNoise(cv::Size& frameSize, bool left, bool up,
                int threshNorm, int threshNum);
    RemoveNoise(const RemoveNoise& rn);
    
    RemoveNoise& operator=(const RemoveNoise& rn);
    
    void removeOutlier(cv::vector<cv::Point2f>& start,
                       cv::vector<cv::Point2f>& end) const;
    bool isEnoughAllVector(cv::vector<cv::Point2f>& start) const;
    bool isEnoughHalfVector(cv::vector<cv::Point2f>& start) const;
    bool isEnoughQuarterVector(cv::vector<cv::Point2f>& start) const;
    
private:
    cv::Size& frameSize;
    bool left;
    bool up;
    int threshNorm;
    int threshNum;
};

#endif /* RemoveNoise_hpp */
