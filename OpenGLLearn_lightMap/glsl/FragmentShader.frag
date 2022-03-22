#version 330 core
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shiniess;
};
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{
	//ambient
	vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;
	//diffuse
	
	vec3 lightDir=normalize(light.position-FragPos);
	float diff=max(dot(Normal,lightDir),0.0);
	vec3 diffuse=light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;

	//specular
	vec3 reflectDir=reflect(-lightDir,Normal);
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 specular=pow(max(dot(viewDir, reflectDir), 0.0),material.shiniess)*texture(material.specular,TexCoords).rgb*light.specular;
	

	vec3 result=ambient+diffuse+specular;
	FragColor=vec4(result,1.0);
}