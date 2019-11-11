#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;
		layout (location = 2) in vec3 normal;
		layout (std140) uniform Matrices
		{
			mat4 projectionMatrix;
			mat4 viewMatrix;
			mat4 worldOrientation;
			vec3 cameraPosition;
		};
		uniform mat4 worldMatrix;

		out vec3 vertexColor;
		out vec3 Normal;
		out vec3 FragPos;
		out vec3 camera_position;

		void main()
		{
		Normal = mat3(transpose(inverse(worldMatrix))) * normal;
		FragPos = vec3(worldMatrix * vec4(aPos, 1.0));
		camera_position = cameraPosition;

		vertexColor = aColor;
		mat4 modelViewProjection = projectionMatrix *viewMatrix * worldOrientation * worldMatrix;
		gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}