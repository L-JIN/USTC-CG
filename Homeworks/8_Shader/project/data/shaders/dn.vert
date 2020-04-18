#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 WorldPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform sampler2D displacementmap;
uniform float displacement_coefficient;

void main()
{
    // HW8 - 0_displacement_normal | calculate TBN
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = normalize(normalMatrix * aTangent);
	vec3 B = cross(N, T);
	vs_out.TBN = mat3(T, B, N);

    // HW8 - 0_displacement_normal | calculate displacement
    float displacement = -1 + 2 * texture(displacementmap, aTexCoord).r;
    displacement *= displacement_coefficient;
    vec4 worldPos = model * vec4(aPos, 1.0) + displacement * vec4(N, 0);
    vs_out.WorldPos = worldPos.xyz / worldPos.w;
    vs_out.TexCoord = aTexCoord;

    gl_Position = projection * view * worldPos;
}
