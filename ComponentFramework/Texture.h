#ifndef TEXTURE_H
#define TEXTURE_H
#include <glew.h>


class Texture {
private:
	GLuint textureID;
public:
	Texture();
	~Texture();
	bool LoadImage(const char* filename);
	bool LoadMultipleImages(const char* filename, GLuint textureUnit);

	inline GLuint getTextureID() const { return textureID; }


};
#endif