//
//  Configure.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/22.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef Configure_hpp
#define Configure_hpp

#include <stdio.h>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

class Configure{
public:
    Configure(std::string fileName);
    ~Configure(){};
    int getConfInt(std::string confName);
    std::string getConfString(std::string confName);
    void printError(std::string confName);
private:
    std::string fileName;
    boost::property_tree::ptree pt;
};

#endif /* Configure_hpp */
