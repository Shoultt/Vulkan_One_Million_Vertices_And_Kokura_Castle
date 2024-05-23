#version 450

layout(location = 0) in vec3 inPos;

layout(set = 0, binding = 0) uniform UBO{
	mat4 model;
	mat4 view;
	mat4 proj;
	vec3 lightPos;
}ubo0;

void main()
{
	vec4 vPosition = vec4(inPos.x * 3, inPos.y * 3, inPos.z * 3, 1.0f);
	gl_Position = vec4(ubo0.proj * ubo0.view * ubo0.model * (vPosition + vec4(ubo0.lightPos, 1.0f)));
}