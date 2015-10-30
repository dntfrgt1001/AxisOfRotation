//
//  CalcDeviationHalf.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/09.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "CalcDeviationHalf.hpp"
#include "CalcDeviation.hpp"

CalcDeviationHalf::CalcDeviationHalf(cv::Size& frameSize,
                                     int dx, int shift, bool left):
CalcDeviation(frameSize, dx, shift), left(left)
{
}

CalcDeviationHalf::CalcDeviationHalf(const CalcDeviationHalf& cdh):
CalcDeviation(cdh), left(cdh.left)
{
}

void CalcDeviationHalf::resetIter()
{
    curLeft = left? 0: frameSize.width/2;
}

bool CalcDeviationHalf::isIterEnd() const
{
    return left? frameSize.width/2 < curLeft: frameSize.width < curLeft;
}