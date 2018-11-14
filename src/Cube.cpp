//
//  Cube.cpp
//  CubesWorld
//
//  Created by Shanghao Zhong on 10/25/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "Cube.hpp"

Cube::Cube(unsigned int id) : m_id(id){
}

Cube::~Cube() {
    // prevent its neighbor from accessing random memory
    if(m_xNeg) m_xNeg->m_xPos = nullptr;
    if(m_xPos) m_xPos->m_xNeg = nullptr;
    if(m_yNeg) m_yNeg->m_yPos = nullptr;
    if(m_yPos) m_yPos->m_yNeg = nullptr;
    if(m_zNeg) m_zNeg->m_zPos = nullptr;
    if(m_zPos) m_zPos->m_zNeg = nullptr;
    // setting its neighbor to
}

void JoinX(Cube* xNeg, Cube* xPos) {
    if(xNeg) {
        xNeg->m_xPos = xPos;
    }
    if(xPos) {
        xPos->m_xNeg = xNeg;
    }
}

void JoinY(Cube* yNeg, Cube* yPos) {
    if(yNeg) {
        yNeg->m_yPos = yPos;
    }
    if(yPos) {
        yPos->m_yNeg = yNeg;
    }
}

void JoinZ(Cube* zNeg, Cube* zPos) {
    if(zNeg) {
        zNeg->m_zPos = zPos;
    }
    if(zPos) {
        zPos->m_zNeg = zNeg;
    }
}

