#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"
#include "ModuleProgram.h"

#include "Model.h"

#include "SDL.h"
#include "glew-2.1.0/include/GL/glew.h"

ModuleOpenGL::ModuleOpenGL()
{

}

ModuleOpenGL::~ModuleOpenGL()
{
}

// Called before render is available
bool ModuleOpenGL::Init()
{
		//Create Context
	//Setup attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bit

	LOG("Creating Renderer context");
	context = SDL_GL_CreateContext(App->GetWindow()->window);

	//SDL_WINDOW_OPENGL();  Remember to create the SDL window with special flag

	//Init the GLEW library
 	GLenum err = glewInit();
	// � check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	// Should be 2.0

	//Detect current hardware and driver capabilities
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Initialize global states
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise*/f

	App->GetCamera()->Init();
	cubeModel = new Model();
	cubeModel->Load("BoxTextured.gltf");

	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{
	// Note: Debug draw disables blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	cubeModel->Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	int w, h;
	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);
	App->GetDebugDraw()->Draw(App->GetCamera()->GetViewMatrix(), App->GetCamera()->GetProjectionMatrix(), w, h);
	//change backbuffer y front buffer 
	SDL_GL_SwapWindow(App->GetWindow()->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer");
	//App->GetProgram()->CleanUp(); 
	//Destroy window
	SDL_GL_DeleteContext(App->GetWindow()->window);
	return true;
}

void ModuleOpenGL::WindowResized(unsigned width, unsigned height)
{
	int w, h;
	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);
	glViewport(0, 0, w, h);
}

