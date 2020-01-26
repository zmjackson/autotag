#version 330 core

in float intensity;

out vec4 color;

void main() {
    color = vec4(0.2, 0.2 + intensity, 1.0 - intensity, 1.0);
}
