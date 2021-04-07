//-----------------------------------------------------------------------------
// basic.frag by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// Fragment shader for Part 1 of this tutorial
//-----------------------------------------------------------------------------
#version 330 core

in vec2 TexCoord;
in vec3 VertPos;
out vec4 frag_color;

uniform float time;
uniform sampler2D texSampler1;

void main()
{
	frag_color = texture(texSampler1, TexCoord);
    frag_color.r = mix(frag_color.r, 0.5, sin(time * 2 + VertPos.x) / 2 + 0.5);
    frag_color.g = mix(frag_color.g, 0.5, cos(time * 3 + VertPos.y) / 2 + 0.5);
    frag_color.b = mix(frag_color.b, 0.5, sin(time * 4 + VertPos.z) / 2 + 0.5);
}
