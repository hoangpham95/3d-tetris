//
//  CubeUnitFactory.cpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "CubeUnitFactory.hpp"
#include <sstream>
#include <fstream>
#include <ostream>
#include <cassert>
#include <random>

using namespace std;

// This is not a very robust parser
Cube* CubeUnitFactory::ParseCubeUnit(stringstream &description, unsigned int id) {
    unsigned short x, y, z;
    description >> x >> y >> z;
    bool cubeLocation[z][y][x];
    for(int i = 0; i < z; i++) {
        for(int j = 0; j < y; j++) {
            for(int k = 0; k < x; k++) {
                description >> cubeLocation[i][j][k];
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

Cube* CubeUnitFactory::ParseCubeUnit(string &description, unsigned int id) {
    stringstream ssD(description);
    return ParseCubeUnit(ssD, id);
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

void CubeUnitFactory::AddNewCubeUnits(string filepath) {
    ifstream file(filepath);
    
    // check if the file can be opened
    if(!file.is_open()) {
        cout << "Unable to read Cube Units from file " << filepath << ".\n";
        return;
    }
    
    // the file can be opened
    cout << "Using Cube Units from file " << filepath << ".\n";
    
    string line, word;
    stringstream description;
    unsigned int numD(0); // number of description
    unsigned short x, y, z;
    bool val;
    while(file >> word) {
        // opening of a description
        if(word == "{") {
            ++numD;
            // getting dimension
            file >> x >> y >> z;
            description << x << ' ' << y << ' ' << z << ' ';
            // checking the description is properly formatted
            for(int i = 0; i < x * y * z; ++i) {
                file >> val;
                description << val << ' ';
            }
            // check if the description ends, as it should
            file >> word;
            if(word == "}") {
                m_Descriptions.push_back(description.str());
                description = stringstream();
            } else {
                cout << "Description error for description #" << numD << ". Abort reading from file " << filepath << ".\n";
                break;
            }
        } else {
            // skip the line that is not within curly braces
            getline(file, line);
        }
    }
    cout << numD << " Cube Unit descriptions have been added from " << filepath << ".\n";
}

unsigned int CubeUnitFactory::GetCubeUnitFactorySize() {
    return m_Descriptions.size();
}

Cube* CubeUnitFactory::MakeCubeUnit(unsigned int index, unsigned int id) {
    string description = m_Descriptions[index];
    return ParseCubeUnit(description, id);
}

Cube* CubeUnitFactory::NewRandomCubeUnit() {
    static random_device rd;
    static mt19937_64 gen(rd());
    uniform_int_distribution<> dis(0, m_Descriptions.size() - 1);
    
    static unsigned int id = 0;
    
    return MakeCubeUnit(dis(gen), id++);
}
