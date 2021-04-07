//-----------------------------------------------------------------------------
// basic.vert by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// Vertex shader
//-----------------------------------------------------------------------------
#version 330 core

layout (location = 0) in vec3 pos;  // in local coords
layout (location = 1) in vec2 texCoord;

out vec3 VertPos;
out vec2 TexCoord;

uniform float time;		    // interpolation
uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

void main()
{
    vec3 finalPostion = pos;
	gl_Position = projection * view * model * vec4(finalPostion, 1.0f);
	TexCoord = texCoord;
    VertPos = finalPostion;
}