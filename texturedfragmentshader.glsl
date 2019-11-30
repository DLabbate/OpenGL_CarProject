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
		uniform sampler2D shadowMap;
		uniform bool displayShadows = true;
		uniform bool displayHeadLights = false;


		uniform vec3 lightcolour = vec3(1.0f);
		uniform vec3 lightposition = vec3(0.0f,10.0f,0.0f);


		uniform Material material;
		uniform Light light;
		uniform Light headlight1;
		uniform Light headlight2;
		uniform Light taillight1;
		uniform Light taillight2;
		
		uniform vec3 headlight_positions[12];
		uniform vec3 taillight_positions[12];

		uniform float far_plane = 25.0f;

		vec3 CalculatePointLight(Light,vec3,vec3,vec3);
		vec3 CalculateHeadLight(vec3,vec3,vec3,vec3);
		vec3 CalculateTailLight(vec3,vec3,vec3,vec3);
		float ShadowCalculation(vec4);

		void main()
		{
		if (vec4(texture(texturesampler,vertexUV)).a < 0.1)
			discard;

		vec3 result = CalculatePointLight(light,Normal, FragPos, camera_position);
		vec3 result2 = CalculatePointLight(headlight1,Normal, FragPos, camera_position);
		vec3 result3 = CalculatePointLight(headlight2,Normal, FragPos, camera_position);
		vec3 result4 = CalculatePointLight(taillight1,Normal, FragPos, camera_position);
		vec3 result5 = CalculatePointLight(taillight2,Normal, FragPos, camera_position);

		vec3 result6 = vec3(0.0);
		vec3 result7 = vec3(0.0);

		for (int i=0; i < 12; i++)
		{
			result6 += CalculateHeadLight(headlight_positions[i],Normal, FragPos, camera_position);
		}

		for (int i=0; i < 12; i++)
		{
			result7 += CalculateTailLight(taillight_positions[i],Normal, FragPos, camera_position);
		}

		if (displayHeadLights)
		{
		FragColor = vec4(result+result2+result3+result4+result5+result6+result7,1.0f);
		}

		else
		{
		FragColor = vec4(result,1.0f);
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

		vec3 result = (ambient + ( 1.0f - ShadowCalculation(FragPosLightSpace) ) * (diffuse + specular));
		return result;
		}



		float ShadowCalculation(vec4 fragposition_lightspace)
		{
		
		vec3 projection_coordinates = fragposition_lightspace.xyz/fragposition_lightspace.w;
		projection_coordinates = projection_coordinates * 0.5 + 0.5;

		float shadowmap_depth = texture(shadowMap,projection_coordinates.xy).r;

		if (!displayShadows)
		return 0.0f;


		if (projection_coordinates.z > 1)
		{
		return 0.0f;
		}

		if (!displayShadows)
		return 0.0f;

		else
		{
			float shadow = 0.0;
			vec2 texelsize = 1.0/textureSize(shadowMap,0);

			for (int x= -1; x< 1; x++)
			{
				for (int y= -1; y< 1; y++)
				{
					float pcfdepth = texture(shadowMap, projection_coordinates.xy + vec2(x,y) * texelsize).r;
					shadow += projection_coordinates.z - 0.0005 > pcfdepth ? 1.0: 0.0;
				}

			}

			if (projection_coordinates.z - 0.0005 > shadowmap_depth)
			{
			return shadow/9;
			}
			
			else
			{
			return 0.0f;
			}

		}

		}


		vec3 CalculateHeadLight(vec3 lightpos,vec3 Normal, vec3 FragPos, vec3 camera_position)
		{
		float constant = 1.0f;
		float linear = 0.14f;
		float quadratic = 0.07f;
		float distance = length(lightpos - FragPos);
		float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));  

		vec3 normal_updated = normalize(Normal);
		vec3 lightdirection = normalize(lightpos - FragPos);
		float diffuse_value = max(dot(normal_updated, lightdirection), 0.0);

		vec3 view_direction = normalize(camera_position - FragPos);
		vec3 reflection_direction = reflect(-lightdirection,normal_updated);
		float spec = pow(max(dot(view_direction, reflection_direction), 0.0),material.shininess);
		
		vec3 ambient = vec3(texture(texturesampler,vertexUV)) * vec3(0.5f,0.5f,0.5f);
		vec3 diffuse = vec3(texture(texturesampler,vertexUV)) * (diffuse_value) * vec3(1.0f,1.0f,1.0f); 
		vec3 specular = (material.specular * spec) * vec3(1.0f,1.0f,1.0f);

		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;   

		vec3 result = (ambient + ( 1.0f - ShadowCalculation(FragPosLightSpace) ) * (diffuse + specular));
		return result;
		}


		vec3 CalculateTailLight(vec3 lightpos,vec3 Normal, vec3 FragPos, vec3 camera_position)
		{
		float constant = 1.0f;
		float linear = 0.14f;
		float quadratic = 0.07f;
		float distance = length(lightpos - FragPos);
		float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));  

		vec3 normal_updated = normalize(Normal);
		vec3 lightdirection = normalize(lightpos - FragPos);
		float diffuse_value = max(dot(normal_updated, lightdirection), 0.0);

		vec3 view_direction = normalize(camera_position - FragPos);
		vec3 reflection_direction = reflect(-lightdirection,normal_updated);
		float spec = pow(max(dot(view_direction, reflection_direction), 0.0),material.shininess);
		
		vec3 ambient = vec3(texture(texturesampler,vertexUV)) * vec3(0.5f,0.0f,0.0f);
		vec3 diffuse = vec3(texture(texturesampler,vertexUV)) * (diffuse_value) * vec3(1.0f,0.0f,0.0f); 
		vec3 specular = (material.specular * spec) * vec3(1.0f,1.0f,1.0f);

		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;   

		vec3 result = (ambient + ( 1.0f - ShadowCalculation(FragPosLightSpace) ) * (diffuse + specular));
		return result;
		}