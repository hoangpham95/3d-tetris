//
//  Cube.hpp
//  CubesWorld
//
//  Created by Shanghao Zhong on 10/25/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//


#ifndef Cube_hpp
#define Cube_hpp


#include <stdio.h>
#include <iostream>
#include <unordered_set>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>


class Cube {
public:
    // constructor
    Cube(unsigned int CubeID);
    // destructor
    ~Cube();
    // get id
    inline unsigned int GetID() const {return m_id;};
    // get color
    inline float* const GetColor() {return m_rgba;};
    // set color
    void SetColor(float r, float g, float b, float a = 1.0f);
    // get location
    int m_x, m_y, m_z;
    // set location
    void SetLocation(int x, int y, int z);
    // return the neighboring cubes
    inline Cube* GetXNeg() const {return m_xNeg;};
    inline Cube* GetXPos() const {return m_xPos;};
    inline Cube* GetYNeg() const {return m_yNeg;};
    inline Cube* GetYPos() const {return m_yPos;};
    inline Cube* GetZNeg() const {return m_zNeg;};
    inline Cube* GetZPos() const {return m_zPos;};
    // return a vector storing the entire Cube Unit
    std::vector<Cube*>& GetCubeUnit();
    // duplicate the entire Cube Unit
    Cube* DuplicateCubeUnit();
    // update the entire Cube Unit location based on the location of this
    void UpdateCubeUnitLocation();
    // rotate the whole piece of cubes that are connecting to this cube
    // when ccw = true, it is rotating counterclockwise with the given axis
    void RotateCubeUnitOverX(bool ccw = true);
    void RotateCubeUnitOverY(bool ccw = true);
    void RotateCubeUnitOverZ(bool ccw = true);
    // export the whole piece of cubes that are connecting to this cube
    std::string PrintCubeUnitLocationAsCSV();
    void ExportCubeUnitLocationAsCSV(std::ofstream& file);
    // join neighboring cubes
    friend void JoinX(Cube* xNeg, Cube* xPos);
    friend void JoinY(Cube* zNeg, Cube* yPos);
    friend void JoinZ(Cube* zNeg, Cube* zPos);
    // overload <<
    friend std::ostream& operator<<(std::ostream& out, const Cube& cube);
    friend std::ostream& operator<<(std::ostream& out, const Cube* cube);
    // output the cube or an array of cubes as CSV
    friend std::string PrintLocationAsCSV(Cube** cubes, unsigned long size);
    friend void ExportLocationAsCSV(std::ofstream& file, Cube** cubes, unsigned long size);
private:
    // copy constructor
    // !!! doesn't work for neighbor
    Cube(const Cube& cube);
    // identifier
    const unsigned int m_id;
    // neighbors
    Cube* m_xNeg{nullptr};
    Cube* m_xPos{nullptr};
    Cube* m_yNeg{nullptr};
    Cube* m_yPos{nullptr};
    Cube* m_zNeg{nullptr};
    Cube* m_zPos{nullptr};
    // colors
    float m_rgba[4]{1.0f, 1.0f, 1.0f, 1.0f};
    // push all the neighboring Cube into a given queue, if the Cube is not in the visited set
    friend void storeUnvisitedCube(const Cube* cube, std::unordered_set<Cube*>& store, std::unordered_set<Cube*>& visited);
};
#endif /* Cube_hpp */
