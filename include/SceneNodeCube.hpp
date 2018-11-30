//
//  SceneNodeCube.hpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/29/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#ifndef SceneNodeCube_hpp
#define SceneNodeCube_hpp

#include <stdio.h>

#include "SceneNode.h"
#include "ObjectCube.hpp"
#include "Cube.hpp"


class SceneNodeCube final : public SceneNode {
public:
    // this will take in a ObjectCube and a Cube
    // ObjectCube is responsible for render a cube in OpenGL
    // Cube is responsible to provide information about the location and the color of the cube.
    SceneNodeCube(ObjectCube* obCube, Cube* cube);
    // Load shader, it will load the cube_vert.glsl and cube_frag.glsl
    void init() override;
    // update the transformation based on m_cube, projection matrix and camera
    void Update(glm::mat4 projectionMatrix, Camera* camera) override;
    
private:
    Cube* m_cube;
    
    
};

#endif /* SceneNodeCube_hpp */
