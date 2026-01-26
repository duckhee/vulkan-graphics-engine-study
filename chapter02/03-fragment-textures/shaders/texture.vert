#version 450

vec2 positions[6] = vec2[](
    vec2(-1.0, -1.0), vec2( 1.0, -1.0), vec2( 1.0,  1.0),
    vec2(-1.0, -1.0), vec2( 1.0,  1.0), vec2(-1.0,  1.0)
);

vec2 uvs[6] = vec2[](
    vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(1.0, 1.0),
    vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(0.0, 1.0)
);

layout(location = 0) out vec2 fragUV;

layout(push_constant) uniform PushConstants {
    float time;
    float textureType;    // 0: checkerboard, 1: circles, 2: stripes, 3: procedural
    float scale;
    float param1;
} pc;

void main()
{
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragUV = uvs[gl_VertexIndex];
}
