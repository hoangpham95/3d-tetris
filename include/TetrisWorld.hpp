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

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include "Cube.hpp"

class TetrisWorld {
 public:
  enum Direction { D_DOWN, D_LEFT, D_RIGHT };
  enum Rotation { R_NONE, R_LEFT, R_RIGHT };

  TetrisWorld(unsigned int, unsigned int, unsigned int);
  ~TetrisWorld();

  void Update(Direction, Rotation);
  void init();
  void getPoints();
  void isEndGame();

 private:
  unsigned int board_x, board_y, board_z;
  unsigned int points{0};

  void genNextFigure();
  std::vector<Cube*> m_CurrentMovingCubes;
  std::vector<Cube*> m_Cubes;
  bool isGameOver;

  bool isColliding();
  void merge();     // if colliding, merge current moving cubes to all cubes
  void collapse();  // action when 1 surface has been filled 
  void move(Direction);
  void rotate(Rotation);
};

#endif /* TetrisWorld_hpp */
