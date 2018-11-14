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
#include <sstream>
#include <fstream>


// if true: Cube(id:..|xyz:...)
// if false: id:..|xyz:...
bool CUBE_PrintType = true; // it wiil print "Cube(...)"
// if true: Cube(id:..|xyz:...)
// if false: Cube(..|...)
bool CUBE_PrintDataName = true;
// print id or not
bool CUBE_PrintID = true;
// print location or not
bool CUBE_PrintLocation = true;
// print color or not
bool CUBE_PrintColor = false;
// print neighbors' ID or not
bool CUBE_PrintNeighborID = false;
// seperator between each data
char CUBE_PrimarySeperator = '|';
// seperatpr between values of the same data
char CUBE_SecondarySeperator = ',';

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
    inline int* const GetLocation() {return m_xyz;};
    // set location
    void SetLocation(int x, int y, int z);
    // update neighbor cube location based on the given center
    void UpdateNeighborLocation();
    // return the neighboring cubes
    inline Cube* GetXNeg() const {return m_xNeg;};
    inline Cube* GetXPos() const {return m_xPos;};
    inline Cube* GetYNeg() const {return m_yNeg;};
    inline Cube* GetYPos() const {return m_yPos;};
    inline Cube* GetZNeg() const {return m_zNeg;};
    inline Cube* GetZPos() const {return m_zPos;};
    // rotate the whole piece of cubes that are connecting to this cube
    // when ccw = true, it is rotating counterclockwise with the given axis
    void RotateNeighborX(bool ccw = true);
    void RotateNeighborY(bool ccw = true);
    void RotateNeighborZ(bool ccw = true);
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
    // locations
    int m_xyz[3]{0,0,0};
    // push all the neighboring Cube into a given queue, if the Cube is not in the visited set
    friend void storeUnvisitedCube(const Cube* cube, std::unordered_set<Cube*>& store, std::unordered_set<Cube*>& visited);
    // if true: Cube(id:..|xyz:...)
    // if false: id:..|xyz:...
    bool CUBE_PrintType = true; // it wiil print "Cube(...)"
    // if true: Cube(id:..|xyz:...)
    // if false: Cube(..|...)
    bool CUBE_PrintDataName = true;
    // print id or not
    bool CUBE_PrintID = true;
    // print location or not
    bool CUBE_PrintLocation = true;
    // print color or not
    bool CUBE_PrintColor = false;
    // print neighbors' ID or not
    bool CUBE_PrintNeighborID = false;
    // seperator between each data
    char CUBE_PrimarySeperator = '|';
    // seperatpr between values of the same data
    char CUBE_SecondarySeperator = ',';
};
#endif /* Cube_hpp */
