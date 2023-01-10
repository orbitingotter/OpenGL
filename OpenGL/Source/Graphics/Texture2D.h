#pragma once

class Texture2D
{
public:
	Texture2D(const std::string& filePath, const std::string& type = "");
	~Texture2D();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline const std::string& GetType() const { return mType; }
private:
	unsigned int mID;
	std::string mFilePath;
	std::string mType;
	unsigned char* mImageBuffer;
	int mWidth, mHeight, mChannels;
};