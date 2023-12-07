#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"

#include "SDL.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"


//#include "glew-2.1.0/include/GL/glew.h"

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
	//Create Context
	LOG("Creating ImGui context");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->GetContext());
	ImGui_ImplOpenGL3_Init("#version 330");

	//Set Configuración 
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	ImGui::StyleColorsDark();

	//Give info SDL/OpenGL/Devil 
	SDL_GetVersion(&info.sdlVersion);
	//about.devil_version = App->textures->GetDevilVersion();
	info.CPUcacheSize = SDL_GetCPUCacheLineSize();
	info.CPUcount = SDL_GetCPUCount();
	info.SystemRAM = SDL_GetSystemRAM(); 
	info.openglVersion.glew = App->GetOpenGL()->GetHardwareData().glew;
	info.openglVersion.glsl = App->GetOpenGL()->GetHardwareData().glsl;
	info.openglVersion.opengl = App->GetOpenGL()->GetHardwareData().opengl;
	info.openglVersion.brand = App->GetOpenGL()->GetHardwareData().brand;
	info.openglVersion.gpu = App->GetOpenGL()->GetHardwareData().gpu;


	//about.gpu = App->renderer->GetGpuData();
	//about.gl = App->renderer->GetGlVersion();

	return true;
}



void ModuleEditor::Histogram()
{
	char title[25];
	//sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
	//ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	//sprintf_s(title, 25, "Milliseconds %0.1f%", ms_log[ms_log.size() - 1]);
	//ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
}

update_status ModuleEditor::PreUpdate()
{
	//New Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
	ImGui::NewFrame();

	Draw();
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
	ImGui::ShowDemoWindow();

	//Render frame before swapping buffers
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();

	if (quit) {
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	//Destroy Context
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleEditor::Draw() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("About")) {
			//		RequestBrowser();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GitHub")) {

			if (ImGui::MenuItem("Documentation")) {
			//	RequestBrowser();
			}
			if (ImGui::MenuItem("Report a Bug")) {
			//	RequestBrowser();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Quit")) {
			if (ImGui::MenuItem("Yes")) {
				quit = true;
			}
			if (ImGui::MenuItem("No")) {

			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ModuleEditor::RequestBrowser(const char* link) {
	ShellExecute(0, 0, link, 0, 0, SW_SHOW);
}