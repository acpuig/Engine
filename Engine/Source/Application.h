#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleEditor;


class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleWindow* GetWindow() { return window; }  
    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleInput*  GetInput() { return input; }
    ModuleProgram* GetProgram() { return program; }
   ModuleEditor* GetEditor() { return editor; }


private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleProgram* program = nullptr;
    ModuleRenderExercise* triangle = nullptr;
    ModuleEditor* editor = nullptr;


    std::list<Module*> modules;

};

extern Application* App;
