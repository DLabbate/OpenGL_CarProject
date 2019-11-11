#version 330 core

in vec4 FragPos;
struct Light {
			vec3 position;
			vec3 ambient;
			vec3 diffuse;
			vec3 specular;

			vec3 terms;
		};

uniform Light light;
uniform float near_plane = 1.0f;
uniform float far_plane = 25.0f;

void main()
{
	
}