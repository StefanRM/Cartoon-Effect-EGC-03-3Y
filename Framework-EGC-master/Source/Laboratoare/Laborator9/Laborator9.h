#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

class Laborator9 : public SimpleScene
{
	public:
		Laborator9();
		~Laborator9();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, int isEarth = 0, int isLight = 0, int isOutline = 0);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		std::unordered_map<std::string, Texture2D*> mapTextures;
		GLuint randomTextureID;
		float clock;
		glm::vec3 lightPosition, lightPosition2, lightPosition3, lightPosition4;
		glm::vec3 colorLight, colorLight2, colorLight3, colorLight4;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		float angle = 0;
		int toon = 0, go = 0;
		float numLevels = 1.5f;
};
