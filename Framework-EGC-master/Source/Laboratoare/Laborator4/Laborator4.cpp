#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;

	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;
	angularStepOX2 = 0;
	angularStepOY2 = 0;
	angularStepOZ2 = 0;

	// initialize for jump
	height = 0;
	distance = 0;
	peak = -1;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -6);
	modelMatrix *= Transform3D::Translate(0, 0, 4.5f) * Transform3D::RotateOY(angularStepOY2) * Transform3D::Translate(0, 0, -4.5f);
	/*modelMatrix *= Transform3D::Translate(0, 0, 4.5f) * Transform3D::RotateOX(angularStepOX2) * Transform3D::Translate(0, 0, -4.5f);
	modelMatrix *= Transform3D::Translate(0, 0, 4.5f) * Transform3D::RotateOZ(angularStepOZ2) * Transform3D::Translate(0, 0, -4.5f);*/
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f + distance, 0.5f + height, 6);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 2, -1.5f);
	modelMatrix *= Transform3D::Translate(translateX, -1.5f + translateY, translateZ) * Transform3D::RotateOZ(angularStepOZ2) * Transform3D::Translate(-translateX, 1.5f - translateY, -translateZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		translateY += 10 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		translateY -= 10 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		translateX += 10 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		translateX -= 10 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_F)) {
		translateZ += 10 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_R)) {
		translateZ -= 10 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_1)) {
		scaleX += deltaTime;
		scaleY += deltaTime;
		scaleZ += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_2)) {
		scaleX -= deltaTime;
		scaleY -= deltaTime;
		scaleZ -= deltaTime;

		if (scaleX < 0.1) {
			scaleX = 0.1;
			scaleY = 0.1;
			scaleZ = 0.1;
		}
	}

	if (window->KeyHold(GLFW_KEY_3)) {
		angularStepOX += 5 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_4)) {
		angularStepOX -= 5 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_5)) {
		angularStepOY += 5 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_6)) {
		angularStepOY -= 5 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_7)) {
		angularStepOZ += 5 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_8)) {
		angularStepOZ -= 5 * deltaTime;
	}

	if (rotate) {
		//angularStepOX2 += 5 * deltaTime;
		angularStepOY2 += 5 * deltaTime;
		angularStepOZ2 += 5 * deltaTime;
	}

	if (jump) {
		distance += 4 * deltaTime;
		height = abs(sinf(distance) * 3);
	}
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}

	if (key == GLFW_KEY_I) {
		rotate = !rotate;
	}
	if (key == GLFW_KEY_J) {
		jump = !jump;
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
