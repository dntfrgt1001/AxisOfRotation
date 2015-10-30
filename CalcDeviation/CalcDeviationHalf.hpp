//
//  CalcDeviationHalf.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/09.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef CalcDeviationHalf_hpp
#define CalcDeviationHalf_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "CalcDeviation.hpp"

class CalcDeviationHalf: public CalcDeviation{
public:
    CalcDeviationHalf(cv::Size& frameSize, int dx, int shift, bool left);
    CalcDeviationHalf(const CalcDeviationHalf& cdh);
    
    void resetIter();
    bool isIterEnd() const;
    
protected:
    CalcDeviationHalf& operator=(const CalcDeviationHalf& cdh);
    bool left;
};

#endif /* CalcDeviationHalf_hpp */
