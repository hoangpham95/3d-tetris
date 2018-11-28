//
//  TetrisWorld.hpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#ifndef TetrisWorld_hpp
#define TetrisWorld_hpp

#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Cube.hpp"

class TetrisWorld {
 public:
  enum Direction { D_UP, D_DOWN, D_LEFT, D_RIGHT, D_FRONT, D_BACK, D_NONE };
  enum Rotation { R_UP, R_DOWN, R_LEFT, R_RIGHT, R_FRONT, R_BACK, R_NONE };

  void Update(Direction, Rotation);
  void init();
  void getPoints();
  void isEndGame();

 private:
  void genNextFigure();
  std::vector<Cube*> m_CurrentMovingCubes;
  std::vector<Cube*> m_Cubes;
  int points{0};
  bool isColliding();
  void merge();     // if colliding, merge current moving cubes to all cubes
  bool collapse();  // action when 1 surface has been filled
};

#endif /* TetrisWorld_hpp */
