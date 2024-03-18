#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/skybox.h"

#include "glm/glm.hpp"


using glm::mat4;
class SceneBasic_Uniform : public Scene
{
private:


    SkyBox skybox;
    
    float angle;
    float rotSpeed;
    float tPrev;


    std::unique_ptr<ObjMesh> mesh;
    std::unique_ptr<ObjMesh> Plane;
    std::unique_ptr<ObjMesh> crystal1;
    std::unique_ptr<ObjMesh> crystal2;
    std::unique_ptr<ObjMesh> crystal3;
    std::unique_ptr<ObjMesh> crystal4;

    GLSLProgram prog;
    void setMatrices();
    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
