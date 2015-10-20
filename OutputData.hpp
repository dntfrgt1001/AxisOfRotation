//
//  OutputData.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/02.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef OutputData_hpp
#define OutputData_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>

class OutputData {
public:
    OutputData(std::string fileName, char seperater);
    ~OutputData();
    
    void outputTwin(int first, int second);
    void outputTriple(int first, int second, int third);
    
private:
    const std::string fileName;
    std::ofstream ofs;
    const char seperater;
};

#endif /* OutputData_hpp */
