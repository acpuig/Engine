#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"
#include "ModuleProgram.h"
#include "ModuleEditor.h"

#include "imgui/imgui.h"

#include "Model.h"
#include "glew-2.1.0/include/GL/glew.h"
#include <psapi.h>

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
		
	//Init the GLEW library
 	GLenum err = glewInit();
	// … check for errors
	clearColor = float4(0.0f, 0.0f, 0.0f, 1.0f);


	//Initialize global states
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise*/f

	App->GetCamera()->Init();
	App->GetProgram()->Init();

	hardwareData.glew = (unsigned char*)glewGetString(GLEW_VERSION);
	hardwareData.glsl = (unsigned char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	hardwareData.opengl = (unsigned char*)glGetString(GL_VERSION);

	hardwareData.brand = (unsigned char*)glGetString(GL_VENDOR);
	hardwareData.gpu = (unsigned char*)glGetString(GL_RENDERER);

	//Detect current hardware and driver capabilities
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));

	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Init Variables


	return true;
}

HardwareDetection ModuleOpenGL::GetHardwareData() {
	return hardwareData;
}


update_status ModuleOpenGL::PreUpdate()
{
	model = new Model();
	if (bakerModel) {
		float3 scaledPosition = float3(40.0f, 40.0f, 40.0f) * modelScale;
		App->GetCamera()->SetModel(scaledPosition);
		model->Load("BakerHouse.gltf");
	}
	else if (cubeModel){
		float3 scaledPosition = float3(2.0f, 2.0f, 2.0f) * modelScale;
		App->GetCamera()->SetModel(scaledPosition);
		model->Load("BoxTextured.gltf");
	}

	//Screen color
//	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate frame time in milliseconds
	float frameTime = 1000.0f / ImGui::GetIO().Framerate;

	// Log FPS and frame time
	fps_log.push_back(ImGui::GetIO().Framerate);
	ms_log.push_back(frameTime);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{
	// Note: Debug draw disables blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model->Draw();
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

void ModuleOpenGL::Histogram()
{
	if (!fps_log.empty())
	{
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	}

	if (!ms_log.empty())
	{
		char title[25];
		sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 50.0f, ImVec2(310, 100));
	}
}

void ModuleOpenGL::WindowResized(int width, int height)
{
	SDL_SetWindowSize(App->GetWindow()->window, width, height);
	glViewport(0, 0, width, height);
}

/*void ModuleOpenGL::MenuConfigWindow() {
	ImGui::Text("Background Color");
	ImGui::ColorPicker3("", &clearColor[0]);
	ImGui::Separator();

}*/

void ModuleOpenGL::MenuConfigModels() {
	ImGui::Text("Model to Show:");
	if (ImGui::Checkbox("Cube Model", &cubeModel)) {
		bakerModel = false;
		model->CleanUp();
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Baker Model", &bakerModel)) {
		cubeModel = false;
		model->CleanUp();
	}
	ImGui::SliderFloat("Model Scale", &modelScale, 0.1f, 10.0f);
	if (ImGui::Button("Reset Model Scale"))
	{
		modelScale = 1.0f;
	}
	ImGui::Separator();

}
void ModuleOpenGL::MenuConfigApp() {


	// Display memory consumption (platform-specific, example for Windows)
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T memoryUsed = pmc.PrivateUsage / (1024 * 1024); // Memory usage in MB
	ImGui::Text("Memory Consumption: %lu MB", memoryUsed);
	ImGui::Separator();

	Histogram();


	/*
	if (ImGui::CollapsingHeader("Window Resizing"))
	{
		// Input fields for width and height
		static int newWidth = SCREEN_WIDTH;
		static int newHeight = SCREEN_HEIGHT;

		ImGui::InputInt("New Width", &newWidth);
		ImGui::InputInt("New Height", &newHeight);

		// Button to apply the new size
		if (ImGui::Button("Apply Size"))
		{
			// Call your window resizing function here
			WindowResized(newWidth, newHeight);
		}
	}
	*/
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer");
	delete model;

	//Destroy window
	SDL_GL_DeleteContext(App->GetWindow()->window);
	return true;
}
