#version 330 core
		layout (location = 0) in vec3 aPos;

		uniform mat4 lightSpaceMatrix;
		uniform mat4 worldMatrix;
		out vec4 FragPos;
		
		void main()
		{
		gl_Position = lightSpaceMatrix * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
		FragPos = worldMatrix * vec4(aPos,1.0f);
		}