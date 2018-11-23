//
//  main.cpp
//  CubesWorld
//
//  Created by Shanghao Zhong on 10/25/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "CubeUnitFactory.hpp"

using namespace std;

#include "Cube.hpp"

int main(int argc, const char * argv[]) {
    // create a csv for recording cubes location
    ofstream myCSV;
    // use std::ios::app as the second argument if don't want to overwrite
    myCSV.open("./cubes.txt");
    
    // create cubes
    vector<Cube*> cubes;
    for(int i = 0; i < 5; i++) {
        cubes.push_back(new Cube(i));
    }
    /*
     0^
     12y
     34|
     <z--+
     */
    JoinZ(cubes[4], cubes[3]);
    JoinZ(cubes[2], cubes[1]);
    JoinY(cubes[4], cubes[2]);
    JoinY(cubes[3], cubes[1]);
    JoinY(cubes[2], cubes[0]);
    
    cubes[0]->SetLocation(10,10,10);
    cubes[0]->UpdateCubeUnitLocation();
    ExportLocationAsCSV(myCSV, &cubes[0], cubes.size());
    
    cubes[0]->RotateCubeUnitOverX();
    ExportLocationAsCSV(myCSV, &cubes[0], cubes.size());
    
    cubes[4]->RotateCubeUnitOverY();
    ExportLocationAsCSV(myCSV, &cubes[0], cubes.size());
    
    string description = "3 2 2 1 0 1 1 1 1 0 1 0 0 1 1";
    stringstream ssDescription(description);
    Cube *cubeUnit = CubeUnitFactory::instance().ParseCubeUnit(ssDescription, 1);
    std::cout << cubeUnit->PrintCubeUnitLocationAsCSV();
    cubeUnit->ExportCubeUnitLocationAsCSV(myCSV);
    
    
    myCSV.close();
    
    return 0;
}
