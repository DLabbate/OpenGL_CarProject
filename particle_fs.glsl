#version 330 core
		in vec3 vertexColor;
		in vec2 vertexUV;
		in vec3 Normal;
		in vec3 FragPos;
		in vec3 camera_position;
		in vec4 FragPosLightSpace;

		out vec4 FragColor;

		struct Material {
		  vec3 ambient;
		  vec3 diffuse;
		  vec3 specular;    
		  float shininess;
		}; 

		struct Light {
			vec3 position;
			vec3 ambient;
			vec3 diffuse;
			vec3 specular;

			vec3 terms;
		};

		uniform vec3 mycolour;

		uniform sampler2D texturesampler;
	
		uniform vec3 lightcolour = vec3(1.0f);
		uniform vec3 lightposition = vec3(0.0f,10.0f,0.0f);
		uniform bool displayHeadLights = false;


		uniform Material material;
		uniform Light light;
		uniform Light headlight1;
		uniform Light headlight2;

		uniform float far_plane = 25.0f;

		vec3 CalculatePointLight(Light,vec3,vec3,vec3);

		uniform float opacity;

		void main()
		{
		if (vec4(texture(texturesampler,vertexUV)).a < 0.1)
			discard;

		vec3 result = CalculatePointLight(light,Normal, FragPos, camera_position);
		vec3 result2 = CalculatePointLight(headlight1,Normal, FragPos, camera_position);
		vec3 result3 = CalculatePointLight(headlight2,Normal, FragPos, camera_position);

		if (displayHeadLights)
		{
		FragColor = vec4(result+result2+result3,opacity);
		}

		else
		{
		FragColor = vec4(result,opacity);
		}
		

		}


		vec3 CalculatePointLight(Light light,vec3 Normal, vec3 FragPos, vec3 camera_position)
		{
		float constant = light.terms.x;
		float linear = light.terms.y;
		float quadratic = light.terms.z;
		float distance = length(light.position - FragPos);
		float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));  

		vec3 normal_updated = normalize(Normal);
		vec3 lightdirection = normalize(light.position - FragPos);
		float diffuse_value = max(dot(normal_updated, lightdirection), 0.0);

		vec3 view_direction = normalize(camera_position - FragPos);
		vec3 reflection_direction = reflect(-lightdirection,normal_updated);
		float spec = pow(max(dot(view_direction, reflection_direction), 0.0),material.shininess);
		
		vec3 ambient = vec3(texture(texturesampler,vertexUV)) * light.ambient;
		vec3 diffuse = vec3(texture(texturesampler,vertexUV)) * (diffuse_value) * light.diffuse; 
		vec3 specular = (material.specular * spec) * light.specular;

		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;   

		vec3 result = (ambient) + (diffuse + specular);
		return result;
		}