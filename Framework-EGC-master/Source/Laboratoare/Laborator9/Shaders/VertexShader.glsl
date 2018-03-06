#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Clock;
uniform int isOutlineVertex;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texcoord;

void main()
{
	// pass v_texture_coord as output to Fragment Shader
	vec2 texture_coord = v_texture_coord;
	texcoord = texture_coord;

	// pass world properties to Fragment Shader
	world_position = (Model * vec4(v_position,1)).xyz;
	world_normal = normalize( mat3(Model) * v_normal);

	if (isOutlineVertex == 0) // if not outline shape (for toon shading)
	{
		gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	}
	else
	{
		gl_Position = Projection * View * Model * vec4(v_position + v_normal * sin(Clock * 5), 1.0);
	}
}
