//
//  CalcDeviationHalf.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/09.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "CalcDeviationHalf.hpp"
#include "CalcDeviation.hpp"

CalcDeviationHalf::CalcDeviationHalf(cv::Size frameSize,
                                     cv::vector<cv::Point2f>& start,
                                     cv::vector<cv::Point2f>& end,
                                     int dx, int shift, bool left):
CalcDeviation(frameSize, start, end, dx, shift), left(left)
{
}

bool CalcDeviationHalf::isInWindow(const cv::Point2f& start)
{
    if(curLeft <= start.x && start.x < curLeft + dx) {
        return true;
    }
    
    return false;
}

float CalcDeviationHalf::calcValue(const cv::Point2f& start,
                                   const cv::Point2f& end)
{
    float value = end.x - start.x;
    
    if(frameSize.height / 2 < start.y)
        value *= -1;
    
    return value;
}
