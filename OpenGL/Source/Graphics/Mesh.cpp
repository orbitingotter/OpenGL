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

const std::string diffuse = "uTextureDiffuse0";
const std::string spec = "uTextureSpecular0";


void Mesh::BindTextures(Shader& shader)
{
	unsigned int diffuseN = 0;
	unsigned int specularN = 0;
	unsigned int normalN = 0;


	for (unsigned int i = 0; i < mTextures.size(); i++)
	{
		std::string finalUniformName = "uTexture";
		std::string name = mTextures[i].GetType();
		int number = 0;
		if (name == "Diffuse")
		{
			//number = diffuseN++;

			//finalUniformName += name + std::to_string(number);
			mTextures[i].Bind(0);
			//shader.SetUniform(finalUniformName, 0);

		}
		else if (name == "Specular")
		{
			//number = specularN++;

			//finalUniformName += name + std::to_string(number);
			mTextures[i].Bind(1);
			//shader.SetUniform(finalUniformName, (int)1);

		}
		else if (name == "Normal")
		{
			//number = normalN++;

			//finalUniformName += name + std::to_string(number);
			mTextures[i].Bind(2);
			//shader.SetUniform(finalUniformName, (int)2);

		}
		else if (name == "Displacement")
		{
			mTextures[i].Bind(3);
		}


		//finalUniformName += name + std::to_string(number);
		//
		//mTextures[i].Bind(i);
		//shader.SetUniform(finalUniformName, (int)i);
	}

	/*
	// same order as loaded
	if (mTextures.size() >= 1)
	{
		mTextures[0].Bind(0);
		//shader.SetUniform(diffuse, 0);
	}
	if (mTextures.size() >= 2)
	{
		mTextures[1].Bind(1);
		//shader.SetUniform(spec, 1);
	}
	if (mTextures.size() >= 3)
	{
		mTextures[2].Bind(2);
		//shader.SetUniform("uTextureNormal0", 2);
	}
	*/
}

void Mesh::UnbindTextures()
{
	for (unsigned int i = 0; i < mTextures.size(); i++)
	{
		mTextures[i].Unbind();
	}
}

void Mesh::SetupMesh()
{
	BufferLayout layout;
	layout.Push<float>(3);	// position
	layout.Push<float>(3);	// normal
	layout.Push<float>(2);	// texture coordinates
	layout.Push<float>(3);	// tangent
	layout.Push<float>(3);	// bitangent


	mVAO.AddBuffer(mVBO, mIBO, layout);

}
