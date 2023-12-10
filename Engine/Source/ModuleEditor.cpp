#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "Model.h"
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
	info.CPUcacheSize = SDL_GetCPUCacheLineSize();
	info.CPUcount = SDL_GetCPUCount();
	info.SystemRAM = SDL_GetSystemRAM(); 
	info.openglVersion.glew = App->GetOpenGL()->GetHardwareData().glew;
	info.openglVersion.glsl = App->GetOpenGL()->GetHardwareData().glsl;
	info.openglVersion.opengl = App->GetOpenGL()->GetHardwareData().opengl;
	info.openglVersion.brand = App->GetOpenGL()->GetHardwareData().brand;
	info.openglVersion.gpu = App->GetOpenGL()->GetHardwareData().gpu;

	return true;
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

	ConfigWindow();
	MainMenu();

}

void ModuleEditor::ConfigWindow() {

	ImGui::Begin("Configuration");

	//a. A graph for the frames per second
	if (ImGui::CollapsingHeader("Application")) {
		App->GetOpenGL()->MenuConfigApp();

	}
	//b. Configuration for all variables on each module (OpenGL, window, input and textures)
	if (ImGui::CollapsingHeader("Window")) {
		App->GetWindow()->MenuConfigWindow();
	}
	//There must be information output with FPS graph, memory consumption, hardware detection and software versions(SDL, OpenGL, Devil)
	if (ImGui::CollapsingHeader("Information")){
		ImGui::Text("SDL Version: %d.%d.%d ", info.sdlVersion.major, info.sdlVersion.minor, info.sdlVersion.patch);
		static const ImVec4 pink = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
		ImGui::TextColored(pink, "------Software------");
		ImGui::Text("OpenGL Version: %s ",info.openglVersion.opengl);
		ImGui::Text("Glew Version: %s ", info.openglVersion.glew);
		ImGui::Text("GLSL Version: %s ", info.openglVersion.glsl);

		ImGui::TextColored(pink, "------Hardware------");
		ImGui::Text("GPU: %s ", info.openglVersion.gpu);
		ImGui::Text("Brand: %s ", info.openglVersion.brand);
	}
	if (ImGui::CollapsingHeader("Models")) {
		App->GetOpenGL()->MenuConfigModels();
		model = new Model();
		model->MenuConfigTexture();
	}
	ImGui::End();

}

void ModuleEditor::MainMenu() {
	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("GitHub")) {

			if (ImGui::MenuItem("Profile")) {
				RequestBrowser("https://github.com/acpuig");
			}
			if (ImGui::MenuItem("Documentation")) {
				RequestBrowser("https://github.com/acpuig/Engine/blob/3db98b0a43c36fb4f87b214bd7021145e20d5341/README.md");
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

