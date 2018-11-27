#ifndef  SHADEROOP_HPP
#define  SHADEROOP_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

typedef unsigned int uint;

class Shader
{
public:
    GLenum type;
    GLuint ID;
    mutable uint state;
    Shader(const char * path = nullptr, GLenum shader_type = GL_VERTEX_SHADER)
    {

        this->ID = 0;
        this->type = shader_type;
        if (path == nullptr)
        {
            this->state = 2;
            return;
        }
        this->state = 0;
        std::string code;
        std::ifstream file;
        // Удостоверимся, что ifstream объекты могут выкидывать исключения
        file.exceptions(std::ifstream::failbit);
        try 
        {
            file.open(path);
            std::stringstream cstream;
            cstream << file.rdbuf();
            file.close();
            code = cstream.str();   
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            this->state = 2;
            return;
        }
        const GLchar * shaderCode = code.c_str();

        GLint success;
        
        this->ID = glCreateShader(shader_type);
        glShaderSource(this->ID, 1, &shaderCode, NULL);
        glCompileShader(this->ID);
        // Если есть ошибки - вывести их
        glGetShaderiv(this->ID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            this->state = 1;
            GLchar infoLog[512];
            glGetShaderInfoLog(this->ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };
        if (this->state != 0)
            this->Destroy();
    }

    operator GLuint() const
    {
        return this->ID;
    }

    void Destroy()
    {
        glDeleteShader(this->ID);
        this->ID = 0;
    }
};

class Program
{
public:
    GLuint ID;
    mutable uint state;
    Program()
    {
        this->ID = 0;
        this->state = 0;
    }
    Program(const char * vspath, const char * fspath)
    {
        //std::cout << "hi" << std::endl;
        this->ID = 0;
        this->state = 0;
        Shader vs = Shader(vspath, GL_VERTEX_SHADER);
        if (vs.state != 0)
        {
            std::cout << "ERROR::SHADER::PROGRAM::VSHADER_IS_INVALID" << std::endl;
            this->state = 3;
            return;
        }
        //std::cout << "its vs " << vs << std::endl;
        Shader fs = Shader(fspath, GL_FRAGMENT_SHADER);
        if (fs.state != 0)
        {
            std::cout << "ERROR::SHADER::PROGRAM::FSHADER_IS_INVALID" << std::endl;
            this->state = 4;
            vs.Destroy();
            return;
        }
        //std::cout << "its fs " << fs << std::endl;
        this->Create();
        //std::cout << "created " << this->ID << ' ' << this->state << std::endl;
        this->Add(vs);
        this->Add(fs);
        //std::cout << "addvsfs " << this->ID << ' ' << this->state << std::endl;
        this->Link();
        vs.Destroy();
        fs.Destroy();
        //std::cout << "finally " << this->ID << ' ' << this->state << std::endl;
    }
    void Create()
    {
        this->ID = glCreateProgram();
    }
    void Destroy()
    {
        glDeleteProgram(this->ID);
        this->ID = 0;
    }
    void Add(GLuint shader) const
    {
        if (glIsShader(shader))
            glAttachShader(this->ID, shader);
    }
    void Use() const
    {
        if (this->state == 0)
            glUseProgram(this->ID);
    }
    GLint GetLoc(const char * varname) const
    {
        if (this->state == 0)
            return glGetUniformLocation(this->ID, varname);
    }
    void Link() const
    {
        glLinkProgram(this->ID);
        GLint success;
        glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            this->state = 1;
            GLchar infoLog[512];
            glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
    operator GLuint() const
    {
        return this->ID;
    }
};

#endif //SHADEROOP_HPP
