// GLEW нужно подключать до GLFW.
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
//#include <SOIL/SOIL.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObjectOOP.hpp"
#include "ShaderOOP.hpp"
#include "Camera.hpp"

#include <cstdlib>
#include <string>
#include <iostream>

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
    //Настройка GLFW
    //Задается минимальная требуемая версия OpenGL.
    //Мажорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //Минорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Выключение возможности изменения размера окна
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
        std::exit(-1);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

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
    unsigned int textureID;
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

int main(int argc, const char *argv[])
{
    GLFWwindow * window = myGLinit();

#ifdef MYDEBUG_BREAK
    __asm__ __volatile__("int3");
#endif //MYDEBUG_BREAK

    GameObject fobj("cubetex.vc", "lightning.vs", "lightning.fs");

    GameObject lightSource("cube.vc", "lamp.vs", "lamp.fs");

    int width, height;
    unsigned char* image;

    GLuint texture1;
    GLuint texture2;
    // ====================
    // Texture 1
    // ====================
    uint diffuseMap = loadTexture("container2.png");
    uint specularMap = loadTexture("container2_specular.png");
    //uint emissiveMap = loadTexture("matrix.jpg");
    /*
    // ===================
    // Texture 2
    // ===================
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("awesomeface2.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    */
    if (argc > 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    lightSource.position = glm::vec3(1.2f, 1.0f, 2.0f);
    lightSource.scale = glm::vec3(0.2f, 0.2f, 0.2f);
    lightSource.SetModelUniform(true);
    GLint lightViewLoc = lightSource.GetProg().GetLoc("view");
    GLint lightProjLoc = lightSource.GetProg().GetLoc("projection");

    GLint fobjViewLoc = fobj.GetProg().GetLoc("view");
    GLint fobjProjLoc = fobj.GetProg().GetLoc("projection");
    GLint viewPosLoc  = fobj.GetProg().GetLoc("viewPos");
    GLint lightPosLoc = fobj.GetProg().GetLoc("spotLight.position");
    GLint lightDirLoc = fobj.GetProg().GetLoc("spotLight.direction");

    glm::vec3 pointLightPositions[] =
    {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    fobj.GetProg().SetUVec3f("material.ambient",  1.0f, 0.5f, 0.31f);
    fobj.GetProg().SetUVec3f("material.diffuse",  1.0f, 0.5f, 0.31f);
    fobj.GetProg().SetUVec3f("material.specular", 0.5f, 0.5f, 0.5f);
    fobj.GetProg().SetUVec1f("material.shininess", 32.0f);

    fobj.GetProg().SetUVec3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    fobj.GetProg().SetUVec3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    fobj.GetProg().SetUVec3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    fobj.GetProg().SetUVec3f("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // point light 1
    fobj.GetProg().SetUVec3f("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    fobj.GetProg().SetUVec3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    fobj.GetProg().SetUVec3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    fobj.GetProg().SetUVec3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    fobj.GetProg().SetUVec1f("pointLights[0].constant", 1.0f);
    fobj.GetProg().SetUVec1f("pointLights[0].linear", 0.09);
    fobj.GetProg().SetUVec1f("pointLights[0].quadratic", 0.032);
    // point light 2
    fobj.GetProg().SetUVec3f("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    fobj.GetProg().SetUVec3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    fobj.GetProg().SetUVec3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    fobj.GetProg().SetUVec3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    fobj.GetProg().SetUVec1f("pointLights[1].constant", 1.0f);
    fobj.GetProg().SetUVec1f("pointLights[1].linear", 0.09);
    fobj.GetProg().SetUVec1f("pointLights[1].quadratic", 0.032);
    // point light 3
    fobj.GetProg().SetUVec3f("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    fobj.GetProg().SetUVec3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    fobj.GetProg().SetUVec3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    fobj.GetProg().SetUVec3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    fobj.GetProg().SetUVec1f("pointLights[2].constant", 1.0f);
    fobj.GetProg().SetUVec1f("pointLights[2].linear", 0.09);
    fobj.GetProg().SetUVec1f("pointLights[2].quadratic", 0.032);
    // point light 4
    fobj.GetProg().SetUVec3f("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    fobj.GetProg().SetUVec3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    fobj.GetProg().SetUVec3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    fobj.GetProg().SetUVec3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    fobj.GetProg().SetUVec1f("pointLights[3].constant", 1.0f);
    fobj.GetProg().SetUVec1f("pointLights[3].linear", 0.09);
    fobj.GetProg().SetUVec1f("pointLights[3].quadratic", 0.032);



    fobj.GetProg().SetUVec3f("spotLight.ambient",   0.1f, 0.1f, 0.1f);
    fobj.GetProg().SetUVec3f("spotLight.diffuse",   0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
    fobj.GetProg().SetUVec3f("spotLight.specular",  1.0f, 1.0f, 1.0f);
    fobj.GetProg().SetUVec1f("spotLight.cutOff",      glm::cos(glm::radians(12.5f)));
    fobj.GetProg().SetUVec1f("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    fobj.GetProg().SetUVec1f("spotLight.constant",    1.0f);
    fobj.GetProg().SetUVec1f("spotLight.linear",      0.09f);
    fobj.GetProg().SetUVec1f("spotLight.quadratic",   0.032f);

    glm::vec3 cubePositions[] =
    {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    GameObject lights[4];
    for (int i = 0; i < 4; ++i)
    {
        lights[i] = lightSource;
        lights[i].position = pointLightPositions[i];
        lights[i].Transform();
    }

    GameObject visual[10];
    for (int i = 0; i < 10; ++i)
    {
        visual[i] = fobj;
        visual[i].position = cubePositions[i];
        visual[i].rotation = glm::vec4(1.0f, 0.3f, 0.5f, glm::radians(20.0f * i));
        visual[i].Transform();
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    //glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D, emissiveMap);
    glActiveTexture(GL_TEXTURE0);

    fobj.GetProg().SetUVec1i("material.diffuse", 0);
    fobj.GetProg().SetUVec1i("material.specular", 1);
    //fobj.GetProg().SetUVec1i("emissiveMap", 2);

    while(!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        do_movement();

        /*
        lightSource.position = glm::vec3(glm::sin(glm::radians(glfwGetTime() * 60.0f)) * 5.0f, 1.0f,
                                         glm::cos(glm::radians(glfwGetTime() * 60.0f)) * 5.0f);
        lightSource.SetModelUniform(true);
        */



        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), screenWidth / screenHeight, 0.1f, 1000.0f);

        glProgramUniformMatrix4fv(fobj.GetProg(), fobjViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glProgramUniformMatrix4fv(fobj.GetProg(), fobjProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //glProgramUniform3f(fobj.GetProg(), lightPosLoc, lightSource.position.x, lightSource.position.y, lightSource.position.z);
        glProgramUniform3f(fobj.GetProg(), lightDirLoc, camera.Front.x,    camera.Front.y,    camera.Front.z);
        glProgramUniform3f(fobj.GetProg(), lightPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
        glProgramUniform3f(fobj.GetProg(), viewPosLoc,  camera.Position.x, camera.Position.y, camera.Position.z);

        glProgramUniformMatrix4fv(lightSource.GetProg(), lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glProgramUniformMatrix4fv(lightSource.GetProg(), lightProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for (int i = 0; i < 4; ++i)
        {
            lights[i].Draw();
        }

        //lightSource.Draw(false);

        for (int i = 0; i < 10; ++i)
        {
            visual[i].Draw();
        }

        glfwSwapBuffers(window);
    }

    fobj.Destroy();
    lightSource.Destroy();
    glfwTerminate();

    return 0;
}
