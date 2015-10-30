//
//  process.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/22.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef process_hpp
#define process_hpp

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "Configure.hpp"

void calibrateProcess(Configure& conf, cv::Mat& cameraMatrix, cv::Mat& distCoeffs);


#endif /* process_hpp */
