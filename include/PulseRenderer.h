#ifndef PULSE_H
#define PULSE_H

#include "Shader.h"

typedef struct pulse_state {
    double charge_start = -1.0;
    double pulse_start = -1.0;
    double pulse_end = -1.0;
    float radius = -1.0f;
    glm::vec2 pulse_position = {0.0f, 0.0f};
} pulse_state_t;

class PulseRenderer {
public:
    PulseRenderer();
    ~PulseRenderer();

    void draw(pulse_state const& state);
private:
    unsigned int vao, vbo, ebo;

    Shader pulse_shader;
};
#endif
