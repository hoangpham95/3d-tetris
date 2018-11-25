//
//  CubeUnitFactory.cpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "CubeUnitFactory.hpp"
#include <sstream>


// This is not a very robust parser
Cube* CubeUnitFactory::ParseCubeUnit(std::string &description, unsigned int id) {
    std::stringstream ssDescription(description);
    unsigned short x, y, z;
    ssDescription >> x >> y >> z;
    bool cubeLocation[z][y][x];
    for(int i = 0; i < z; i++) {
        for(int j = 0; j < y; j++) {
            for(int k = 0; k < x; k++) {
                ssDescription >> cubeLocation[i][j][k];
            }
        }
    }
    // build a dynamically-allocated cube unit based on the description
    Cube* head = nullptr;
    Cube* curr;
    Cube* cubeUnit[z][y][x];
    for(int i = 0; i < z; i++) {
        for(int j = 0; j < y; j++) {
            for(int k = 0; k < x; k++) {
                // if there is a cube in that location
                if(cubeLocation[i][j][k]) {
                    // create a new cube
                    curr = new Cube(id);
                    // if the head is null, set it as head
                    if(!head) {
                        head = curr;
                    }
                    // put the cube into the array
                    cubeUnit[i][j][k] = curr;
                    // if there is a cube in the neg x direction, connect it
                    if(k > 0 && cubeLocation[i][j][k-1]) {
                        JoinX(cubeUnit[i][j][k-1], curr);
                    }
                    // if there is a cube in the neg y direction, connect it
                    if(j > 0 && cubeLocation[i][j-1][k]) {
                        JoinY(cubeUnit[i][j-1][k], curr);
                    }
                    // if there is a cube in the neg z direction, connect it
                    if(i > 0 && cubeLocation[i-1][j][k]) {
                        JoinZ(cubeUnit[i-1][j][k], curr);
                    }
                }
            }
        }
    }
    // set head location to origin and update cube unit's location
    head->SetLocation(0, 0, 0);
    head->UpdateCubeUnitLocation();
    return head;
}

CubeUnitFactory& CubeUnitFactory::instance() {
    static CubeUnitFactory* instance;
    if(!instance) {
        instance = new CubeUnitFactory();
    }
    return *instance;
}

CubeUnitFactory::CubeUnitFactory() {
    
}
