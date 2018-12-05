//
//  TetrisWorld.cpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "TetrisWorld.hpp"



TetrisWorld::TetrisWorld(unsigned int x, unsigned int y, unsigned int z)
: board_x(x), board_y(y), board_z(z) {
    m_Cubes = std::deque<Cube*>();
    GenNextFigure();
}

TetrisWorld::~TetrisWorld() {}

// tetris will always going down on each tick
void TetrisWorld::Update(Direction d, Rotation r) {
    // need to talk to Zachary about the coordinates of the cube
    if (r != R_NONE) {
        Rotate(r);
    } else {
        Move(d);
    }
}

int TetrisWorld::GetPoints() { return points; }

bool TetrisWorld::IsEndGame() { return isGameOver; }

void TetrisWorld::Move(Direction d) {
    // use hasCube[i + board_x * j + board_x * board_y * k] to access the (i,j,k) location
    std::vector<bool> hasCube(board_x * board_y * board_z, false);
    for(Cube* c : m_Cubes) {
        hasCube[c->m_x + board_x * c->m_y + board_x * board_y * c->m_z] = true;
    }
    
    switch (d) {
        case D_DOWN:
            m_currCubeUnit->UpdateCubeUnitLocation(0, -1, 0);
            for(Cube* c : m_currCubeUnitArray) {
                // undo if there is conflict
                if(c->m_y < 0) {
                    m_currCubeUnit->UpdateCubeUnitLocation(0, 1, 0);
                    Merge();
                    break;
                } else if(hasCube[c->m_x + board_x * c->m_y + board_x * board_y * c->m_z]) {
                    m_currCubeUnit->UpdateCubeUnitLocation(0, 1, 0);
                    Merge();
                    break;
                }
            }
            break;
        case D_LEFT:
            m_currCubeUnit->UpdateCubeUnitLocation(-1, 0, 0);
            for(Cube* c : m_currCubeUnitArray) {
                // undo if there is conflict
                if(hasCube[c->m_x + board_x * c->m_y + board_x * board_y * c->m_z]) {
                    m_currCubeUnit->UpdateCubeUnitLocation(1, 0, 0);
                    break;
                } else if(c->m_x < 0) {
                    m_currCubeUnit->UpdateCubeUnitLocation(1, 0, 0);
                    break;
                }
            }
            break;
        case D_RIGHT:
            m_currCubeUnit->UpdateCubeUnitLocation(1, 0, 0);
            for(Cube* c : m_currCubeUnitArray) {
                // undo if there is conflict
                if(hasCube[c->m_x + board_x * c->m_y + board_x * board_y * c->m_z]) {
                    m_currCubeUnit->UpdateCubeUnitLocation(-1, 0, 0);
                    break;
                } else if(c->m_x > board_x) {
                    m_currCubeUnit->UpdateCubeUnitLocation(-1, 0, 0);
                    break;
                }
            }
            break;
        case D_FRONT:
            m_currCubeUnit->UpdateCubeUnitLocation(0, 0, -1);
            for(Cube* c : m_currCubeUnitArray) {
                // undo if there is conflict
                if(hasCube[c->m_x + board_x * c->m_y + board_x * board_y * c->m_z]) {
                    m_currCubeUnit->UpdateCubeUnitLocation(0, 0, 1);
                    break;
                } else if(c->m_z < 0) {
                    m_currCubeUnit->UpdateCubeUnitLocation(0, 0, 1);
                    break;
                }
            }
            break;
        case D_BACK:
            m_currCubeUnit->UpdateCubeUnitLocation(0, 0, 1);
            for(Cube* c : m_currCubeUnitArray) {
                // undo if there is conflict
                if(hasCube[c->m_x + board_x * c->m_y + board_x * board_y * c->m_z]) {
                    m_currCubeUnit->UpdateCubeUnitLocation(0, 0, -1);
                    break;
                } else if(c->m_z > board_z) {
                    m_currCubeUnit->UpdateCubeUnitLocation(0, 0, -1);
                    break;
                }
            }
            break;
        default:
            break;
    }
}

