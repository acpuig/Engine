#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "SDL.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>


#include <GL/glew.h>

ModuleEditor::ModuleEditor()
{

}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

// Called before render is available
bool ModuleEditor::Init()
{


	LOG("Creating ImGui context");
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	context = SDL_GL_CreateContext(App->GetWindow()->window);
	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, context);
	ImGui_ImplOpenGL3_Init("#version 330");
	return true;
}

update_status ModuleEditor::PreUpdate()
{
	 ImGui_ImplOpenGL3_NewFrame();
	 ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
	 ImGui::NewFrame();


	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
	//Render frame before swapping buffers
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{

	return true;
}
