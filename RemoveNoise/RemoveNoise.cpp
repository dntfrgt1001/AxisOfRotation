//
//  RemoveNoise.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/23.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "RemoveNoise.hpp"

RemoveNoise::RemoveNoise(cv::Size& frameSize, bool left, bool up,
                         int threshNorm, int threshNum):
frameSize(frameSize), left(left), up(up),
threshNorm(threshNorm), threshNum(threshNum)
{
}

RemoveNoise::RemoveNoise(const RemoveNoise& rn):
frameSize(rn.frameSize), left(rn.left), up(rn.up), threshNorm(rn.threshNorm)
{
}

RemoveNoise& RemoveNoise::operator=(const RemoveNoise& rn)
{
    frameSize = rn.frameSize;
    left = rn.left;
    up = rn.up;
    threshNorm = rn.threshNorm;
    threshNum = rn.threshNum;
    
    return *this;
}

void RemoveNoise::removeOutlier(cv::vector<cv::Point2f>& start,
                                cv::vector<cv::Point2f>& end) const
{
    float averageNorm = 0.0f;
    
    for(auto startIter=start.begin(),endIter=end.begin();
        startIter!=start.end(); startIter++,endIter++)
    {
        averageNorm += cv::norm(*startIter - *endIter);
    }
    averageNorm /= start.size();
    
    for(auto startIter=start.begin(), endIter=end.begin();
        startIter!=start.end(); /* look at the end of for */)
    {
        if(cv::norm(*startIter - *endIter) > threshNorm * averageNorm){
            startIter = start.erase(startIter);
            endIter = end.erase(endIter);
            
            continue;
        }
        
        startIter++, endIter++;
    }
}

bool RemoveNoise::isEnoughAllVector(cv::vector<cv::Point2f>& start) const
{
    int count = (int) start.size();
    
    if(count < threshNum) return false;
    
    return true;
}

bool RemoveNoise::isEnoughHalfVector(cv::vector<cv::Point2f>& start) const
{
    int xCenter = frameSize.width / 2;
    int count = 0;
    
    for(auto startIter=start.begin(); startIter!=start.end(); startIter++){
        if((left? startIter->x <= xCenter: xCenter < startIter->x)){
            count++;
        }
    }
    
    if(count < threshNum / 2) return false;
    
    return true;
}

bool RemoveNoise::isEnoughQuarterVector(cv::vector<cv::Point2f>& start) const
{
    int xCenter = frameSize.width / 2;
    int yCenter = frameSize.height / 2;
    int count = 0;
    
    for(auto startIter=start.begin(); startIter!=start.end(); startIter++){
        if((left? startIter->x <= xCenter: xCenter < startIter->x) &&
           (up? startIter->y <= yCenter: yCenter < startIter->y))
        {
            count++;
        }
    }
    
    if(count < threshNum / 4) return false;
    
    return true;
}