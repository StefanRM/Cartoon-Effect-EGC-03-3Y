#version 330

in vec2 texcoord;

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 light_position2;
uniform vec3 light_position3;
uniform vec3 light_position4;
uniform vec3 light_color;
uniform vec3 light_color2;
uniform vec3 light_color3;
uniform vec3 light_color4;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float numLevels;

uniform vec3 object_color; 

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int isSanta;
uniform int isLight;
uniform int isToon;
uniform int isOutlineFragment;

layout(location = 0) out vec4 out_color;

void main()
{
	// model vectors for each light
	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	vec3 R = reflect( L, world_normal );

	vec3 L2 = normalize( light_position2 - world_position );
	vec3 H2 = normalize( L2 + V );
	vec3 R2 = reflect( L2, world_normal );

	vec3 L3 = normalize( light_position3 - world_position );
	vec3 H3 = normalize( L3 + V );
	vec3 R3 = reflect( L3, world_normal );

	vec3 L4 = normalize( light_position3 - world_position );
	vec3 H4 = normalize( L3 + V );
	vec3 R4 = reflect( L3, world_normal );

	
	int recvLight, recvLight2, recvLight3, recvLight4;
	if (dot(L, world_normal) >= 0)
	{
		recvLight = 1;
	}
	else
	{
		recvLight = 0;
	}

	if (dot(L2, world_normal) >= 0)
	{
		recvLight2 = 1;
	}
	else
	{
		recvLight2 = 0;
	}

	if (dot(L3, world_normal) >= 0)
	{
		recvLight3 = 1;
	}
	else
	{
		recvLight3 = 0;
	}

	if (dot(L4, world_normal) >= 0)
	{
		recvLight4 = 1;
	}
	else
	{
		recvLight4 = 0;
	}

	vec3 kd;
	vec3 ks;

	if (isSanta == 1) // extract properties from santa textures
	{
		kd = texture2D(texture_1, texcoord).rgb;
		ks = texture2D(texture_2, texcoord).rgb;
	}
	else
	{
		kd = vec3(material_kd);
		ks = vec3(material_ks);
	}

	float ka = 0.5;
	float diffuseFact = ka + clamp(dot(L, world_normal), 0, 1);
	float specularFact = recvLight * pow(clamp(dot(H, world_normal), 0, 1), material_shininess);
	
	float aten_fac = 1 / pow(length(light_position - world_position), 2);

	if (isToon == 1)
	{
		diffuseFact = floor(diffuseFact * numLevels) / numLevels;
		specularFact = floor(specularFact * numLevels) / numLevels;
	}

	vec3 out_color_aux = ((diffuseFact + ka) * kd + specularFact * ks) * light_color * aten_fac;

	// light 2
	diffuseFact = ka + clamp(dot(L2, world_normal), 0, 1);
	specularFact = recvLight2 * pow(clamp(dot(H2, world_normal), 0, 1), material_shininess);
	
	aten_fac = 1 / pow(length(light_position2 - world_position), 2);

	if (isToon == 1)
	{
		diffuseFact = floor(diffuseFact * numLevels) / numLevels;
		specularFact = floor(specularFact * numLevels) / numLevels;
	}

	vec3 out_color_aux2 = ((diffuseFact + ka) * kd + specularFact * ks) * light_color2 * aten_fac;

	// light3
	diffuseFact = ka + clamp(dot(L3, world_normal), 0, 1);
	specularFact = recvLight3 * pow(clamp(dot(H3, world_normal), 0, 1), material_shininess);
	
	aten_fac = 1 / pow(length(light_position3 - world_position), 2);

	if (isToon == 1)
	{
		diffuseFact = floor(diffuseFact * numLevels) / numLevels;
		specularFact = floor(specularFact * numLevels) / numLevels;
	}

	vec3 out_color_aux3 = ((diffuseFact + ka) * kd + specularFact * ks) * light_color3 * aten_fac;

	// light4
	diffuseFact = ka + clamp(dot(L4, world_normal), 0, 1);
	specularFact = recvLight4 * pow(clamp(dot(H4, world_normal), 0, 1), material_shininess);
	
	aten_fac = 1 / pow(length(light_position4 - world_position), 2);

	if (isToon == 1)
	{
		diffuseFact = floor(diffuseFact * numLevels) / numLevels;
		specularFact = floor(specularFact * numLevels) / numLevels;
	}

	vec3 out_color_aux4 = ((diffuseFact + ka) * kd + specularFact * ks) * light_color4 * aten_fac;

	if (isLight == 1) // lights are not illuminating themselves
	{
		out_color = vec4(object_color, 1);
	}
	else
	{
		if (isOutlineFragment == 1) // outline has a constant value
		{
			out_color = vec4(object_color, 1);
		}
		else
		{
			// the rest of objects' color is a sum of all colors obtained from model applied to each light
			out_color = vec4(out_color_aux + out_color_aux2 + out_color_aux3 + out_color_aux4, 1);
		}
	}
}