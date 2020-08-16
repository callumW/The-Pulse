#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec2 pulse_location;
uniform float pulse_radius;

layout(origin_upper_left) in vec4 gl_FragCoord;

void main()
{
    float dist = length(pulse_location - gl_FragCoord.xy);
    if (dist > pulse_radius) {
        discard;
    }
    color = texture(image, TexCoords);
}
