#ifndef  GAMEOBJECTOOP_HPP
#define  GAMEOBJECTOOP_HPP

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderOOP.hpp"

typedef unsigned int uint;

class GameObject
{
private:
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    GLuint vcount;
    mutable GLint modelLoc;
    Program program;
    mutable glm::mat4 model;

public:
    glm::vec3 scale;
    glm::vec4 rotation;
    glm::vec3 position;

public:
    void DebugLog() const
    {
        std::cout << "VAO    = " << VAO << std::endl;
        std::cout << "VBO    = " << VBO << std::endl;
        std::cout << "EBO    = " << EBO << std::endl;
        std::cout << "vcount = " << vcount << std::endl;
        std::cout << "mLoc   = " << modelLoc << std::endl;
        std::cout << "progID = " << program.ID << std::endl;
        std::cout << "pstate = " << program.state << std::endl;
        std::cout << "scale  = " << glm::to_string(scale) << std::endl;
        std::cout << "rot    = " << glm::to_string(rotation) << std::endl;
        std::cout << "pos    = " << glm::to_string(position) << std::endl;
        std::cout << glm::to_string(model) << std::endl;
    }

    void Destroy()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        VBO = VAO = EBO = 0;
        program.Destroy();
    }

    GameObject() : VBO(0), VAO(0), EBO(0), vcount(0), modelLoc(0),
    scale(1.0f, 1.0f, 1.0f), rotation(0.0f, 0.0f, 1.0f, 0.0f),
    position(0.0f, 0.0f, 0.0f) { }

    GameObject (const char * vcpath) : GameObject()
    {
        std::ifstream file;
        GLuint attrcount, vertcount, indcount;
        GLuint * attrs = nullptr;
        GLuint * inds = nullptr;
        GLfloat * verts = nullptr;
        // Удостоверимся, что ifstream объекты могут выкидывать исключения
        file.exceptions(std::ifstream::failbit);
        try 
        {
            file.open(vcpath);
            file >> attrcount >> vertcount >> indcount;
            attrs = new GLuint[attrcount * 4];
            verts = new GLfloat[vertcount];
            inds = new GLuint[indcount];
            for (int i = 0; i < attrcount * 4; ++i)
            {
                file >> attrs[i];
            }
            for (int i = 0; i < vertcount; ++i)
            {
                file >> verts[i];
            }
            for (int i = 0; i < indcount; ++i)
            {
                file >> inds[i];
            }
            file.close();
        }
        catch(std::ifstream::failure e)
        {
            delete[] attrs;
            delete[] verts;
            delete[] inds;
            std::cout << "ERROR::VERTICES::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            return;
        }
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertcount * sizeof(GLfloat), verts, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indcount * sizeof(GLuint), inds, GL_STATIC_DRAW);
        for (int i = 0; i < attrcount; ++i)
        {
            glVertexAttribPointer(attrs[i * 4 + 0],
                                  attrs[i * 4 + 1],
                                  GL_FLOAT,
                                  GL_FALSE,
                                  attrs[i * 4 + 2] * sizeof(GLfloat),
                                  (GLvoid*)(attrs[i * 4 + 3] * sizeof(GLfloat))
                                 );
            glEnableVertexAttribArray(attrs[i * 4 + 0]);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        delete[] attrs;
        delete[] verts;
        delete[] inds;
        vcount = indcount;
    }

    GameObject(const char * vcpath, const char * vspath,
               const char * fspath) : GameObject(vcpath)
    {
        this->program = Program(vspath, fspath);

        if (this->program.state != 0)
        {
            this->Destroy();
            return;
        }
        UpdateMLoc();
    }

    glm::mat4 GetModel() const
    {
        return model;
    }
    void SetModel(glm::mat4 model)
    {
        this->model = model;
    }
    Program GetProg() const
    {
        return program;
    }
    void SetProg(Program prog)
    {
        program = prog;
        this->UpdateMLoc();
    }
    GLuint GetVCount() const
    {
        return vcount;
    }
    GLint GetMLoc() const
    {
        return modelLoc;
    }
    void UpdateMLoc() const
    {
        modelLoc = program.GetLoc("model");
    }
    void Transform() const
    {
        model = glm::mat4();
        model = glm::translate(model, this->position);
        model = glm::rotate(model, this->rotation.w, glm::vec3(this->rotation));
        model = glm::scale(model, this->scale);
    }
    void SetModelUniform(bool needTransform = false) const
    {
        if (needTransform)
            Transform();
        glProgramUniformMatrix4fv(program.ID, modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    }
    void BindVAO() const
    {
        glBindVertexArray(VAO);
    }
    void UnBindVAO() const
    {
        glBindVertexArray(0);
    }
    void Draw(bool setUniform = true, bool needTransform = false) const
    {
        program.Use();
        if (setUniform)
            SetModelUniform(needTransform);
        BindVAO();
        glDrawElements(GL_TRIANGLES, vcount, GL_UNSIGNED_INT, 0);
        UnBindVAO();
    }
    operator GLuint() const
    {
        return this->VAO;
    }
    
    
};


#endif //GAMEOBJECTOOP_HPP
