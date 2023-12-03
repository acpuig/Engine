#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleProgram : public Module 
{
public:
    // Constructor and Destructor
    ModuleProgram();
    ~ModuleProgram();

    // Function to create a program from vertex and fragment shader files
    unsigned Init(const char* defaultshader_file_name, const char* fragmentshader_file_name);
    void UseProgram();

    void SendToShader(const char* name, const float* data);

private: 
    unsigned int program;
    unsigned int vertex_shader;
    unsigned int fragment_shader;

    char* LoadShaderSource(const char* shader_file_name);
    unsigned CompileShader(unsigned type, const char* source);
    unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
};