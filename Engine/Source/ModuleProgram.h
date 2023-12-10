#pragma once
#include "Module.h"
#include "Globals.h"
#include "glew-2.1.0/include/GL/glew.h"

class ModuleProgram : public Module 
{
public:
    ModuleProgram();
    ~ModuleProgram();

    bool Init();
    void UseProgram();

    unsigned int GetProgram();

    void SendToShaderMatrix4fv(const char* name, const float* data);
    void SendToShaderUniform(const char* name, GLint iter);

private: 
    unsigned int program;
    unsigned int vertex_shader;
    unsigned int fragment_shader;

    char* LoadShaderSource(const char* shader_file_name);
    unsigned CompileShader(unsigned type, const char* source);
    unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
    bool CleanUp() ;
};