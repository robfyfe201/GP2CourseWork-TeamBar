#version 150

in vec3 vertexPosition;
out vec4 vertexColourOut;

uniform mat4 MVP;
uniform vec4 ambientMaterialColour;
uniform vec4 ambientLightColour;

void main()
{
	gl_Position = MVP*vec4(vertexPosition, 1.0);
	vertexColourOut = ambientMaterialColour*ambientLightColour;
}