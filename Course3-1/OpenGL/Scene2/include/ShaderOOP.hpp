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
            std::cout << "TYPE: " << shader_type << std::endl;
            this->state = 1;
            GLchar infoLog[512];
            glGetShaderInfoLog(this->ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
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
    Program(const char * vspath, const char * fspath, const char* gspath = nullptr)
    {
        this->ID = 0;
        this->state = 0;
        Shader vs = Shader(vspath, GL_VERTEX_SHADER);
        if (vs.state != 0)
        {
            std::cout << "ERROR::SHADER::PROGRAM::VSHADER_IS_INVALID" << std::endl;
            this->state = 3;
            return;
        }
        Shader fs = Shader(fspath, GL_FRAGMENT_SHADER);
        if (fs.state != 0)
        {
            std::cout << "ERROR::SHADER::PROGRAM::FSHADER_IS_INVALID" << std::endl;
            this->state = 4;
            vs.Destroy();
            return;
        }
        Shader gs = Shader(gspath, GL_GEOMETRY_SHADER);
        if (gspath != nullptr && gs.state != 0)
        {
            std::cout << "ERROR::SHADER::PROGRAM::GSHADER_IS_INVALID" << std::endl;
            this->state = 5;
            vs.Destroy();
            fs.Destroy();
            return;
        }
        this->Create();
        this->Attach(vs);
        this->Attach(fs);
        if (gspath != nullptr)
            this->Attach(gs);
        this->Link();
        this->Detach(vs);
        this->Detach(fs);
        if (gspath != nullptr)
            this->Detach(gs);
        vs.Destroy();
        fs.Destroy();
        if (gspath != nullptr)
            gs.Destroy();
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
    void Attach(GLuint shader) const
    {
        if (glIsShader(shader))
            glAttachShader(this->ID, shader);
    }
    void Detach(GLuint shader) const
    {
        if (glIsShader(shader))
            glDetachShader(this->ID, shader);
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
    void SetUVec1f(const char * varname, GLfloat v0) const
    {
        glProgramUniform1f(this->ID, this->GetLoc(varname), v0);
    }
    void SetUVec2f(const char * varname, GLfloat v0, GLfloat v1) const
    {
        glProgramUniform2f(this->ID, this->GetLoc(varname), v0, v1);
    }
    void SetUVec3f(const char * varname, GLfloat v0, GLfloat v1, GLfloat v2) const
    {
        glProgramUniform3f(this->ID, this->GetLoc(varname), v0, v1, v2);
    }
    void SetUVec4f(const char * varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
    {
        glProgramUniform4f(this->ID, this->GetLoc(varname), v0, v1, v2, v3);
    }
    void SetUVec1i(const char * varname, GLint v0) const
    {
        glProgramUniform1i(this->ID, this->GetLoc(varname), v0);
    }
    void SetUVec2i(const char * varname, GLint v0, GLint v1) const
    {
        glProgramUniform2i(this->ID, this->GetLoc(varname), v0, v1);
    }
    void SetUVec3i(const char * varname, GLint v0, GLint v1, GLint v2) const
    {
        glProgramUniform3i(this->ID, this->GetLoc(varname), v0, v1, v2);
    }
    void SetUVec4i(const char * varname, GLint v0, GLint v1, GLint v2, GLint v3) const
    {
        glProgramUniform4i(this->ID, this->GetLoc(varname), v0, v1, v2, v3);
    }
    void SetUMat4f(const char * varname, const GLfloat * value, bool transpose = false) const
    {
        glProgramUniformMatrix4fv(this->ID, this->GetLoc(varname), 1, transpose, value);
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
