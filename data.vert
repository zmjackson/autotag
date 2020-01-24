#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in float vertIntensity;

out float intensity;

uniform mat4 projection;
uniform mat4 view;
uniform mat3 model;

void main() {
   gl_Position = projection * view * model * vec4(position, 1.0);
   intensity = vertIntensity;
}
