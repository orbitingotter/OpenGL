#include "pch.h"
#include "Model.h"

Model::Model(const std::string& filePath)
    : mVertexCount(0), mIndexCount(0)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR : ASSIMP : " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    mDirectory = filePath.substr(0, filePath.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

Model::~Model()
{
}

// traverse through all the nodes and process their meshes
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back(ProcessMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

// convert assimp mesh to our mesh class
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    // Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }


    if (mesh->mMaterialIndex >= 0)
    {
        // Material / Textures
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // diffuse
        std::vector<Texture2D> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "Diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // specular
        std::vector<Texture2D> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "Specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    mVertexCount += vertices.size();
    mIndexCount += indices.size() / 3;

    return Mesh(vertices, indices, textures);
}

std::vector<Texture2D> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture2D> textures;
    std::string completeFilePath;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        completeFilePath = mDirectory + '/' + str.C_Str();

        bool skip = false;
        for (unsigned int j = 0; j < mTextureCache.size(); j++)
        {
            if (mTextureCache[j].GetFilePath() == completeFilePath)
            {
                textures.push_back(mTextureCache[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture2D texture(completeFilePath, typeName);
            textures.push_back(texture);
            mTextureCache.push_back(texture);
        }
    }
    return textures;
}


