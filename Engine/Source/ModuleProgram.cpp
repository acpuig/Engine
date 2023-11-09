#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "SDL.h"

#include <GL/glew.h>

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}


unsigned ModuleProgram::Init(const char* defaultshader_file_name, const char* fragmentshader_file_name) {

    // Load and compile shaders
    const char* vertexShaderSource = App->GetProgram()->LoadShaderSource(defaultshader_file_name);
    const char* fragmentShaderSource = App->GetProgram()->LoadShaderSource(fragmentshader_file_name);

    unsigned vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Link shaders into a program
    unsigned program = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);

    // Delete individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}



char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{
    char* data = nullptr;
    FILE* file = nullptr;
    fopen_s(&file, shader_file_name, "rb"); // Open the file in binary mode

    if (file)
    {
        fseek(file, 0, SEEK_END); // Move the file pointer to the end to get file size
        int size = ftell(file); // Get the file size
        data = (char*)malloc(size + 1); // Allocate memory for the shader source (+1 for null-terminator)
        fseek(file, 0, SEEK_SET); // Reset the file pointer to the beginning
        fread(data, 1, size, file); // Read the file content into data
        data[size] = 0; // Add null-terminator to the end of data
        fclose(file); // Close the file
    }

    return data; // Return the loaded shader source
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
    unsigned shader_id = glCreateShader(type); // Create a shader object of specified type
    glShaderSource(shader_id, 1, &source, 0); // Load shader source into the shader object
    glCompileShader(shader_id); // Compile the shader

    int res = GL_FALSE; // Variable to store the compilation result
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res); // Get the compilation status

    if (res == GL_FALSE)
    {
        int len = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len); // Get the length of the info log

        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len); // Allocate memory for the info log
            glGetShaderInfoLog(shader_id, len, &written, info); // Get the info log

            LOG("Log Info: %s", info); // Print the info log
            free(info); // Free the allocated memory for the info log
        }
    }

    return shader_id; // Return the shader object ID
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
    unsigned program_id = glCreateProgram();
    glAttachShader(program_id, vtx_shader);
    glAttachShader(program_id, frg_shader);
    glLinkProgram(program_id);
    int res;
    glGetProgramiv(program_id, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetProgramInfoLog(program_id, len, &written, info);
            LOG("Program Log Info: %s", info);
            free(info);
        }
    }
    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    return program_id;
}