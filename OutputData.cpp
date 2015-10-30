//
//  OutputData.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/02.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "OutputData.hpp"

OutputData::OutputData(const std::string fileName, char seperater):
fileName(fileName), seperater(seperater)
{
    ofs.open(fileName);
}

OutputData::~OutputData()
{
    ofs.close();
}

OutputData::OutputData(const OutputData& od):
fileName(od.fileName), seperater(od.seperater)
{
    ofs.open(fileName);
}

void OutputData::outputTwin(int first, int second)
{
    ofs << first << seperater << second << std::endl;
}

void OutputData::outputTriple(int first, int second, int third)
{
    ofs << first << seperater << second << seperater << third << std::endl;
}
