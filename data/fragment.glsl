uniform sampler2D inTexture;

varying vec2 outTexCoord;
varying vec4 outColour;

void main() {
	gl_FragColor = mix(outColour, texture2D(inTexture, outTexCoord.st), 1.0);
}
