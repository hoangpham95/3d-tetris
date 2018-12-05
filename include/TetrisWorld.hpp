//
//  TetrisWorld.hpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#ifndef TetrisWorld_hpp
#define TetrisWorld_hpp

#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else  // This works for Mac
#include <SDL.h>
#endif

#include <Constants.h>
#include <Tetromino.h>
#include <stdio.h>
#include <Cube.hpp>
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
#include <random>
#include <deque>

#include "CubeUnitFactory.hpp"

class TetrisWorld {
 public:
  TetrisWorld(unsigned int, unsigned int, unsigned int);
  ~TetrisWorld();

  void Update(Direction, Rotation);
  int GetPoints();
  bool IsEndGame();
    inline unsigned int GetX() { return board_x;}
    inline unsigned int GetY() { return board_y;}
    inline unsigned int GetZ() { return board_z;}
  std::vector<Cube*> GetAllCubes();

 private:
  unsigned int board_x, board_y, board_z;
  unsigned int points{0};

  void GenNextFigure();
    Cube* m_currCubeUnit;
    std::vector<Cube*> m_currCubeUnitArray;
  std::deque<Cube*> m_Cubes;
    bool isGameOver{false};

  bool IsColliding();
  void Merge();     // if colliding, merge current moving cubes to all cubes
  bool Collapse();  // action when 1 surface has been filled
  void Move(Direction);
  void Rotate(Rotation);
   inline static bool compareY(Cube* c1, Cube* c2) { return c1->m_y < c2->m_y;}
    inline static bool compareYX(Cube* c1, Cube* c2) {
        return (c1->m_y < c2->m_y)? true : (c1->m_x < c2->m_x);
    }
};

#endif /* TetrisWorld_hpp */
