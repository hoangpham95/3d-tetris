//
//  CubeUnitFactory.hpp
//  3d-tetris
//
//      This class is a singleton class for creating different types of cube
//      units in the 3d-tetris game. It is able to create a cube unit given a
//      text description. Users can provide a set of cube unit description as a
//      string or in a text file and this class can generate cube unit for all
//      of them. Moreover, this class will also randomly output a cube unit when
//      needed from the 3d-tetris game.
//      Note that each cube unit must be a set a contiguous cubes.
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#ifndef CubeUnitFactory_hpp
#define CubeUnitFactory_hpp

#include <stdio.h>
#include <vector>
#include <sstream>

#include "Cube.hpp"

class CubeUnitFactory {
public:
    // Access to the singleton CubeUnitFactory
    static CubeUnitFactory& instance();
    // Return a new cube unit from m_CubeUnits
    Cube NewRandomCubeUnit();
    // Add new type of Cube Units to m_CubeUnits from file
    void AddNewCubeUnits(std::string filepath);
    
    // parse the specific format as a cube unit
    // The first line of description indicate the dimension of the cube unit (x y z)
    // From the second line, each line is a description a row (parallel to x axis) in the cube unit
    // The order of rows is going toward y+ then z+
    // In each row, 1 indicates there is a cube and 0 indicates the lack of
    // e.g. a unit of 4 cubes on the x axis
    // 4 1 1
    // 1 1 1 1
    // e.g. a unit of 2x3 rectangle on the yz plane
    // 2 1 3
    // 1 1
    // 1 1
    // 1 1
    Cube* ParseCubeUnit(std::string &description, unsigned int id);
private:
    // Constructor
    CubeUnitFactory();
    std::vector<std::string> m_Descriptions;
    
};
#endif /* CubeUnitFactory_hpp */
