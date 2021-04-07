#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>

#include "GL/glew.h"
using std::string;

class Texture2D {
  public:
    Texture2D();
    virtual ~Texture2D();

    bool loadTexture(const string& fileName, bool generateMipMaps = true);
    void bind(GLuint texUnit = 0);
    void unbind(GLuint texUnit = 0);

  private:
    Texture2D(const Texture2D& rhs) {}

    GLuint mTexture;
};
#endif
