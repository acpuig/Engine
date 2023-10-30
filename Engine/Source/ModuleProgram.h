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
    char* LoadShaderSource(const char* shader_file_name);
    unsigned CompileShader(unsigned type, const char* source);
    unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);


};