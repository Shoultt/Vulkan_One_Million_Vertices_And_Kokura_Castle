#version 450

layout(location = 0) in vec3 lightIntensity;
layout(location = 1) in vec3 lightSpecular;
layout(location = 2) in vec2 fragTexCoord;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(lightIntensity, 0.0f) * texture(texSampler, fragTexCoord) + vec4(lightSpecular, 0.0f);
}