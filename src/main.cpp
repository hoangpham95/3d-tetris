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
    
    cubes[0]->DeleteCubeUnit();
    
    CubeUnitFactory& factory = CubeUnitFactory::instance();
    
    string description = "3 2 2 1 0 1 1 1 1 0 1 0 0 1 1";
    Cube *cubeUnit = factory.ParseCubeUnit(description, 1);
    cubeUnit->SetLocation(5, -5, 5);
    cubeUnit->UpdateCubeUnitLocation();
    
    Cube *dupCubeUnit = cubeUnit->DuplicateCubeUnit();
    dupCubeUnit->SetLocation(7, -7, 7);
    dupCubeUnit->RotateCubeUnitOverX(false);
    
    cubeUnit->ExportCubeUnitLocationAsCSV(myCSV);
    dupCubeUnit->ExportCubeUnitLocationAsCSV(myCSV);
    
    cubeUnit->DeleteCubeUnit();
    dupCubeUnit->DeleteCubeUnit();
    
    factory.AddNewCubeUnits("./other/cubes.txt");
    Cube *tmp;
//    for(int i = 0; i < 4; ++i) {
//        tmp = factory.MakeCubeUnit(i, i);
//        tmp->SetLocation(-4 + 2 * i, -4 + 2 * i, 0);
//        tmp->UpdateCubeUnitLocation();
//        tmp->ExportCubeUnitLocationAsCSV(myCSV);
//        tmp->DeleteCubeUnit();
//    }
    
    for(int i = 0; i < 10; ++i) {
        tmp = factory.NewRandomCubeUnit();
        tmp->SetLocation(-10 + 2 * i, -10 + 2 * i, 0);
        tmp->UpdateCubeUnitLocation();
        tmp->ExportCubeUnitLocationAsCSV(myCSV);
        cout << tmp->GetID() << ' ';
        tmp->DeleteCubeUnit();
    }
    
    myCSV.close();
    
    return 0;
}
