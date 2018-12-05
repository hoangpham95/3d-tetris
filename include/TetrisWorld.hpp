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

class TetrisWorld {
 public:
  TetrisWorld(unsigned int, unsigned int, unsigned int);
  ~TetrisWorld();

  void Update(Direction, Rotation);
  int GetPoints();
  bool IsEndGame();

  std::vector<Cube*> GetAllCubes();

 private:
  unsigned int board_x, board_y, board_z;
  unsigned int points{0};

  void GenNextFigure();
  Tetromino* m_CurrentMovingCubes;
  std::vector<Cube*> m_Cubes;
  bool isGameOver;

  bool IsColliding();
  void Merge();     // if colliding, merge current moving cubes to all cubes
  bool Collapse();  // action when 1 surface has been filled
  void Move(Direction);
  void Rotate(Rotation);
};

#endif /* TetrisWorld_hpp */
