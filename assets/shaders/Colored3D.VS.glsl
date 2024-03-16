#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_norm;
layout(location = 2) in vec3 v_coords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 color;
uniform vec3 tankPosition;
uniform int hp;
uniform bool isTank;
uniform int random;

out vec3 frag_color;

void main()
{
    if (!isTank) {
        frag_color = color;
    }
    else if (isTank && hp > 0) {
        frag_color = color - (50.0f * vec3((3 - hp))) / 255.0f;
    } else {
        float bw = (color.x + color.y + color.z) / 3.0f;
        frag_color = glm::vec3(bw, bw, bw);
    }

    if (isTank) {
        vec3 world_position = vec3(Model * vec4(v_position, 1.0));

        vec3 dir = normalize(world_position - tankPosition);

        gl_Position = Projection * View * vec4(world_position - 0.07f * dir * (3 - hp), 1.0);
    }
    else {
        gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    }

}
