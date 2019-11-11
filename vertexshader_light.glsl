#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;
		layout (std140) uniform Matrices
		{
			mat4 projectionMatrix;
			mat4 viewMatrix;
			mat4 worldOrientation;
			vec3 cameraPosition;
		};
		uniform mat4 worldMatrix;
		out vec3 vertexColor;
		void main()
		{
		vertexColor = aColor;
		mat4 modelViewProjection = projectionMatrix *viewMatrix * worldOrientation * worldMatrix;
		gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}