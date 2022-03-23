#version 330 core

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight
{

	vec3 position;
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLight[NR_POINT_LIGHTS];

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight;


struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

uniform vec3 viewPos;
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;


/*
	平行光计算
*/
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir=normalize(-light.direction);
	float diff=max(dot(normal,lightDir),0.0);
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

	vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoord));
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoord));
	vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoord));

	return ambient+diffuse+specular;
};
/*
	点光源计算
*/
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
	vec3 lightDir=normalize(light.position-fragPos);
	
	//漫反射
	float diff=max(dot(normal,lightDir),0.0);
	//镜面光
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

	//衰减
	float distances=length(light.position-fragPos);
	float attenuation=1.0/(light.constant+light.linear*distances+light.quadratic*(distances*distances));

	vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoord));
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoord));
	vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoord));

	//衰减计算
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	return ambient+diffuse+specular;
};

/*
	聚光源计算
*/
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
	 vec3 lightDir=normalize(light.position-fragPos);
	
	//漫反射
	float diff=max(dot(normal,lightDir),0.0);
	//镜面光
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

	//内外角计算
	float theta=dot(lightDir,normalize(-light.direction));
	float epsilon=(light.cutOff-light.outerCutOff);
	float intensity=clamp((theta-light.outerCutOff)/epsilon,0.0,1.0);

	vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoord));
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoord));
	vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoord));

	//根据内外角进行衰减计算
	ambient*=intensity;
	diffuse*=intensity;
	specular*=intensity;
	return ambient+diffuse+specular;
}



void main()
{
	vec3 norm=normalize(Normal);
	vec3 viewDir=normalize(viewPos-FragPos);
	
	vec3 result=CalcDirLight(dirLight,norm,viewDir);

	
	for(int i=0;i<NR_POINT_LIGHTS;i++)
	{
		result+=CalcPointLight(pointLight[i],norm,FragPos,viewDir);
	}
	result+=CalcSpotLight(spotLight,norm,FragPos,viewDir);
	FragColor=vec4(result,1.0);
}


