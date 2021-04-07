@ECHO OFF
g++                 ^
main.cpp            ^
Camera.cpp          ^
Mesh.cpp            ^
ShaderProgram.cpp   ^
Texture2D.cpp       ^
-o bin/main.exe     ^
-I includes         ^
-L lib              ^
-l glew32           ^
-l glfw3            ^
-l gdi32            ^
-l opengl32         ^
-D GLEW_STATIC      ^
-g