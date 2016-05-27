#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

layout (location = 2) uniform mat4 transformMatrix;

layout (location = 3) uniform mat4 cameraMat;

layout (location = 4) in vec3 normal;

layout (location = 5) uniform vec3 lightLocation;
layout (location = 6) uniform vec3 camLocation;

out vec2 fragUV;
out vec3 fragLoc;
out vec3 fragNorm;
out vec3 lightLoc;
out vec3 camLoc;

void main()
{
	fragUV = uv;

	lightLoc = lightLocation;
	camLoc = camLocation;

	//fragLoc = transformMatrix * fragLoc;
	fragNorm = vec3(transpose(inverse(transformMatrix)) * vec4(normal, 0));

	gl_Position = cameraMat * transformMatrix * vec4(position, 1);
}