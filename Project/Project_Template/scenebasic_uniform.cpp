#include "scenebasic_uniform.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

#include <sstream>

const float radius = 10.0f;



//constructor for torus
SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 10.0f, 100, 100) 
{
    mesh = ObjMesh::loadWithAdjacency("media/Craig.obj",
        true);
}



//constructor for teapot
//SceneBasic_Uniform::SceneBasic_Uniform() : teapot(13, glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f))) {}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(vec3(10.0f, 10.25f, 10.25f), vec3(0.0f, 10.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    projection = mat4(1.0f);
    float x, z;
    for (int i = 0; i < 3; i++)
    {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z +
            1.0f, 1.0f));
    }

    prog.setUniform("EdgeWidth", 0.015f);
    prog.setUniform("PctExtend", 0.0005f);
    prog.setUniform("LineColor", vec4(0.05f, 0.0f, 0.05f, 1.0f));

    prog.setUniform("Lights[0].L", vec3(0.0f, 0.0f, 1.0f));
    prog.setUniform("Lights[1].L", vec3(0.0f, 1.0f, 0.0f));
    prog.setUniform("Lights[2].L", vec3(1.0f, 0.0f, 0.0f));
    prog.setUniform("Lights[0].La", vec3(0.2f, 0.2f, 0.2f));
    prog.setUniform("Lights[1].La", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Lights[2].La", vec3(0.1f, 0.1f, 0.1f));

    GLuint brick = Texture::loadTexture("media/texture/brick1.jpg");
    GLuint moss = Texture::loadTexture("media/texture/moss.png");
    GLuint craig = Texture::loadTexture("media/texture/Craig.png");
    GLuint dirt = Texture::loadTexture("media/texture/Dirty_Craig.png");
   
    
    // Load brick texture file into channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brick);
    // Load moss texture file into channel 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, moss);
    // Load craig texture file into channel 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, craig);
    // Load dirt texture file into channel 3
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, dirt);
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("basic_uniform.gs");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	//update your angle here
    view = glm::lookAt(vec3(0.5f * sin(t) * radius, 5.75f, 0.75f * cos(t) * radius), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 50.0f);
    prog.setUniform("texUse", true);
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 1.5f, 0.0f));
    setMatrices();
    mesh->render();

    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 50.0f);
    prog.setUniform("texUse", true);
    model = mat4(1.0f);
    glm::vec3 scale = glm::vec3(10.0f, 10.0f, 10.0f);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, -0.5f, 0.55f));
    setMatrices();
    mesh->render();

    
    //prog.setUniform("Material.Kd", 0.3f, 0.3f, 0.3f);
    //prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    //prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    //prog.setUniform("Material.Shininess", 180.0f);
    //prog.setUniform("texUse", false);
    //model = mat4(1.0f);
    //model = glm::translate(model, vec3(2.0f, 0.0f, 0.0f));
    //setMatrices();
    //cube.render();

    //prog.setUniform("Material.Kd", 0.3f, 0.3f, 0.3f);
    //prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    //prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    //prog.setUniform("Material.Shininess", 180.0f);
    //prog.setUniform("texUse", false);
    //model = mat4(1.0f);
    //model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    //setMatrices();
    //plane.render();
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model; //we create a model view matrix
    
    prog.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix
    
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix
    
    prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
