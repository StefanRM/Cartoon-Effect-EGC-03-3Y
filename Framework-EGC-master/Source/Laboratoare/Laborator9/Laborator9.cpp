#include "Laborator9.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator9::Laborator9()
{
}

Laborator9::~Laborator9()
{
}

void Laborator9::Init()
{
	const string textureLoc = "Source/Laboratoare/Laborator9/Textures/";

	printf("\nTema 3 - EGC - Toon Fusion\nAuthor: Maftei Stefan - Radu\nGrupa: 336CC\n\nControls:\n-> T key to activate toon shading;\n-> P key to toggle lights movement;\n-> U / I key to increase / decrease numLevels.\n\n");

	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		mapTextures["crate"] = texture;
	}

	// Santa texture (diffuse)
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/Models/Santa/santa_diffuse.png", GL_REPEAT);
		mapTextures["santa_diffuse"] = texture;
	}

	// Santa texture (specular)
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/Models/Santa/santa_specular.png", GL_REPEAT);
		mapTextures["santa_specular"] = texture;
	}

	// Load meshes
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Santa mesh
	{
		Mesh* mesh = new Mesh("santa");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Santa/", "santa.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple quad
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab9");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Lights & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
		colorLight = glm::vec3(1, 0, 0);

		lightPosition2 = glm::vec3(1, 1, 0);
		colorLight2 = glm::vec3(0, 1, 0);

		lightPosition3 = glm::vec3(0, 1, -1);
		colorLight3 = glm::vec3(0, 0, 1);

		lightPosition4 = glm::vec3(-1, 1, 0);
		colorLight4 = glm::vec3(0, 1, 1);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
	}
}

void Laborator9::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator9::Update(float deltaTimeSeconds)
{
	clock = Engine::GetElapsedTime();

	// environment
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(12.5f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f), mapTextures["none"], mapTextures["none"], 0, 1);
	}

	// Render ground
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
		RenderSimpleMesh(meshes["plane"], shaders["ShaderLab9"], modelMatrix, glm::vec3(1, 0, 1), mapTextures["none"], mapTextures["none"], 0, 0);
	}

	// put santa in scene
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
		RenderSimpleMesh(meshes["santa"], shaders["ShaderLab9"], modelMatrix, glm::vec3(0, 0, 1), mapTextures["santa_diffuse"], mapTextures["santa_specular"], 1, 0);
	}

	if (toon == 1) // toon shading outline
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
		RenderSimpleMesh(meshes["santa"], shaders["ShaderLab9"], modelMatrix, glm::vec3(0, 0, 0), mapTextures["none"], mapTextures["none"], 0, 0, 1);
		glDisable(GL_CULL_FACE);
	}

	// Render the point lights in the scene with their movement
	// light 1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		lightPosition.x = 5 * sinf(angle) * sinf(angle) * sinf(angle) / 5.0;
		lightPosition.z = (4 * cosf(angle) - 1.3 * cosf(2 * angle) - 0.6 * cosf(3 * angle) - 0.2 * cos(4 * angle)) / 5.0;

		modelMatrix = glm::translate(modelMatrix, lightPosition);
		
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, colorLight, mapTextures["none"], mapTextures["none"], 0, 1);
	}

	// light 2
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		lightPosition2.x = cosf(3 * angle / 3.0);
		lightPosition2.y = abs(cosf(angle / 3.0)) + 0.5f;
		lightPosition2.z = cosf(4 * angle / 3.0);

		modelMatrix = glm::translate(modelMatrix, lightPosition2);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, colorLight2, mapTextures["none"], mapTextures["none"], 0, 1);
	}

	// light 3
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		lightPosition3.x = cosf(3 * angle);
		lightPosition3.y = abs(cosf(angle) + sinf(angle));
		lightPosition3.z = sinf(angle);

		modelMatrix = glm::translate(modelMatrix, lightPosition3);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, colorLight3, mapTextures["none"], mapTextures["none"], 0, 1);
	}

	// light 4
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		lightPosition4.x = sinf(3 * angle);
		lightPosition4.y = abs(cosf(angle) + sinf(angle)) + 0.5f;
		lightPosition4.z = -cosf(angle);

		modelMatrix = glm::translate(modelMatrix, lightPosition4);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, colorLight4, mapTextures["none"], mapTextures["none"], 0, 1);
	}
}

void Laborator9::FrameEnd()
{
}

void Laborator9::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color, Texture2D* texture1, Texture2D* texture2, int isSanta, int isLight, int isOutline)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// clock
	int location_clock = glGetUniformLocation(shader->program, "Clock");
	glUniform1f(location_clock, clock / 8);

	// santa
	int location_earth = glGetUniformLocation(shader->program, "isSanta");
	glUniform1i(location_earth, isSanta);

	// lights
	int location_light = glGetUniformLocation(shader->program, "isLight");
	glUniform1i(location_light, isLight);

	// toon shading
	int location_toon = glGetUniformLocation(shader->program, "isToon");
	glUniform1i(location_toon, toon);

	// numLevels
	int location_numLevels = glGetUniformLocation(shader->program, "numLevels");
	glUniform1f(location_numLevels, numLevels);

	// outline effect on
	int location_outline = glGetUniformLocation(shader->program, "isOutlineVertex");
	glUniform1i(location_outline, isOutline);
	location_outline = glGetUniformLocation(shader->program, "isOutlineFragment");
	glUniform1i(location_outline, isOutline);

	if (texture1)
	{
		// activate texture location 0
		// Bind the texture1 ID
		// Send texture uniform value
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	if (texture2)
	{
		// activate texture location 1
		// Bind the texture2 ID
		// Send texture uniform value
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}

	// lighting properties
	// Set shader uniforms for light & material properties
	// Set lights' positions uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	light_position = glGetUniformLocation(shader->program, "light_position2");
	glUniform3f(light_position, lightPosition2.x, lightPosition2.y, lightPosition2.z);
	light_position = glGetUniformLocation(shader->program, "light_position3");
	glUniform3f(light_position, lightPosition3.x, lightPosition3.y, lightPosition3.z);
	light_position = glGetUniformLocation(shader->program, "light_position4");
	glUniform3f(light_position, lightPosition4.x, lightPosition4.y, lightPosition4.z);

	int light_color = glGetUniformLocation(shader->program, "light_color");
	glUniform3f(light_color, colorLight.x, colorLight.y, colorLight.z);

	light_color = glGetUniformLocation(shader->program, "light_color2");
	glUniform3f(light_color, colorLight2.x, colorLight2.y, colorLight2.z);
	light_color = glGetUniformLocation(shader->program, "light_color3");
	glUniform3f(light_color, colorLight3.x, colorLight3.y, colorLight3.z);
	light_color = glGetUniformLocation(shader->program, "light_color4");
	glUniform3f(light_color, colorLight4.x, colorLight4.y, colorLight4.z);

	// Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator9::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;
	if (go)
	{
		angle += speed * deltaTime;
	}

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}
}

void Laborator9::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_T)
	{
		toon = (toon + 1) % 2;
	}

	if (key == GLFW_KEY_P)
	{
		go = (go + 1) % 2;
	}

	if (key == GLFW_KEY_I)
	{
		numLevels += 0.1f;
	}

	if (key == GLFW_KEY_U)
	{
		numLevels -= 0.1f;
		if (numLevels == 0)
		{
			numLevels = 1;
		}
	}
}

void Laborator9::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator9::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator9::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator9::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator9::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator9::OnWindowResize(int width, int height)
{
}
