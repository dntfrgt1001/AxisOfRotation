//
//  CalcDeviationQuarter.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/18.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "CalcDeviationQuarter.hpp"

CalcDeviationQuarter::CalcDeviationQuarter(cv::Size& frameSize,
                                           int dx, int shift,
                                           bool left, bool up):
CalcDeviationHalf(frameSize, dx, shift, left), up(up)
{
}

CalcDeviationQuarter::CalcDeviationQuarter(const CalcDeviationQuarter& cdq):
CalcDeviationHalf(cdq), up(cdq.up)
{
}

bool CalcDeviationQuarter::isInWindow(const cv::Point2f& start) const
{
    cv::Rect window(curLeft, (up? 0: frameSize.height/2),
                    dx, frameSize.height/2);
    
    if(window.contains(start)){
        return true;
    }
    
    return false;
}

float CalcDeviationQuarter::calcValue(const cv::Point2f& start,
                                      const cv::Point2f& end) const
{
    float value = end.x - start.x;
    
    return value;
}