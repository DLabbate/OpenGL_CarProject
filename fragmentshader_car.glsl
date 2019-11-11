#version 330 core

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

		in vec3 vertexColor;
		in vec3 Normal;
		in vec3 FragPos;
		in vec3 camera_position;

		out vec4 FragColor;
		uniform vec3 mycolour;
		uniform vec3 lightposition = vec3(0.0f,10.0f,0.0f);
		uniform vec3 lightcolour = vec3(1.0f);

		uniform Material material;
		uniform Light light;

		vec3 CalculatePointLight(Light,vec3,vec3,vec3);

		void main()
		{
		
		vec3 result = CalculatePointLight(light,Normal, FragPos, camera_position);
		FragColor = vec4(result, 1.0f);

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
		
		vec3 specular = (material.specular * spec) * lightcolour * light.specular;
		vec3 ambient = (material.ambient) * lightcolour * light.ambient;
		vec3 diffuse = (material.diffuse * diffuse_value) * lightcolour * light.diffuse; 

		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;   

		vec3 result = (ambient + diffuse + specular);
		return result;
		}