#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture2D>& textures)
	: mVertices(vertices), mIndices(indices), mTextures(textures),
	mVAO(),
	mVBO(vertices.data(), vertices.size() * sizeof(Vertex)),
	mIBO(indices.data(), mIndices.size())
{
	SetupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::BindTextures(Shader& shader)
{
	shader.Bind();
	unsigned int diffuseN = 0;
	unsigned int specularN = 0;

	for (unsigned int i = 0; i < mTextures.size(); i++)
	{
		std::string name = mTextures[i].GetType();
		int number = 0;
		if (name == "Diffuse")
			number = diffuseN++;
		else if (name == "Specular")
			number = specularN++;

		mTextures[i].Bind(i);
		shader.SetUniform("uTexture" + name + std::to_string(number), i);
	}
}


void Mesh::SetupMesh()
{

	BufferLayout layout;
	layout.Push<float>(3);	// position
	layout.Push<float>(3);	// normal
	layout.Push<float>(2);	// texture coordinates

	mVAO.AddBuffer(mVBO, mIBO, layout);

}
