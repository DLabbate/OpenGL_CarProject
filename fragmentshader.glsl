#version 330 core
		in vec3 vertexColor;
		out vec4 FragColor;
		uniform vec3 mycolour;
		uniform vec3 lightposition;
		uniform vec3 lightcolour = vec3(1.0f);
		uniform float ambient_value = 0.2f;
		void main()
		{
		FragColor = vec4(mycolour, 1.0f);
		}