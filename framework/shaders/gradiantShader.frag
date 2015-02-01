#version 330

uniform float windowSize;

out vec4 outputColor;

void main()
{
    float l_lerpValue = gl_FragCoord.y / windowSize;

    outputColor = mix ( vec4 ( 1.0f, 1.0f, 1.0f, 1.0f ), vec4 ( 0.4f, 0.3f, 0.2f, 1.0f ), l_lerpValue );
}
