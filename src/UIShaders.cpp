#include "ShaderUtils.h"
#include "UIShaders.h"

const char* vertexSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos; // pixel coordinates
uniform vec2 uResolution;
void main(){
    // convert from pixel coordinates (0..res) to NDC (-1..1), flip Y so 0 is top
    vec2 pos = vec2(
        (aPos.x / uResolution.x) * 2.0 - 1.0,
        1.0 - (aPos.y / uResolution.y) * 2.0
    );    
    gl_Position = vec4(pos, 0.0, 1.0);
    gl_PointSize = 1.0;
}
)";

const char* fragSrc = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;
void main(){
    FragColor = uColor;
}
)";

GLuint createUIProgram(){
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    return linkProgram(vs, fs);
}
