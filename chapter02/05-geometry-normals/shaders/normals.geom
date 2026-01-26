#version 450

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    float normalLength;
    float time;
    float showNormals;
    float padding;
} ubo;

layout(location = 0) in vec3 inWorldPos[];
layout(location = 1) in vec3 inNormal[];

layout(location = 0) out vec3 outColor;

void main()
{
    if (ubo.showNormals < 0.5) {
        return;
    }

    mat4 vp = ubo.proj * ubo.view;

    // For each vertex of the triangle, emit a line showing the normal
    for (int i = 0; i < 3; i++) {
        vec3 pos = inWorldPos[i];
        vec3 normal = normalize(inNormal[i]);

        // Start point (at vertex)
        gl_Position = vp * vec4(pos, 1.0);
        outColor = vec3(0.0, 1.0, 1.0);  // Cyan at base
        EmitVertex();

        // End point (along normal)
        vec3 endPos = pos + normal * ubo.normalLength;
        gl_Position = vp * vec4(endPos, 1.0);
        outColor = vec3(1.0, 1.0, 0.0);  // Yellow at tip
        EmitVertex();

        EndPrimitive();
    }
}
