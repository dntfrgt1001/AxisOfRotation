//
//  CalcDeviationQuarter.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/18.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "CalcDeviationQuarter.hpp"

CalcDeviationQuarter::CalcDeviationQuarter(cv::Size frameSize,
                                           cv::vector<cv::Point2f>& start,
                                           cv::vector<cv::Point2f>& end,
                                           int dx, int shift, bool left, bool up):
CalcDeviationHalf(frameSize, start, end, dx, shift, left), up(up)
{
}

bool CalcDeviationQuarter::isInWindow(const cv::Point2f& start)
{
    if(curLeft <= start.x && start.x < curLeft + dx &&
       (up? start.y <= frameSize.height / 2:
            frameSize.height / 2 < start.y)){
           return true;
    }
    
    return false;
}

float CalcDeviationQuarter::calcValue(const cv::Point2f& start,
                                      const cv::Point2f& end)
{
    float value = end.x - start.x;
    
    return value;
}