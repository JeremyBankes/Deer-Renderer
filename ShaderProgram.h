#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"
using std::string;

class ShaderProgram {
  public:
    ShaderProgram();
    ~ShaderProgram();

    enum ShaderType { VERTEX, FRAGMENT, PROGRAM };

    bool loadShaders(const char* vsFilename, const char* fsFilename);
    void use();

    GLuint getProgram() const;

    void setUniform(const GLchar* name, float v);
    void setUniform(const GLchar* name, const glm::vec2& v);
    void setUniform(const GLchar* name, const glm::vec3& v);
    void setUniform(const GLchar* name, const glm::vec4& v);
    void setUniform(const GLchar* name, const glm::mat4& m);

    GLint getUniformLocation(const GLchar* name);

  private:
    string fileToString(const string& filename);
    void checkCompileErrors(GLuint shader, ShaderType type);

    GLuint mHandle;
    std::map<string, GLint> mUniformLocations;
};
#endif