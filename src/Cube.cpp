//
//  Cube.cpp
//  CubesWorld
//
//  Created by Shanghao Zhong on 10/25/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "Cube.hpp"
#include <unordered_map>

Cube::Cube(unsigned int id) : m_id(id) {}

Cube::Cube(float x, float y, float z) : m_x(x), m_y(y), m_z(z), m_id(1) {}

Cube::~Cube() {
  // prevent its neighbor from accessing random memory
  if (m_xNeg) m_xNeg->m_xPos = nullptr;
  if (m_xPos) m_xPos->m_xNeg = nullptr;
  if (m_yNeg) m_yNeg->m_yPos = nullptr;
  if (m_yPos) m_yPos->m_yNeg = nullptr;
  if (m_zNeg) m_zNeg->m_zPos = nullptr;
  if (m_zPos) m_zPos->m_zNeg = nullptr;
  // setting its neighbor to
}

void JoinX(Cube* xNeg, Cube* xPos) {
  if (xNeg) {
    xNeg->m_xPos = xPos;
  }
  if (xPos) {
    xPos->m_xNeg = xNeg;
  }
}

void JoinY(Cube* yNeg, Cube* yPos) {
  if (yNeg) {
    yNeg->m_yPos = yPos;
  }
  if (yPos) {
    yPos->m_yNeg = yNeg;
  }
}

void JoinZ(Cube* zNeg, Cube* zPos) {
  if (zNeg) {
    zNeg->m_zPos = zPos;
  }
  if (zPos) {
    zPos->m_zNeg = zNeg;
  }
}

