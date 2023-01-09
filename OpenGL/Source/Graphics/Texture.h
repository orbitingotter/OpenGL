#pragma once

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
private:
	unsigned int mID;
	std::string mFilePath;
	unsigned char* mImageBuffer;
	int mWidth, mHeight, mChannels;
};