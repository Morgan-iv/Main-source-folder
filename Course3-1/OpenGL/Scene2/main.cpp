// GLEW нужно подключать до GLFW.
// GLEW
#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
//#include <SOIL/SOIL.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/GameObjectOOP.hpp"
#include "include/ShaderOOP.hpp"
#include "include/Camera.hpp"

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

typedef unsigned int uint;

GLfloat screenWidth = 800;
GLfloat screenHeight = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX, lastY;

GLfloat deltaTime = 0.0f;   // Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;   // Время вывода последнего кадра

void do_movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true,
    // и приложение после этого закроется
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

GLFWwindow * myGLinit(void)
{
    //Инициализация GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow((int)screenWidth, (int)screenHeight, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(-1);
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        std::exit(-1);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double dLastX, dLastY;
    glfwGetCursorPos(window, &dLastX, &dLastY);
    lastX = (GLfloat) dLastX;
    lastY = (GLfloat) dLastY;

    return window;
}

uint loadTexture(const char * path)
{
    uint textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

uint loadCubemap(std::vector<std::string> faces)
{
    uint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (uint i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void moveup1(void)
{
    std::cout << "\e[1F";
}

int main(int argc, char const *argv[])
{
    GLFWwindow * window = myGLinit();

#ifdef MYDEBUG_BREAK
    __asm__ __volatile__("int3");
#endif //MYDEBUG_BREAK


    //std::cout << "Vert: " << GL_VERTEX_SHADER << std::endl;
    //std::cout << "Frag: " << GL_FRAGMENT_SHADER << std::endl;
    //std::cout << "Geom: " << GL_GEOMETRY_SHADER << std::endl;
    std::cout << std::endl;

    GameObject fobj("Vertices/cubetex.vc", "Shaders/lightning.vs", "Shaders/lightning.fs");
    GameObject lightSource("Vertices/cube.vc", "Shaders/lamp.vs", "Shaders/lamp.fs");
    Program Shades("Shaders/depth.vs", "Shaders/depth.fs", "Shaders/depth.gs");

    int width, height;
    unsigned char* image;

    uint diffuseMap = loadTexture("Textures/container2.png");
    uint specularMap = loadTexture("Textures/container2_specular.png");

    const uint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    uint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    uint depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (uint i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    lightSource.position = glm::vec3(2.0f, 1.0f, 2.0f);
    lightSource.scale = glm::vec3(0.2f, 0.2f, 0.2f);
    lightSource.SetModelUniform(true);
    GLint lightViewLoc = lightSource.GetProg().GetLoc("view");
    GLint lightProjLoc = lightSource.GetProg().GetLoc("projection");

    GameObject plane = fobj;
    plane.scale = glm::vec3(100, 100, 100);
    plane.position = glm::vec3(0, 0, -55);
    plane.Transform();

    GLint fobjViewLoc = fobj.GetProg().GetLoc("view");
    GLint fobjProjLoc = fobj.GetProg().GetLoc("projection");
    GLint viewPosLoc  = fobj.GetProg().GetLoc("viewPos");

    fobj.GetProg().SetUVec3f("material.ambient",  1.0f, 0.5f, 0.31f);
    fobj.GetProg().SetUVec3f("material.diffuse",  1.0f, 0.5f, 0.31f);
    fobj.GetProg().SetUVec3f("material.specular", 0.5f, 0.5f, 0.5f);
    fobj.GetProg().SetUVec1f("material.shininess", 128.0f);

    fobj.GetProg().SetUVec3f("lightSource.position", lightSource.position.x, lightSource.position.y, lightSource.position.z);
    fobj.GetProg().SetUVec3f("lightSource.ambient", 0.1f, 0.1f, 0.1f);
    fobj.GetProg().SetUVec3f("lightSource.diffuse", 1.0f, 1.0f, 1.0f);
    fobj.GetProg().SetUVec3f("lightSource.specular", 1.0f, 1.0f, 1.0f);
    fobj.GetProg().SetUVec1f("lightSource.constant", 1.0f);
    fobj.GetProg().SetUVec1f("lightSource.linear", 0.045);
    fobj.GetProg().SetUVec1f("lightSource.quadratic", 0.0075);

    glm::vec3 cubePositions[] =
    {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3(-1.3f,  0.5f, -1.5f)
    };

    GameObject visual[2];
    for (int i = 0; i < 2; ++i)
    {
        visual[i] = fobj;
        visual[i].position = cubePositions[i];
        visual[i].Transform();
    }

    float near_plane = 1.0;
    float far_plane = 25.0f;
    glm::mat4 ShadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
    glm::mat4 shadowTransforms[6];

    shadowTransforms[0] = (ShadowProj * glm::lookAt(lightSource.position, lightSource.position + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms[1] = (ShadowProj * glm::lookAt(lightSource.position, lightSource.position + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms[2] = (ShadowProj * glm::lookAt(lightSource.position, lightSource.position + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms[3] = (ShadowProj * glm::lookAt(lightSource.position, lightSource.position + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms[4] = (ShadowProj * glm::lookAt(lightSource.position, lightSource.position + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms[5] = (ShadowProj * glm::lookAt(lightSource.position, lightSource.position + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

    glProgramUniformMatrix4fv(Shades, Shades.GetLoc("shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
    glProgramUniformMatrix4fv(Shades, Shades.GetLoc("shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
    glProgramUniformMatrix4fv(Shades, Shades.GetLoc("shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
    glProgramUniformMatrix4fv(Shades, Shades.GetLoc("shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
    glProgramUniformMatrix4fv(Shades, Shades.GetLoc("shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
    glProgramUniformMatrix4fv(Shades, Shades.GetLoc("shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));

    Shades.SetUVec3f("lightPos", lightSource.position.x, lightSource.position.y, lightSource.position.z);
    Shades.SetUVec1f("far_plane", far_plane);

    fobj.GetProg().SetUVec1f("far_plane", far_plane);
    





    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    glActiveTexture(GL_TEXTURE0);

    fobj.GetProg().SetUVec1i("material.diffuse", 0);
    fobj.GetProg().SetUVec1i("material.specular", 1);
    fobj.GetProg().SetUVec1i("depthMap", 2);

    int FramesCount = 0;
    int lastSecond = -1;

    while(!glfwWindowShouldClose(window))
    {
        ++FramesCount;
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        visual[0].position.x = sin(currentFrame * 0.5) * 3.0;
        visual[1].position.x = 1.3 - sin(currentFrame * 0.5) * 3.0;
        visual[0].Transform();
        visual[1].Transform();

        int currSecond = (int)currentFrame;
        if (lastSecond < currSecond)
        {
            moveup1();
            std::cout << FramesCount << std::endl;
            FramesCount = 0;
            lastSecond = currSecond;
        }

        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        do_movement();

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        plane.SetProg(Shades);
        plane.Draw();
        for (int i = 0; i < 2; ++i)
        {
            visual[i].SetProg(Shades);
            visual[i].Draw();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glViewport(0, 0, (int)screenWidth, (int)screenHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        plane.SetProg(fobj.GetProg());
        for (int i = 0; i < 2; ++i)
        {
            visual[i].SetProg(fobj.GetProg());
        }
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), screenWidth / screenHeight, 0.1f, 1000.0f);

        glProgramUniformMatrix4fv(fobj.GetProg(), fobjViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glProgramUniformMatrix4fv(fobj.GetProg(), fobjProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glProgramUniform3f(fobj.GetProg(), viewPosLoc,  camera.Position.x, camera.Position.y, camera.Position.z);

        glProgramUniformMatrix4fv(lightSource.GetProg(), lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glProgramUniformMatrix4fv(lightSource.GetProg(), lightProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

        lightSource.Draw(false);

        plane.Draw();
        for (int i = 0; i < 2; ++i)
        {
            visual[i].Draw();
        }

        glfwSwapBuffers(window);
    }

    fobj.Destroy();
    lightSource.Destroy();
    Shades.Destroy();
    glfwTerminate();

    return 0;
}
