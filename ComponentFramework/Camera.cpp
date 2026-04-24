#include "Camera.h"


Camera::Camera():useSkybox(true), skybox(nullptr) {
	projection = MMath::perspective(45.0f, 16.0f / 9.0f, 0.5f, 100.0f);
	orientation = QMath::angleAxisRotation(0.0f, Vec3(0.0f, 1.0f, 0.0f));
	translation = Vec3(0.0f, -1.0f, -5.0f);

	// for underwater scene
	/*if (useSkybox == true) {
		skybox = new Skybox("textures/Underwater Box_Posx.png", "textures/Underwater Box_Negx.png",
			"textures/Underwater Box_Posy.png", "textures/Underwater Box_Negy.png",
			"textures/Underwater Box_Posz.png", "textures/Underwater Box_Negz.png");
		skybox->OnCreate();
		skybox->LoadImages();
	}*/

	// for city scene
	if (useSkybox == true) {
		skybox = new Skybox("textures/posx.jpg", "textures/negx.jpg",
			"textures/posy.jpg", "textures/negy.jpg",
			"textures/posz.jpg", "textures/negz.jpg");
		skybox->OnCreate();
		skybox->LoadImages();
	}
}

void Camera::HandleEvents(const SDL_Event& sdlEvent) {
	trackball.HandleEvents(sdlEvent);
	orientation = trackball.getQuat();

}
Camera::~Camera() {
	if (skybox) delete skybox;
}

void Camera::Render() const {
	if (useSkybox) {
		glDisable(GL_DEPTH_TEST);
		glUseProgram(skybox->getSkyboxShader()->GetProgram());
		glUniformMatrix4fv(skybox->getSkyboxShader()->GetUniformID("projection"), 1, GL_FALSE, projection);
		glUniformMatrix4fv(skybox->getSkyboxShader()->GetUniformID("model"), 1, GL_FALSE, MMath::toMatrix4(orientation));
		skybox->Render();
		glUseProgram(0);
	}
}

