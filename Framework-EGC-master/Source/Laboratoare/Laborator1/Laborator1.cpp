#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		Mesh* mesh2 = new Mesh("sphere");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh2->GetMeshID()] = mesh2;
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(r, g, b, a);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	if (switcher == 0) {
		// render the object
		RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));
	}
	else {
		RenderMesh(meshes["sphere"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));
	}

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(xAxis, yAxis, 0));

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (window->KeyHold(GLFW_KEY_W)) {
		yAxis += 1 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		yAxis -= 1 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		xAxis += 1 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		xAxis -= 1 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_N)) {
		switcher = (switcher + 1) % 2;
	}
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_R) {
		r = (r + 1) % 2;
	}

	if (key == GLFW_KEY_G) {
		g = (g + 1) % 2;
	}

	if (key == GLFW_KEY_B) {
		b = (b + 1) % 2;
	}

	if (key == GLFW_KEY_Q) {
		r = 0;
		g = 0;
		b = 0;
	}

	if (key == GLFW_KEY_M) {
		switcher = (switcher + 1) % 2;
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
