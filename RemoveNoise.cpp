//
//  RemoveNoise.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/09/22.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include "RemoveNoise.h"

RemoveNoise::RemoveNoise(cv::Size frameSize,
                         cv::vector<cv::Point2f>* start,
                         cv::vector<cv::Point2f>* end,
                         bool left, bool up,
                         int threshNorm, int threshNum):
frameSize(frameSize), start(start), end(end),
left(left), up(up), threshNorm(threshNorm), threshNum(threshNum)
{
    
}

void RemoveNoise::setOpticalflow(cv::vector<cv::Point2f>* start,
                                 cv::vector<cv::Point2f>* end)
{
    this->start = start;
    this->end = end;
}

bool RemoveNoise::isEnoughAllVector()
{
    int count = (int)start->size();
    
    if(count < threshNum)
        return false;
    
    return true;
}

void RemoveNoise::removeOutlier()
{
    float averageNorm = 0.0f;
    
    cv::vector<cv::Point2f>::iterator startIter;
    cv::vector<cv::Point2f>::iterator endIter;

    startIter = start->begin();
    endIter = end->begin();
    for(; startIter!=start->end(); startIter++, endIter++){
        averageNorm += cv::norm(*startIter - *endIter);
    }
    averageNorm /= start->size();
    
    startIter = start->begin();
    endIter = end->begin();
    for(; startIter!=start->end();){
        if(cv::norm(*startIter - *endIter) > threshNorm * averageNorm){
            startIter = start->erase(startIter);
            endIter = end->erase(endIter);
            
            continue;
        }
        
        startIter++;
        endIter++;
    }
}

bool RemoveNoise::isEnoughHalfVector()
{
    int xCenter = frameSize.width / 2;
    int count = 0;
    
    cv::vector<cv::Point2f>::const_iterator startIter = start->begin();
    for(; startIter!=start->end(); startIter++){
        if((left? startIter->x <= xCenter: xCenter < startIter->x)){
            count++;
        }
    }
    
    if(count < threshNum / 2)
        return false;
    
    return true;
}

bool RemoveNoise::isEnoughQuarterVector()
{
    int xCenter = frameSize.width / 2;
    int yCenter = frameSize.height / 2;
    int count = 0;
 
    cv::vector<cv::Point2f>::const_iterator startIter = start->begin();
    for(; startIter!=start->end(); startIter++){
        if((left? startIter->x <= xCenter: xCenter < startIter->x) &&
           (up?   startIter->y <= yCenter: yCenter < startIter->y)){
            count++;
        }
    }
    
    if(count < threshNum / 4)
        return false;
    
    return true;
}