std::ostream& operator<<(std::ostream& out, const Cube& cube) {
    out << (CUBE_PrintType ? "Cube(" : "");
    // print id
    if(CUBE_PrintID) {
        if(CUBE_PrintDataName) {
            out << "id:";
        }
        out << cube.GetID();
    }
    out << CUBE_PrimarySeperator;
    // print color
    if(CUBE_PrintColor) {
        if(CUBE_PrintDataName) {
            out << "rgba:";
        }
        for(int i = 0; i < 4; i++) {
            out << cube.m_rgba[i];
            if (i != 3) out << CUBE_SecondarySeperator;
        }
    }
    out << CUBE_PrimarySeperator;
    // print location
    if(CUBE_PrintLocation) {
        if(CUBE_PrintDataName) {
            out << "xyz:";
        }
        for(int i = 0; i < 3; i++) {
            out << cube.m_xyz[i];
            if (i != 2) out << CUBE_SecondarySeperator;
        }
    }
    out << CUBE_PrimarySeperator;
    // print neighbor
    if(CUBE_PrintNeighborID) {
        if(CUBE_PrintDataName) {
            out << "neighbor:";
        }
        out << cube.GetXNeg()->GetID() << CUBE_SecondarySeperator;
        out << cube.GetXPos()->GetID() << CUBE_SecondarySeperator;
        out << cube.GetYNeg()->GetID() << CUBE_SecondarySeperator;
        out << cube.GetYPos()->GetID() << CUBE_SecondarySeperator;
        out << cube.GetZNeg()->GetID() << CUBE_SecondarySeperator;
        out << cube.GetZPos()->GetID() << CUBE_SecondarySeperator;
    }
    if(CUBE_PrintType) {
        out << ")";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Cube* cube) {
    if (cube) {
        out << *cube;
    } else {
        out << "NULL";
    }
    return out;
}

void Cube::SetColor(float r, float g, float b, float a) {
    m_rgba[0] = r;
    m_rgba[1] = g;
    m_rgba[2] = b;
    m_rgba[3] = a;
}

void Cube::SetLocation(int x, int y, int z) {
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
}

void Cube::UpdateNeighborLocation() {
    std::unordered_set<Cube*> visited;
    std::unordered_set<Cube*> next;
    next.insert(this);
    
    while(!next.empty()){
        // pop the first element in the queue
        Cube& wkc = **(next.begin());
        next.erase(&wkc);
        visited.insert(&wkc);
        // record the location
        int x = wkc.m_xyz[0];
        int y = wkc.m_xyz[1];
        int z = wkc.m_xyz[2];
        // update each of the unvisited neighboring cubes
        if(wkc.m_xNeg && visited.count(wkc.m_xNeg) == 0) {
            wkc.m_xNeg->SetLocation(x - 1, y, z);
            next.insert(wkc.m_xNeg);
        }
        if(wkc.m_xPos && visited.count(wkc.m_xPos) == 0) {
            wkc.m_xPos->SetLocation(x + 1, y, z);
            next.insert(wkc.m_xPos);
        }
        if(wkc.m_yNeg && visited.count(wkc.m_yNeg) == 0) {
            wkc.m_yNeg->SetLocation(x, y - 1, z);
            next.insert(wkc.m_yNeg);
        }
        if(wkc.m_yPos && visited.count(wkc.m_yPos) == 0) {
            wkc.m_yPos->SetLocation(x, y + 1, z);
            next.insert(wkc.m_yPos);
        }
        if(wkc.m_zNeg && visited.count(wkc.m_zNeg) == 0) {
            wkc.m_zNeg->SetLocation(x, y, z - 1);
            next.insert(wkc.m_zNeg);
        }
        if(wkc.m_zPos && visited.count(wkc.m_zPos) == 0) {
            wkc.m_zPos->SetLocation(x, y, z + 1);
            next.insert(wkc.m_zPos);
        }
    }
    
}

void Cube::RotateNeighborX(bool ccw) {
    std::unordered_set<Cube*> visited;
    std::unordered_set<Cube*> next;
    next.insert(this);
    // rotate each connected cubes
    while(!next.empty()){
        // pop the first element in the queue
        Cube& center = **(next.begin());
        next.erase(&center);
        visited.insert(&center);
        // record the old neighors
        Cube* a0 = center.m_yPos;
        Cube* a90 = center.m_zPos;
        Cube* a180 = center.m_yNeg;
        Cube* a270 = center.m_zNeg;
        // put old neighbors to the correct orientation
        center.m_yPos = ccw ? a270 : a90;
        center.m_zPos = ccw ? a0 : a180;
        center.m_yNeg = ccw ? a90 : a270;
        center.m_zNeg = ccw ? a180 : a0;
        // add non-null & unvisited neighbor to next
        storeUnvisitedCube(&center, next, visited);
    }
    // update all cubes to the new location
    UpdateNeighborLocation();
}

void Cube::RotateNeighborY(bool ccw) {
    std::unordered_set<Cube*> visited;
    std::unordered_set<Cube*> next;
    next.insert(this);
    // rotate each connected cubes
    while(!next.empty()){
        // pop the first element in the queue
        Cube& center = **(next.begin());
        next.erase(&center);
        visited.insert(&center);
        // record the old neighors
        Cube* a0 = center.m_zPos;
        Cube* a90 = center.m_xPos;
        Cube* a180 = center.m_zNeg;
        Cube* a270 = center.m_xNeg;
        // put old neighbors to the correct orientation
        center.m_zPos = ccw ? a270 : a90;
        center.m_xPos = ccw ? a0 : a180;
        center.m_zNeg = ccw ? a90 : a270;
        center.m_xNeg = ccw ? a180 : a0;
        // add non-null & unvisited neighbor to next
        storeUnvisitedCube(&center, next, visited);
    }
    // update all cubes to the new location
    UpdateNeighborLocation();
}

void Cube::RotateNeighborZ(bool ccw) {
    std::unordered_set<Cube*> visited;
    std::unordered_set<Cube*> next;
    next.insert(this);
    // rotate each connected cubes
    while(!next.empty()){
        // pop the first element in the queue
        Cube& center = **(next.begin());
        next.erase(&center);
        visited.insert(&center);
        // record the old neighors
        Cube* a0 = center.m_xPos;
        Cube* a90 = center.m_yPos;
        Cube* a180 = center.m_xNeg;
        Cube* a270 = center.m_yNeg;
        // put old neighbors to the correct orientation
        center.m_xPos = ccw ? a270 : a90;
        center.m_yPos = ccw ? a0 : a180;
        center.m_xNeg = ccw ? a90 : a270;
        center.m_yNeg = ccw ? a180 : a0;
        // add non-null & unvisited neighbor to next
        storeUnvisitedCube(&center, next, visited);
    }
    // update all cubes to the new location
    UpdateNeighborLocation();
}

void storeUnvisitedCube(const Cube* cube, std::unordered_set<Cube*>& store, std::unordered_set<Cube*>& visited) {
    if(cube->m_xPos && visited.count(cube->m_xPos) == 0) store.insert(cube->m_xPos);
    if(cube->m_xNeg && visited.count(cube->m_xNeg) == 0) store.insert(cube->m_xNeg);
    if(cube->m_yPos && visited.count(cube->m_yPos) == 0) store.insert(cube->m_yPos);
    if(cube->m_yNeg && visited.count(cube->m_yNeg) == 0) store.insert(cube->m_yNeg);
    if(cube->m_zPos && visited.count(cube->m_zPos) == 0) store.insert(cube->m_zPos);
    if(cube->m_zNeg && visited.count(cube->m_zNeg) == 0) store.insert(cube->m_zNeg);
}

std::string PrintLocationAsCSV(Cube** cubes, unsigned long size = 1) {
    std::stringstream out;
    for(int i = 0; i < size; i++) {
        out << cubes[i]->m_xyz[0] << ',';
        out << cubes[i]->m_xyz[1] << ',';
        out << cubes[i]->m_xyz[2] << '\n';
    }
    return out.str();
}

void ExportLocationAsCSV(std::ofstream& file, Cube** cubes, unsigned long size = 1) {
    for(int i = 0; i < size; i++) {
        file << cubes[i]->m_xyz[0] << ',';
        file << cubes[i]->m_xyz[1] << ',';
        file << cubes[i]->m_xyz[2] << std::endl;
    }
}

