#version 330 core
out vec4 FragColor;

/*
    Get out frag coords in screen we're used too, i.e. origin at top left with x increasing to right
    and y increasing down screen.
*/
layout(origin_upper_left) in vec4 gl_FragCoord;

in float colorStrength;

uniform vec2 pulse_location;
uniform float pulse_radius;

void main()
{
    float dist = length(pulse_location - gl_FragCoord.xy);
    if (dist > pulse_radius - 0.5f && dist < pulse_radius + 0.5f) {
        FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
    else {
        FragColor = vec4(1.0f);
    }
}
