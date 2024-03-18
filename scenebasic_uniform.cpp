#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>

//GLAD & GLFW
#include<GLAD/glad.h>
#include <GLFW/glfw3.h>

//GLM
#include "glm/ext/vector_float3.hpp"
#include <glm/gtc/type_ptr.hpp> //Access to value_ptr

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#include "helper/texture.h"
#include <scenerunner.h>


using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::mat3;

GLuint programID;

float rotationSpeed = 0.5f;
float rotationAngle = 0.0f;

                                           // torus(0.7f, 0.3f, 30, 30) 
                                           // plane(10.0f, 10.0f, 100, 100)
                                           // slybox
                                           // angle(90.0f),tPrev(0), rotSpeed(glm::pi<float>() / 8.0f), skybox(50.0f)
SceneBasic_Uniform::SceneBasic_Uniform() : angle(90.0f), tPrev(0), rotSpeed(glm::pi<float>() / 8.0f), skybox(100.0f) {

    float angle;

   // Mesh's
    mesh = ObjMesh::load("media/meshes/Rock.obj");
    Plane = ObjMesh::load("media/meshes/plane.obj");
    crystal1 = ObjMesh::load("media/meshes/Crystal1.obj");
    crystal2 = ObjMesh::load("media/meshes/Crystal2.obj");
    crystal3 = ObjMesh::load("media/meshes/Crystal3.obj");
    crystal4 = ObjMesh::load("media/meshes/Crystal4.obj");

}

