#ifndef SCENENODE_H
#define SCENENODE_H
/** @file SceneNode.h
 *  @brief SceneNode helps organize a large 3D graphics scene.
 *
 *  SceneNode helps organize a large 3D graphics scene.
 *  The traversal of the tree takes place starting from
 *  a single SceneNode (typically called root).
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#include <vector>

#include "Camera.h"
#include "Object.h"
#include "Shader.h"
#include "Transform.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"

class SceneNode {
public:
    // A SceneNode is created by taking
    // a pointer to an object.
    SceneNode(Object* ob);
    // Our destructor takes care of destroying
    // all of the children within the node.
    // Now we do not have to manage deleting
    // each individual object.
    virtual ~SceneNode();
    // initialize the shader of this scenenode
    virtual void init();
    // Adds a child node to our current node.
    virtual void AddChild(SceneNode* n);
    // Draws the current SceneNode
    virtual void Draw();
    // Updates the current SceneNode
    virtual void Update(glm::mat4 projectionMatrix, Camera* camera);
    // Returns the local transformation transform
    // Remember that local is local to an object, where it's center is the origin.
    Transform& getLocalTransform();
    // Returns a SceneNode's world transform
    Transform& getWorldTransform();
    // For now we have one shader per object.
    Shader myShader;
protected:
    // Parent
    SceneNode* parent;
    // Children holds all a pointer to all of the descendents
    // of a particular SceneNode. A pointer is used because
    // we do not want to hold or make actual copies.
    std::vector<SceneNode*> children;
    // The object stored in the scene graph
    Object* object;
    // Each SceneNode nodes locals transform.
    Transform localTransform;
    // We additionally can store the world transform
    Transform worldTransform;

};

#endif
