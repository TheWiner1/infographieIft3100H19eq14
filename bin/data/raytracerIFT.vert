#version 120

attribute vec2 pos_xyA;
varying vec2 pos_xyV;

void main()
{
    gl_Position = vec4(pos_xyA, 0.0, 1.0);
    pos_xyV = pos_xyA;
}
