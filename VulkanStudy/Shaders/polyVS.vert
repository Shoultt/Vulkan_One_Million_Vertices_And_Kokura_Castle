#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 1) out vec3 lightIntensity;
layout(location = 2) out vec2 fragTexCoord;

layout (set = 0, binding = 0) uniform UBO0
{
	mat4 model;
	mat4 view;
	mat4 proj;
	vec3 lightPos;
}ubo0;

void main()
{
	vec4 vPosition = vec4(inPos, 1.0f) * vec4(1.0f, -1.0f, 1.0f, 1.0f) - vec4(90.0f, 80.0f, 90.0f, 0.0f);
	vec4 worldPos = ubo0.model * (vec4(inPos, 1.0f) + vec4(90.0f, 80.0f, 90.0f, 0.0f));
	vec3 fragNormal = mat3(ubo0.model) * inNormal;
	vec3 fragViewVec = (ubo0.view * worldPos).xyz;
	vec3 fragLightVec = ubo0.lightPos - vec3(worldPos);

	vec3 N = normalize(fragNormal);
	vec3 L = normalize(fragLightVec);
	vec3 V = normalize(fragViewVec);
	vec3 R = reflect(L, N);

	vec3 ambient = vec3(0.70f, 0.0f, 0.91f) * 0.2f;
	vec3 diffuse = max(dot(N, L), 0.0f) * vec3(0.90f, 0.0f, 0.91f) * 1.5f;
	vec3 specular = pow(max(dot(R, V), 0.0f), 2.0f) * vec3(0.3f);

	float dist = length(ubo0.lightPos - vec3(worldPos));
	float attenuation = 1.0f / (1.0f + 0.00000000009f * dist + 0.00007f * (dist * dist));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	lightIntensity = vec3(ambient + diffuse + specular);
	fragTexCoord = inTexCoord;

	gl_Position = ubo0.proj * ubo0.view * ubo0.model * vPosition;
}