void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    // Light position 

    prog.setUniform("Light.L", vec3(-8.0f, 0.0f, -10.0f));
    prog.setUniform("Light.La", vec3(0.2f));

    // Fog 

    prog.setUniform("Fog.MaxDist", 60.0f);
    prog.setUniform("Fog.MinDist", 35.0f);
    prog.setUniform("Fog.Color", vec3(0.0f, 0.0f, 0.2f));


    // Camera setup

    model = mat4(1.0f);
    projection = mat4(1.0f);

}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");

		prog.link();
		prog.use();

        programID = prog.getHandle();

	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{

    // Define rotation parameters
    static float radius = 20.0f;  // Fixed radius
    static float speed = 0.4f;    // Speed of rotation

    // Calculate the target angle based on time
    float targetAngle = speed * t;

    // Smoothly interpolate between previous and target angle
    static float currentAngle = 0.0f;  // Initialize current angle
    float angle = glm::mix(currentAngle, targetAngle, 0.1f); //smoothness

    // Update current angle for the next frame
    currentAngle = angle;

    // new light position 
    float x = radius * cos(angle);
    float y = 7.0f;  
    float z = radius * sin(angle);


    // Update the light position uniform
    glm::vec4 lightPosition = view * glm::vec4(0.0f, 0.0f, -20.0f, 1.0f);

    prog.setUniform("Light.Position", vec4(x, y, z, 1.0f));

}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ---

    float angle = glm::radians(float(glfwGetTime())); 

    glm::vec3 axis(0.0f, 1.0f, 1.0f); // Y-axis as the rotation axis

    // Construct rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);

    // Apply rotation to the original light position
    glm::vec4 lightPos = rotationMatrix * glm::vec4(-30.0f, 10.0f, 10.0f, 1.0f);


    // Pass updated light position to shader
    prog.setUniform("Light.Position", glm::vec3(lightPos));
    prog.setUniform("Light.La", vec3(0.3f));
    prog.setUniform("Light.Ld", vec3(0.5f));




    //-------------------------------
    // 
    // Camera 

    vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(vec3(10.0f, 3.0f, 1.25f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    //-------------------------------

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Ka", vec3(0.2f*0.3f, 0.55f*0.3f, 0.9f*0.3f));

    prog.setUniform("Material.Shininess", 100.0f);

    //-----------
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
    model  = glm::scale(model, vec3(50.0f));

    //--------------------------------------------------------------------
    //--------------------------------------------------------------------

    // Rock
                  
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, vec3(2.42f));

    // Texture Loading

    GLuint rockTexture = Texture::loadTexture("media/texture/rock.jpg");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rockTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    mesh->render();

    //-----------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(0.0f, 90.0f, 0.0f));
    model = glm::translate(model, vec3(2.0f, 0.0f, 2.6f));
    model = glm::scale(model, vec3(1.0f));

    GLuint crystalTexture = Texture::loadTexture("media/texture/crystal2.jpg");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material - Colours the crystals 
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal1->render();

    //--------------------------------------------------------------------------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(360.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(2.0f, 0.0f, -2.6f));
    model = glm::scale(model, vec3(0.78f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal2->render();

    //--------------------------------------------------------------------------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(20.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 1.6f, 0.0f));
    model = glm::scale(model, vec3(0.80f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);


    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 180.0f);

    setMatrices();
    crystal3->render();

    //--------------------------------------------------------------------------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(3.8f, 0.0f, 0.6f));
    model = glm::scale(model, vec3(0.58f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal4->render();

    //--------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------

    //-----------

    // Rock1

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(-20.0f, 0.0f, 3.0f));
    model = glm::scale(model, vec3(3.0f));


    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rockTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    setMatrices();
    mesh->render();

    //-------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(10.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.0f, 10.6f));
    model = glm::scale(model, vec3(0.5f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material - Colours the crystals 
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal1->render();

    //--------------------------------------------------------------------------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(360.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(-15.0f, 0.0f, 9.6f));
    model = glm::scale(model, vec3(1.0f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);


    setMatrices();
    crystal2->render();

    //--------------------------------------------------------------------------------


    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(3.8f, 0.0f, 0.6f));
    model = glm::scale(model, vec3(0.58f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal4->render();

    //--------------------------------------------------------------------------------
    //--------- GEODE 2
    //--------------------------------------------------------------------------------

    // Rock2

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(10.0f, 0.0f, -20.0f));
    model = glm::scale(model, vec3(3.0f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rockTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    mesh->render();

    //--------------------------------------------------------------------------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(0.0f, 90.0f, 0.0f));
    model = glm::translate(model, vec3(-11.0f, 1.5f, 15.6f));
    model = glm::scale(model, vec3(2.0f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material - Colours the crystals 
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal1->render();

    //--------------------------------------------------------------------------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(360.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(-5.0f, 0.0f, -23.6f));
    model = glm::scale(model, vec3(2.5f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal2->render();

    //--------------------------------------------------------------------------------


    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(360.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(5.0f, 0.0f, -6.6f));
    model = glm::scale(model, vec3(0.5f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);


    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal2->render();

    //--------------------------------------------------------------------------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(40.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(-10.0f, 0.0f, -20.0f));
    model = glm::scale(model, vec3(1.0f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 180.0f);

    setMatrices();
    crystal3->render();

    //--------------------------------------------------------------------------------

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(-8.0f, 0.0f, -16.6f));
    model = glm::scale(model, vec3(1.5f));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, crystalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    //specular material
    prog.setUniform("Material.Kd", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ka", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));

    prog.setUniform("Material.Shininess", 200.0f);

    setMatrices();
    crystal4->render();

    //--------------------------------------------------------------------------------

    prog.use();
    model = mat4(1.0f);

    GLuint rockFloorTexture = Texture::loadTexture("media/texture/rock-floor.jpg");

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rockFloorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    prog.setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
    prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.2f, 0.2f));

    prog.setUniform("Material.Shininess", 180.0f);

    model = glm::scale(model, vec3(5.0f));

    setMatrices();
    Plane -> render();

    //------
    model = mat4(1.0f);
    prog.use();


        // "media/texture/cube/pisa-hdr/pisa" , /media/Cubemap/Space-hdr
    GLuint cubeTex = Texture::loadCubeMap("media/Cubemap/Space/Space");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

    setMatrices();
    skybox.render();
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);

    width = w;
    height = h;

    projection=glm::perspective(glm::radians(90.0f), (float)w/h, 0.5f, 100.0f);
}

void SceneBasic_Uniform::setMatrices() {

    mat4 mv = view * model;

    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);

}