std::ostream& operator<<(std::ostream& out, const Cube& cube) {
  out << "Cube(id:" << cube.m_id;
  out << "|xyz:" << cube.m_x << "," << cube.m_y << "," << cube.m_z << ")";
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

void Cube::SetLocation(float x, float y, float z) {
  m_x = x;
  m_y = y;
  m_z = z;
}

std::vector<Cube*>& Cube::GetCubeUnit() {
  // initialize output
  std::vector<Cube*>* output = new std::vector<Cube*>();
  output->push_back(this);

  std::unordered_set<Cube*> visited;
  std::unordered_set<Cube*> next;
  next.insert(this);

  while (!next.empty()) {
    // pop the first element in the queue as the working cube
    Cube& wkc = **(next.begin());
    next.erase(&wkc);
    // add the working cube into output
    output->push_back(&wkc);
    // add the working cube into visited
    visited.insert(&wkc);
    // add each of the unvisited neighboring cubes to visited
    if (wkc.m_xNeg && visited.count(wkc.m_xNeg) == 0) {
      next.insert(wkc.m_xNeg);
    }
    if (wkc.m_xPos && visited.count(wkc.m_xPos) == 0) {
      next.insert(wkc.m_xPos);
    }
    if (wkc.m_yNeg && visited.count(wkc.m_yNeg) == 0) {
      next.insert(wkc.m_yNeg);
    }
    if (wkc.m_yPos && visited.count(wkc.m_yPos) == 0) {
      next.insert(wkc.m_yPos);
    }
    if (wkc.m_zNeg && visited.count(wkc.m_zNeg) == 0) {
      next.insert(wkc.m_zNeg);
    }
    if (wkc.m_zPos && visited.count(wkc.m_zPos) == 0) {
      next.insert(wkc.m_zPos);
    }
  }

  return *output;
}

void Cube::UpdateCubeUnitLocation() {
  std::unordered_set<Cube*> visited;
  std::unordered_set<Cube*> next;
  next.insert(this);

  while (!next.empty()) {
    // pop the first element in the queue
    Cube& wkc = **(next.begin());
    next.erase(&wkc);
    visited.insert(&wkc);
    // record the location
    int x = wkc.m_x;
    int y = wkc.m_y;
    int z = wkc.m_z;
    // update each of the unvisited neighboring cubes
    if (wkc.m_xNeg && visited.count(wkc.m_xNeg) == 0) {
      wkc.m_xNeg->SetLocation(x - 1, y, z);
      next.insert(wkc.m_xNeg);
    }
    if (wkc.m_xPos && visited.count(wkc.m_xPos) == 0) {
      wkc.m_xPos->SetLocation(x + 1, y, z);
      next.insert(wkc.m_xPos);
    }
    if (wkc.m_yNeg && visited.count(wkc.m_yNeg) == 0) {
      wkc.m_yNeg->SetLocation(x, y - 1, z);
      next.insert(wkc.m_yNeg);
    }
    if (wkc.m_yPos && visited.count(wkc.m_yPos) == 0) {
      wkc.m_yPos->SetLocation(x, y + 1, z);
      next.insert(wkc.m_yPos);
    }
    if (wkc.m_zNeg && visited.count(wkc.m_zNeg) == 0) {
      wkc.m_zNeg->SetLocation(x, y, z - 1);
      next.insert(wkc.m_zNeg);
    }
    if (wkc.m_zPos && visited.count(wkc.m_zPos) == 0) {
      wkc.m_zPos->SetLocation(x, y, z + 1);
      next.insert(wkc.m_zPos);
    }
  }
}

void Cube::RotateCubeUnitOverX(bool ccw) {
  std::unordered_set<Cube*> visited;
  std::unordered_set<Cube*> next;
  next.insert(this);
  // rotate each connected cubes
  while (!next.empty()) {
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
  UpdateCubeUnitLocation();
}

void Cube::RotateCubeUnitOverY(bool ccw) {
  std::unordered_set<Cube*> visited;
  std::unordered_set<Cube*> next;
  next.insert(this);
  // rotate each connected cubes
  while (!next.empty()) {
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
  UpdateCubeUnitLocation();
}

void Cube::RotateCubeUnitOverZ(bool ccw) {
  std::unordered_set<Cube*> visited;
  std::unordered_set<Cube*> next;
  next.insert(this);
  // rotate each connected cubes
  while (!next.empty()) {
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
  UpdateCubeUnitLocation();
}

void storeUnvisitedCube(const Cube* cube, std::unordered_set<Cube*>& store,
                        std::unordered_set<Cube*>& visited) {
  if (cube->m_xPos && visited.count(cube->m_xPos) == 0)
    store.insert(cube->m_xPos);
  if (cube->m_xNeg && visited.count(cube->m_xNeg) == 0)
    store.insert(cube->m_xNeg);
  if (cube->m_yPos && visited.count(cube->m_yPos) == 0)
    store.insert(cube->m_yPos);
  if (cube->m_yNeg && visited.count(cube->m_yNeg) == 0)
    store.insert(cube->m_yNeg);
  if (cube->m_zPos && visited.count(cube->m_zPos) == 0)
    store.insert(cube->m_zPos);
  if (cube->m_zNeg && visited.count(cube->m_zNeg) == 0)
    store.insert(cube->m_zNeg);
}

std::string Cube::PrintCubeUnitLocationAsCSV() {
  std::stringstream out;
  for (Cube* c : GetCubeUnit()) {
    out << c->m_x << ',';
    out << c->m_y << ',';
    out << c->m_z << '\n';
  }
  return out.str();
}

void Cube::ExportCubeUnitLocationAsCSV(std::ofstream& file) {
  for (Cube* c : GetCubeUnit()) {
    file << c->m_x << ',';
    file << c->m_y << ',';
    file << c->m_z << '\n';
  }
}

std::string PrintLocationAsCSV(Cube** cubes, unsigned long size = 1) {
  std::stringstream out;
  for (int i = 0; i < size; i++) {
    out << cubes[i]->m_x << ',';
    out << cubes[i]->m_y << ',';
    out << cubes[i]->m_z << '\n';
  }
  return out.str();
}

void ExportLocationAsCSV(std::ofstream& file, Cube** cubes,
                         unsigned long size = 1) {
  for (int i = 0; i < size; i++) {
    file << cubes[i]->m_x << ',';
    file << cubes[i]->m_y << ',';
    file << cubes[i]->m_z << std::endl;
  }
}

Cube::Cube(const Cube& cube) : m_id(cube.m_id) {
  for (int i = 0; i < 4; ++i) {
    m_rgba[i] = cube.m_rgba[i];
  }
  m_x = cube.m_x;
  m_y = cube.m_y;
  m_z = cube.m_z;
}

Cube* Cube::DuplicateCubeUnit() {
  std::vector<Cube*> cubeUnit = GetCubeUnit();
  std::unordered_map<Cube*, int> cubeUnitIdx;
  std::vector<Cube*> dupCubeUnit;
  dupCubeUnit.reserve(cubeUnit.size());

  for (int i = 0; i < cubeUnit.size(); ++i) {
    cubeUnitIdx[cubeUnit[i]] = i;
    Cube* tmp = new Cube(*cubeUnit[i]);
    dupCubeUnit.push_back(tmp);
  }

  for (int i = 0; i < cubeUnit.size(); ++i) {
    Cube* wkc = cubeUnit[i];
    Cube* dupWkc = dupCubeUnit[i];
    // register neighbor
    int tmp;
    if (wkc->m_xNeg) {
      tmp = cubeUnitIdx[wkc->m_xNeg];
      dupWkc->m_xNeg = dupCubeUnit[tmp];
    }
    if (wkc->m_xPos) {
      tmp = cubeUnitIdx[wkc->m_xPos];
      dupWkc->m_xPos = dupCubeUnit[tmp];
    }
    if (wkc->m_yNeg) {
      tmp = cubeUnitIdx[wkc->m_yNeg];
      dupWkc->m_yNeg = dupCubeUnit[tmp];
    }
    if (wkc->m_yPos) {
      tmp = cubeUnitIdx[wkc->m_yPos];
      dupWkc->m_yPos = dupCubeUnit[tmp];
    }
    if (wkc->m_zNeg) {
      tmp = cubeUnitIdx[wkc->m_zNeg];
      dupWkc->m_zNeg = dupCubeUnit[tmp];
    }
    if (wkc->m_zPos) {
      tmp = cubeUnitIdx[wkc->m_zPos];
      dupWkc->m_zPos = dupCubeUnit[tmp];
    }
  }

  return dupCubeUnit[0];
}
