#version 330 core
		in vec3 vertexColor;
		out vec4 FragColor;
		uniform vec3 mycolour;
		uniform vec3 lightcolour;
		void main()
		{
		FragColor = vec4(lightcolour,1.0f);
		}