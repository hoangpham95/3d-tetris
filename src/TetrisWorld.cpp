//
//  TetrisWorld.cpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "TetrisWorld.hpp"

TetrisWorld::TetrisWorld(unsigned int x, unsigned int y, unsigned int z): 
  board_x(x), board_y(y), board_z(z) {}

TetrisWorld::~TetrisWorld() {}

// tetris will always going down on each tick
void TetrisWorld::Update(Direction d, Rotation r) {
  // need to talk to Zachary about the coordinates of the cube
  move(d);
  rotate(r);
}

void TetrisWorld::move(Direction d) {
  switch (d) {
    case D_DOWN:
      if (isColliding()) {
        merge();
      } else {
        int min_y = INT_MAX, min_x = 0;
        for (int i = 0; i < m_CurrentMovingCubes.size(); i++) {
          m_CurrentMovingCubes[i]->m_y -= 1;
        }
      }
      break;
    case D_LEFT:
      for (int i = 0; i < m_CurrentMovingCubes.size(); i++) {
        Cube* curr = m_CurrentMovingCubes[i];
        if (curr->m_x == 0) {
          return;
        } else {
          for (int j = 0; i < m_Cubes.size(); j++) {
            Cube* c = m_Cubes[j];
            if (c->m_y == curr->m_y && c->m_x + 1 == curr->m_x) {
              return;
            }
          }
        }
      }

      for (int i = 0; i < m_CurrentMovingCubes.size(); i++) {
        m_CurrentMovingCubes[i]->m_x -= 1;
      }
      break;
    case D_RIGHT:
      for (int i = 0; i < m_CurrentMovingCubes.size(); i++) {
        Cube* curr = m_CurrentMovingCubes[i];
        if (curr->m_x == board_y - 1) {
          return;
        } else {
          for (int j = 0; i < m_Cubes.size(); j++) {
            Cube* c = m_Cubes[j];
            if (c->m_y == curr->m_y && c->m_x - 1 == curr->m_x) {
              return;
            }
          }
        }
      }

      for (int i = 0; i < m_CurrentMovingCubes.size(); i++) {
        m_CurrentMovingCubes[i]->m_x += 1;
      }
      break;
  }
}

void TetrisWorld::rotate(Rotation r) {}

bool TetrisWorld::isColliding() {
  int min_y = INT_MAX, min_x = 0;
  for (int i = 0; i < m_CurrentMovingCubes.size(); i++) {
    if (m_CurrentMovingCubes[i]->m_y < board_y) {
      min_y = m_CurrentMovingCubes[i]->m_y;
      min_x = m_CurrentMovingCubes[i]->m_x;
    }
  }
  for (int i = 0; i < m_Cubes.size(); i++) {
    Cubes* current = m_Cubes[i];
    if (current->m_x == min_x && current->m_y + 1 == min_y) {
      return true;
    }
  }

  return false;
}

void TetrisWorld::merge() {
  for (int i = 0; i < m_CurrentMovingCubes.size(); i++) {
    
  }

  int streak = 1;
  while (collapse()) {
    points += board_x * 10 * streak;
    streak += 1;
  }

  streak = 0;
  genNextFigure();
}

// generate next figure, need to talk to Zachary
void TetrisWorld::genNextFigure() {
  
}

bool compare(Cube* x, Cube* y) {
  if (x->m_y == y->m_y)
    return x->m_x < y->m_y;
  
  return x->m_y < y->m_y;
}

bool TetrisWorld::collapse() {
  std::sort(m_Cubes.begin(), m_Cubes.end(), compare);

  int count = 0;
  for (int i = 0; i < m_Cubes.size(); i++) {
    if (m_Cubes[i]->m_y == 0) count++;
  }

  if (count == board_x) {
    for (int i = 0; i < board_x; i++) {
      m_Cubes.pop_front();
    }

    for (int j = 0; j < m_Cubes.size(); j++) {
      m_Cubes[j]->m_y -= 1;
    }

    return true;
  }

  return false;
}