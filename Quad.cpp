#include "pch.h"
#include "Quad.h"


Quad::Quad()
{
	generateVAO();
	float velocity_x = ( static_cast<float> (rand()) / static_cast<float>(RAND_MAX) );
	velocity_x = velocity_x*0.5f - 0.25f; 
	//std::cout << (double)velocity_x;
	velocity = glm::normalize(glm::vec3(velocity_x, 1.0f, 0.0f)) / 1000.0f;

}

Quad::Quad(glm::vec3 pos) : position(pos)
{
	generateVAO();
	float velocity_x = (static_cast<float> (rand()) / static_cast<float>(RAND_MAX));
	float random_angle = (static_cast<float> (rand()) / static_cast<float>(RAND_MAX));
	random_angle *= 360;
	velocity_x = velocity_x * 0.5f - 0.25f;
	//std::cout << (double)velocity_x;
	velocity = glm::normalize(glm::vec3(velocity_x, 1.0f, 0.0f)) / 1000.0f;

	velocity = glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians(random_angle), glm::vec3(0.0f, 1.0f, 0.0f))* glm::vec4(velocity,0.0));
	angle_z_random = rand() % 360;
}

Quad::Quad(glm::vec3 pos,glm::vec3 vel) : position(pos), velocity(vel)
{
	generateVAO();
}


Quad::~Quad()
{
}


void Quad::generateVAO()
{
	struct TexturedVertex
	{
		TexturedVertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _uv)
			: position(_position), normal(_normal), uv(_uv) {}

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};


	TexturedVertex vertices[]
	{
		TexturedVertex(glm::vec3(-1.0f/2, 1.0f/2, 0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec2(0.0f,1.0f)),
		TexturedVertex(glm::vec3(1.0f/2, -1.0f/2, 0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec2(1.0f,0.0f)),
		TexturedVertex(glm::vec3(-1.0f / 2, -1.0f / 2, 0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec2(0.0f,0.0f)),

		TexturedVertex(glm::vec3(-1.0f / 2, 1.0f / 2, 0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec2(0.0f,1.0f)),
		TexturedVertex(glm::vec3(1.0f / 2, 1.0f / 2, 0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec2(1.0f,1.0f)),
		TexturedVertex(glm::vec3(1.0f / 2, -1.0f / 2, 0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec2(1.0f,0.0f)),
	};


	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0); //Positions
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)(2*sizeof(glm::vec3))); //uv
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)(sizeof(glm::vec3))); //Normals
	glEnableVertexAttribArray(3);

	quadVAO = VAO;
}

void Quad::Draw(int shaderprogram)
{
	glUseProgram(shaderprogram);
	glBindVertexArray(quadVAO);

	GLuint worldMatrixLocation = glGetUniformLocation(shaderprogram, "worldMatrix");
	glm::mat4 quad_transform = glm::mat4(1.0f);
	
	//position += velocity;
	quad_transform = glm::translate(quad_transform, position);
	quad_transform = glm::rotate(quad_transform, glm::radians(rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	quad_transform = glm::rotate(quad_transform, glm::radians(angle_z_random), glm::vec3(0.0f, 0.0f, 1.0f));
	quad_transform = glm::scale(quad_transform, glm::vec3(size, size, size));

	

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &quad_transform[0][0]);

	GLuint opacity_location = glGetUniformLocation(shaderprogram, "opacity");
	glUniform1f(opacity_location, opacity);

	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Quad::Update(float dt)
{
	position += velocity * speed * dt;
	size = (size < max_size) ? size + growth_speed*dt : max_size;
	age += dt;

	if (age > lifetime)
	{
		isExpired = true;
	}

	float interpolation_parameter = (age / lifetime); //linear
	opacity = opacity_initial * (1 - interpolation_parameter) + opacity_final * interpolation_parameter;
}

bool Quad::checkExpired()
{
	return isExpired;
}

void Quad::updateMaterial(int shaderprogram)
{
	glUseProgram(shaderprogram);

	GLuint material_ambient_location = glGetUniformLocation(shaderprogram, "material.ambient");
	glUniform3f(material_ambient_location, 0.25f, 0.25f, 0.25f);

	GLuint material_diffuse_location = glGetUniformLocation(shaderprogram, "material.diffuse");
	glUniform3f(material_diffuse_location, 0.8f, 0.8f, 0.8f);

	GLuint material_specular_location = glGetUniformLocation(shaderprogram, "material.specular");
	glUniform3f(material_specular_location, 0.01f, 0.01f, 0.01f);

	GLuint material_shininess_location = glGetUniformLocation(shaderprogram, "material.shininess");
	glUniform1f(material_shininess_location, 0.01f * 128);
}

void Quad::setAngle(float angle)
{
	rotation_angle = angle;
}

void Quad::updateRotation(float angle)
{
	rotation_angle = angle;
}

