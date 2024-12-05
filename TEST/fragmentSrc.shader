#version 330 core
float near = 0.1; 
float far  = 100.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}
struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
	sampler2D lighter;
};
struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float linear;
	float constant;
	float quadratic;
	float cutOff;
	float outerCutOff;
};
#define NR_POINT_LIGHTS 4
//in vec3 ourColor;
in vec2	TexCoords;
//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform float per;
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform SpotLight spotLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 FragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 FragPos,vec3 viewDir);
void main()
{

	vec3 norm=normalize(Normal);
	vec3 viewDir=normalize(vec3(-FragPos));

	//direction light
	//vec3 result=CalcDirLight(dirLight,norm,viewDir);
	
	//position Light
	//for(int i=0;i<NR_POINT_LIGHTS;i++){
	//	result+=CalcPointLight(pointLights[i],norm,FragPos,viewDir);
	//}
	
	//spot Light
	//result+=CalcSpotLight(spotLight,norm,FragPos,viewDir);
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	FragColor=vec4(vec3(depth),1.0);


}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir=normalize(-light.direction);

	//ambient
	vec3 ambient=light.ambient*vec3(texture(material.texture_diffuse1,TexCoords));

	//diffuse
	float diff=max(dot(lightDir,normal),0.0);
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.texture_diffuse1,TexCoords));

	//specular
	vec3 ref=reflect(-lightDir,normal);
	float spec=pow(max(dot(ref,viewDir),0.0),material.shininess);
	vec3 specular=light.specular*spec*vec3(texture(material.texture_specular1,TexCoords));

	return (ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
	//attribute of lightray
	vec3 lightDir=normalize(light.position-fragPos);
	float distance=length(light.position-fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));
	//ambient
	vec3 ambient=light.ambient*vec3(texture(material.texture_diffuse1,TexCoords));

	//diffuse
	float diff=max(dot(lightDir,normal),0.0);
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.texture_diffuse1,TexCoords));

	//specular
	vec3 refle=reflect(-lightDir,normal);
	float spec=pow(max(dot(refle,viewDir),0.0),material.shininess);
	vec3 specular=light.specular*spec*vec3(texture(material.texture_specular1,TexCoords));
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;

	return (ambient+diffuse+specular);
}
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
	
	vec3 lightDir=normalize(-fragPos);
	float theta=dot(lightDir,normalize(-light.direction));
	float epsilon=light.cutOff-light.outerCutOff;
	float intensity=clamp((theta-light.outerCutOff)/epsilon,0.0,1.0);

	//attenuation
	float distance=length(-fragPos);
	float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));

	
	//ambient
	vec3 ambient= light.ambient*vec3(texture(material.texture_diffuse1,TexCoords));

	//diffuse
	float diff=max(dot(normal,lightDir),0.0);
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.texture_diffuse1,TexCoords));
	
	//specular
	vec3 refDirection=reflect(-lightDir,normal);
	float spec=pow(max(dot(refDirection,viewDir),0.0),material.shininess);
	vec3 specular=light.specular*spec*vec3(texture(material.texture_specular1,TexCoords));

	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;

	diffuse*=intensity;
	specular*=intensity;

	return(ambient+diffuse+specular);
	
}