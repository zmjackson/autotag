layout (location = 0) in vec3 position;
layout (location = 1) in float vertIntensity;
//attribute vec4 vertex;
//attribute float intensity;
//attribute vec3 normal;
//varying vec3 vert;
//varying vec3 vertNormal;

out float intensity;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

void main() {
   //vert = vertex.xyz;
   //vertNormal = normalMatrix * normal;
   gl_Position = projMatrix * mvMatrix * vec4(position, 1.0);
   intensity = vec4(position, 1.0);
}
