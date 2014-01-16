uniform mat4 inViewMatrix;
uniform mat4 inModelMatrix;
uniform mat4 inProjectionMatrix;

attribute vec4 inVertex;
attribute vec2 inTexCoord;
attribute vec4 inColour;
varying vec2 outTexCoord;
varying vec4 outColour;

void main() {
    gl_Position = inVertex * inModelMatrix * inViewMatrix * inProjectionMatrix;
    outColour = inColour;
    outTexCoord = inTexCoord;
}
