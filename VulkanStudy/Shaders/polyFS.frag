#version 450

layout(location = 1) in vec3 lightIntensity;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
layout(binding = 2) uniform sampler2D texSampler;

void main()
{
	outColor = texture(texSampler, fragTexCoord) * vec4(lightIntensity, 1.0f);
}