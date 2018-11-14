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


#include "Cube.hpp"

int main(int argc, const char * argv[]) {
    // create a csv for recording cubes location
    std::ofstream myCSV;
    // use std::ios::app as the second argument if don't want to overwrite
    myCSV.open("/Users/zhongzachary/Desktop/cubes.txt");
    
    // create cubes
    std::vector<Cube*> cubes;
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
    cubes[0]->UpdateNeighborLocation();
    for(Cube *c : cubes) {
        std::cout << c;
    }
    ExportLocationAsCSV(myCSV, &cubes[0], cubes.size());
    std::cout << '\n';
    /*
         ^
      024y
       13|
     <z--+
     */
    cubes[0]->RotateNeighborX();
    ExportLocationAsCSV(myCSV, &cubes[0], cubes.size());
    for(Cube *c : cubes) {
        std::cout << c;
    }
    std::cout << '\n';
//    CUBE_PrintType = false;
//    CUBE_PrintDataName = false;
//    CUBE_PrintColor = false;
//    CUBE_PrintLocation = true;
    cubes[4]->RotateNeighborY();
    for(Cube *c : cubes) {
        std::cout << c;
    }

    std::cout << PrintLocationAsCSV(&(cubes[0]), cubes.size());
    
    ExportLocationAsCSV(myCSV, &cubes[0], cubes.size());

    myCSV.close();
    
    
    return 0;
}
