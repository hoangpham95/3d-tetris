//
//  SceneNodeCube.cpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/29/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "SceneNodeCube.hpp"

SceneNodeCube::SceneNodeCube(ObjectCube* obCube, Cube* cube) : SceneNode(obCube), m_cube(cube) {
    
}

void SceneNodeCube::init() {
    // Setup shaders for the node.
    std::string vertexShader = myShader.LoadShader("./shaders/cube_vert.glsl");
    std::string fragmentShader = myShader.LoadShader("./shaders/cube_frag.glsl");
    // Actually create our shader
    myShader.CreateShader(vertexShader, fragmentShader);
}

void SceneNodeCube::Update(glm::mat4 projectionMatrix, Camera *camera) {
    // return if m_cube is nullptr
    if(!m_cube) {
        return;
    }
    
    // set location tranformation based on m_cube
    localTransform.loadIdentity();
    localTransform.translate(m_cube->GetLocation()[0],
                             m_cube->GetLocation()[1],
                             m_cube->GetLocation()[2]);
    
    // set world transformation
    if(parent) { // if there is a parent node
        worldTransform = parent->getWorldTransform() * localTransform;
    } else {
        worldTransform = localTransform;
    }
}
