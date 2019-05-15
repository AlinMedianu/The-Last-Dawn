// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "Importer.hpp"
#include "scene.h"
#include "../../assimp-3.3.1/include/assimp/postprocess.h"

#include "Mesh.h"
#include "D3DUtil.h"
#include "FX.h"
#include "D3D.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void SubMesh::Release()
{
  ReleaseCOM(pVB_);
  ReleaseCOM(pIB_);
  numIndices_ = numVerts_ = 0;
}

bool SubMesh::Initialise(ID3D11Device* device, FX::TexCache& texCache, const aiScene* scene, const aiMesh* mesh)
{
  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  if (scene->HasMaterials()) // Check that there are textures and materials
  {
    aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];
    if (aimaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) // If there is a diffuse texture
    {
      aiString path;
      if (aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
      {
        string textureName = path.C_Str();
        StripPathAndExtension(textureName);
        material_.texture = textureName;
        textureName += ".dds";
        material_.pTextureRV = texCache.LoadTexture(textureName, true, device);
      }
    }
    aiString name;
    aimaterial->Get(AI_MATKEY_NAME, name);
    material_.name = name.C_Str();

    aiColor3D diffuse(1, 1, 1), ambient(1, 1, 1), specular(1, 1, 1);
    float power = 1;
    aimaterial->Get(AI_MATKEY_SHININESS_STRENGTH, power);
    aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    material_.gfxData.Set(Vector4(diffuse.r, diffuse.g, diffuse.b, 1), Vector4(ambient.r, ambient.g, ambient.b, 1), Vector4(specular.r, specular.g, specular.b, power));
  }

  assert(mesh->HasFaces());
  numVerts_ = numIndices_ = mesh->mNumFaces * 3;

  // Create the Mesh using the vertex count that was read in.
  vector<VertexPosNormTex> verts;
  verts.insert(verts.begin(), numVerts_, VertexPosNormTex());
  vector<unsigned int> indices;
  indices.insert(indices.begin(), numIndices_, 0);

  int curentVert = 0;
  for (int f = 0; f < (int)mesh->mNumFaces; f++)
  {
    for (int i = 0; i < (int)mesh->mFaces[f].mNumIndices; i++)
    {
      int ind = mesh->mFaces[f].mIndices[i];

      const aiVector3D* pos = &(mesh->mVertices[ind]);
      const aiVector3D* norm = &(mesh->mNormals[ind]);
      const aiVector3D* tex = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][ind]) : &Zero3D;

      verts[curentVert].Pos.x = pos->x;
      verts[curentVert].Pos.y = pos->y;
      verts[curentVert].Pos.z = pos->z;

      verts[curentVert].Norm.x = norm->x;
      verts[curentVert].Norm.y = norm->y;
      verts[curentVert].Norm.z = norm->z;

      verts[curentVert].Tex.x = tex->x;
      verts[curentVert].Tex.y = tex->y;

      indices[curentVert] = curentVert;

      curentVert++;
    }
  }

  CreateVertexBuffer(sizeof(VertexPosNormTex) * numVerts_, &verts[0], pVB_);
  CreateIndexBuffer(sizeof(unsigned int) * numIndices_, &indices[0], pIB_);

  return true;
}




void Mesh::CreateFrom(const string& fileName, ID3D11Device* pD3DDevice, FX::TexCache& texCache)
{
  name_ = fileName;
  string path, fstring(fileName);
  StripPathAndExtension(fstring, &path);
  string oldPath = texCache.GetAssetPath();
  texCache.SetAssetPath(path);

#if defined(DEBUG) | defined(_DEBUG)
  //disable any memory leak checking, this isn't our code!
  int tmp = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  _CrtSetDbgFlag(0);
#endif

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(fileName,
    aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
    aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
    aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
    aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
    aiProcess_SortByPType | // ?
    aiProcess_RemoveRedundantMaterials | // remove redundant materials
    aiProcess_FindDegenerates | // remove degenerated polygons from the import
    aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
    aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
    aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
    aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
    aiProcess_OptimizeMeshes | // join small meshes, if possible;
    aiProcess_GenSmoothNormals | //if no normals, make them
    0
  );

  if (scene == NULL) // If there is no valid scene it might be using the old model format
  {
    DBOUT("Couldn't load " << name_);
    assert(false);
    return;
  }

  //turn any checking back on
#if defined(DEBUG) | defined(_DEBUG)
  _CrtSetDbgFlag(tmp);
#endif
  if (scene->HasMeshes())
  {
    for (int i = 0; i < (int)scene->mNumMeshes; i++)
    {
      subMeshes_.push_back(new SubMesh);
      subMeshes_.back()->Initialise(pD3DDevice, texCache, scene, scene->mMeshes[i]);
    }
  }

#if defined(DEBUG) | defined(_DEBUG)
  //disable any memory leak checking, this isn't our code!
  _CrtSetDbgFlag(0);
#endif

  importer.FreeScene();

  //turn any checking back on
#if defined(DEBUG) | defined(_DEBUG)
  _CrtSetDbgFlag(tmp);
#endif

  texCache.SetAssetPath(oldPath);
}


Mesh& MeshManager::CreateMesh(const std::string & name)
{
  for (int i = 0; i < (int)meshes_.size(); ++i)
    if (meshes_[i]->name_ == name)
      assert(false);
  meshes_.push_back(new Mesh(name));
  return *(meshes_.back());
}

void MeshManager::Release()
{
  for (int i = 0; i < (int)meshes_.size(); ++i) {
    meshes_[i]->Release();
    delete meshes_[i];
  }
  meshes_.clear();
}

Mesh* MeshManager::GetMesh(const std::string & name)
{
  for (int i = 0; i < (int)meshes_.size(); ++i)
    if (meshes_[i]->name_ == name)
      return meshes_[i];
  return nullptr;
}

void Mesh::Release()
{
  for (int i = 0; i < (int)subMeshes_.size(); ++i) {
    subMeshes_[i]->Release();
    delete subMeshes_[i];
  }
  subMeshes_.clear();
}



void Mesh::CreateFrom(const VertexPosNormTex verts[], int numVerts, const unsigned int indices[], int numIndices,
  const MaterialExt & mat, int meshNumIndices)
{
  Release();
  SubMesh* p = new SubMesh;
  subMeshes_.push_back(p);
  p->numIndices_ = meshNumIndices;
  p->numVerts_ = numVerts;
  p->material_ = mat;
  CreateVertexBuffer(sizeof(VertexPosNormTex) * numVerts, verts, p->pVB_);
  CreateIndexBuffer(sizeof(unsigned int) * numIndices, indices, p->pIB_);

}

