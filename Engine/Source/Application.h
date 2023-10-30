#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleRender;
class ModuleProgram;


class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return renderOPGL; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleRender* GetRender() { return render; }
    ModuleProgram* GetProgram() { return program; }
    //ModuleRenderExercise* GetRenderExercise() { return; }


private:

    ModuleOpenGL* renderOPGL = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleRender* render = nullptr;
    ModuleProgram* program = nullptr;
    ModuleRenderExercise* renderExercise = nullptr;


    std::list<Module*> modules;

};

extern Application* App;
