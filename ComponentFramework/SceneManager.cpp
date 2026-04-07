#include <SDL.h>
#include "SceneManager.h"
#include "Timer.h"
#include "Window.h"
#include "Scene0g.h"
#include "Scene0p.h"
#include "Scene1g.h"
#include "Scene1p.h" // first physics assignment
#include "Scene2p.h"
#include "Scene2g.h"
#include "Scene3g.h"
#include "Scene3p.h"
#include "Scene0pr.h"
#include "Scene4g.h"
#include "Scene4p.h"
// Imgui stuff
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

SceneManager::SceneManager(): 
	currentScene{nullptr}, window{nullptr}, timer{nullptr},
	fps(60), isRunning{false}, fullScreen{false} {
	Debug::Info("Starting the SceneManager", __FILE__, __LINE__);
}

SceneManager::~SceneManager() {
	Debug::Info("Deleting the SceneManager", __FILE__, __LINE__);

	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}
	// Delete ImGui stuff
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (window) {
		delete window;
		window = nullptr;
	}
	
}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize Window object", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}

	/********************************   Default first scene   ***********************/
	BuildNewScene(SCENE_NUMBER::SCENE4p);
	/********************************************************************************/
	
	//// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	////ImGui::StyleColorsLight();

	//// Setup scaling
	//ImGuiStyle& style = ImGui::GetStyle();
	//float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	//style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	//style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

	//// Setup Platform/Renderer backends
	//SDL_GLContext gl_context = SDL_GL_CreateContext(window->getWindow());

	//ImGui_ImplSDL3_InitForOpenGL(window->getWindow(), gl_context);
	//ImGui_ImplOpenGL3_Init("#version 450");

	return true;
}

/// This is the whole game
void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		HandleEvents();
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();

		/*ImGui::Render();
		glViewport(0, 0, 400, 400);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

		
		SDL_GL_SwapWindow(window->getWindow());
		SDL_Delay(timer->GetSleepTime(fps));
	}
}

void SceneManager::HandleEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) { /// Loop over all events in the SDL queue
		if (sdlEvent.type == SDL_EventType::SDL_EVENT_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_EVENT_KEY_DOWN) {
			switch (sdlEvent.key.scancode) {
			[[fallthrough]]; /// C17 Prevents switch/case fallthrough warnings
			case SDL_SCANCODE_ESCAPE:
			case SDL_SCANCODE_Q:
				isRunning = false;
				return;
				

			case SDL_SCANCODE_F1:
			case SDL_SCANCODE_F2:
			case SDL_SCANCODE_F3:
			case SDL_SCANCODE_F4:
			case SDL_SCANCODE_F5:
		
				BuildNewScene(SCENE_NUMBER::SCENE0g);
				break;

			default:
				break;
			}
		}
		if (currentScene == nullptr) { /// Just to be careful
			Debug::FatalError("No currentScene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		//ImGui_ImplSDL3_ProcessEvent(&sdlEvent);

		currentScene->HandleEvents(sdlEvent);
	}
}

bool SceneManager::BuildNewScene(SCENE_NUMBER scene) {
	bool status; 

	if (currentScene != nullptr) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scene) {
	case SCENE_NUMBER::SCENE0g:
		currentScene = new Scene0g();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE0p:
		currentScene = new Scene0p();
		status = currentScene->OnCreate();
		break;

	/*case SCENE_NUMBER::SCENE1g:
		currentScene = new Scene1g();
		status = currentScene->OnCreate();
		break;*/

	case SCENE_NUMBER::SCENE1p:
		currentScene = new Scene1p();
		status = currentScene->OnCreate();
		break;

	case SCENE_NUMBER::SCENE2p:
		currentScene = new Scene2p();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE2g:
		currentScene = new Scene2g();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE3g:
		currentScene = new Scene3g();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE3p:
		currentScene = new Scene3p();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE0pr:
		currentScene = new Scene0pr();
		status = currentScene->OnCreate();
		break;

	case SCENE_NUMBER::SCENE4g:
		currentScene = new Scene4g();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE4p:
		currentScene = new Scene4p();
		status = currentScene->OnCreate();
		break;
	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		return false;
	}
	return true;
}


