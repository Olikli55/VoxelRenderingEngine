#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
//Texture cords
layout (location = 1) in vec2 aTex;


// Outputs the color for the Fragment Shader
out vec2 texCoord;

//camera matrix
uniform mat4 camMatrix;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(aPos, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
}