void TetrisWorld::Rotate(Rotation r) {
    switch (r) {
        case R3_XCW:
            m_currCubeUnit->RotateCubeUnitOverX(false);
            break;
        case R3_XCCW:
            m_currCubeUnit->RotateCubeUnitOverX(true);
            break;
        case R3_YCW:
            m_currCubeUnit->RotateCubeUnitOverY(false);
            break;
        case R3_YCCW:
            m_currCubeUnit->RotateCubeUnitOverY(true);
            break;
        case R3_ZCW:
            m_currCubeUnit->RotateCubeUnitOverZ(false);
            break;
        case R3_ZCCW:
            m_currCubeUnit->RotateCubeUnitOverZ(true);
            break;
        default:
            break;
    }
}


void TetrisWorld::Merge() {
    //  std::cout << "Merge" << std::endl;
    for (Cube* c : m_currCubeUnitArray) {
        m_Cubes.push_back(c);
    }
    m_currCubeUnitArray = std::vector<Cube*>();
    m_currCubeUnit = nullptr;
    
    int streak = 1;
    while (Collapse()) {
        points += board_x * 10 * streak;
        streak += 1;
    }
    
    streak = 0;
    float highest_y = 0.0f;
    for (int i = 0; i < m_Cubes.size(); i++) {
        highest_y = std::max(highest_y, m_Cubes[i]->m_y);
    }
    
    if (highest_y >= board_y) {
        isGameOver = true;
    } else {
        GenNextFigure();
    }
}



// generate next figure, need to talk to Zachary
void TetrisWorld::GenNextFigure() {
    m_currCubeUnit = CubeUnitFactory::instance().NewRandomCubeUnit();
    m_currCubeUnitArray = m_currCubeUnit->GetCubeUnit();
    std::sort(m_currCubeUnitArray.begin(),
              m_currCubeUnitArray.end(),
              TetrisWorld::compareYX);
    // reassign center the the kind-of middle one
    m_currCubeUnit = m_currCubeUnitArray[m_currCubeUnitArray.size()/2];
    
    Cube* highest = nullptr;
    for(Cube* c : m_currCubeUnitArray) {
        if(!highest) {
            highest = c;
        } else {
            if(c->m_y > highest->m_y) {
                highest = c;
            }
        }
    }
    highest->SetLocation(board_x/2, board_y, board_z/2);
    highest->UpdateCubeUnitLocation();
}



bool TetrisWorld::Collapse() {
    std::sort(m_Cubes.begin(), m_Cubes.end(), TetrisWorld::compareY);

    // not enough cube, return false
    if(m_Cubes.size() < board_x * board_z) {
        return false;
    }
    
    // group cubes by y layer
    std::vector<std::vector<Cube*>> layerCubes;
    for(Cube* c : m_Cubes) {
        layerCubes[c->m_y].push_back(c);
    }
    
    bool isCollapse = false;
    int deletingLayer;
    
    // find the first deleted layer
    for(int i = 0; i < board_y; ++i) {
        if(layerCubes[i].size() == board_x * board_z) {
            isCollapse = true;
            deletingLayer = i;
            break;
        }
    }
    
    // return if not collapse
    if(!isCollapse) {
        return false;
    }
    
    // collapsing
    int beginDeleteIdx = 0;
    for(int i = 0; i < deletingLayer; ++i) {
        beginDeleteIdx += layerCubes[i].size();
    }
    
    // change cube location above the layer
    for(unsigned short i = beginDeleteIdx + board_x * board_z;
        i < m_Cubes.size();
        ++i) {
        m_Cubes[i]->m_y--;
    }
    
    // delete the cubes in m_cubes
    m_Cubes.erase(m_Cubes.begin()+beginDeleteIdx,
                  m_Cubes.begin()+beginDeleteIdx + board_x * board_z);
    
    // deallocated deleted cubes
    for(Cube* c : layerCubes[deletingLayer]) {
        delete c;
    }
    
    return true;
}

std::vector<Cube*> TetrisWorld::GetAllCubes() {
    std::vector<Cube*> res;
    
    std::cout << m_Cubes.size();
    for (int i = 0; i < m_Cubes.size(); i++) {
        res.push_back(m_Cubes[i]);
    }
    
    for (Cube* c : m_currCubeUnitArray) {
        res.push_back(c);
    }
    
    return res;
}
