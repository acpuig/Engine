#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "SDL.h"

#include "glew-2.1.0/include/GL/glew.h"

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init() {

    // Load and compile shaders
    const char* vertexShaderSource = App->GetProgram()->LoadShaderSource("vertex.glsl");
    const char* fragmentShaderSource = App->GetProgram()->LoadShaderSource("fragment.glsl");

    unsigned vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Link shaders into a program
    program = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);

    glValidateProgram(program);

    // Delete individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void ModuleProgram::UseProgram() {
    glUseProgram(program);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        LOG("OpenGL Error before draw call: %d", error);
    }
}

unsigned int ModuleProgram::GetProgram() {
    return program;
}

void ModuleProgram::SendToShaderMatrix4fv(const char* name,const float* data) {
    if(program != 0){
        glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_TRUE, data);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            LOG("OpenGL Error after glUniformMatrix4fv: %d", error);
        }
    }
}
void ModuleProgram::SendToShaderUniform(const char* name, GLint iter) {
    if (program != 0) {
        glUniform1i(glGetUniformLocation(program, name), iter);
    }
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
    } else
    {
        LOG("Error loading shader source from file: %s", shader_file_name);
    }

    return data; // Return the loaded shader source
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
    unsigned shader_id = glCreateShader(type); // Create a shader object of specified type
    if (shader_id == 0)
    {
        LOG("Error creating shader of type %d", type);
        return 0;
    }

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

            LOG("Shader compilation error: %s", info);
            free(info); // Free the allocated memory for the info log

        } else
        {
            LOG("Shader compilation error: No additional information available");
        }
        LOG("Shader Source:\n%s", source);
    }

    return shader_id; // Return the shader object ID
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
    unsigned program_id = glCreateProgram();
    if (program_id == 0)
    {
        LOG("Error creating shader program");
        return 0;
    }
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

        } else
        {
            LOG("Program linking error: No additional information available");
        }
        LOG("Vertex Shader Source:\n%s", App->GetProgram()->LoadShaderSource("vertex.glsl"));
        LOG("Fragment Shader Source:\n%s", App->GetProgram()->LoadShaderSource("fragment.glsl"));

    }
    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    return program_id;
}

bool ModuleProgram::CleanUp() {
    glDeleteProgram(program);

    return true; 
}