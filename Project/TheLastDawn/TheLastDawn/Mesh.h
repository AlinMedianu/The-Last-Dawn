// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "ShaderTypes.h"

struct aiScene;
struct aiMesh;
namespace FX { class TexCache; }

/// <summary>
/// Part of a vertex / index buffer that uses the same material(colourand texture).
/// </summary>
class SubMesh
{
public:
  void Release();
  bool Initialise(ID3D11Device* device, FX::TexCache& texCache, const aiScene* scene, const aiMesh* mesh);

  //buffer data
  ID3D11Buffer* pVB_ = nullptr;
  ID3D11Buffer* pIB_ = nullptr;
  int numIndices_ = 0;
  int numVerts_ = 0;

  MaterialExt material_;
};

/// <summary>
/// A mesh contains all the vertex data for a 3D object
/// the indices give the triangle order. Where a mesh uses
/// different materials (colour or textures) it needs to be
/// rendered on material at a time, so the buffer is split
/// into different parts.
/// </summary>
class Mesh
{
public:
  Mesh(const std::string& name) : name_(name) {}
  void Release();
  void CreateFrom(const VertexPosNormTex verts[], int numVerts, const unsigned int indices[],
    int numIndices, const MaterialExt& mat, int meshNumIndices);
  void CreateFrom(const std::string& fileName, ID3D11Device* pD3DDevice, FX::TexCache& texCache);
  int GetNumSubMeshes() const {
    return (int)subMeshes_.size();
  }
  SubMesh& GetSubMesh(int idx) {
    return *subMeshes_.at(idx);
  }
  
  std::string name_;

private:
  Mesh(const Mesh& m) = delete;
  Mesh& operator=(const Mesh& m) = delete;

  std::vector<SubMesh*> subMeshes_;
};

/// <summary>
/// A mesh is only ever loaded once, many model instances can use the same
/// mesh e.g. one tree mesh used to render 1000 trees, all different sizes,
/// orientations, positions.
/// </summary>
class MeshManager
{
public:
  void Release();
  Mesh* GetMesh(const std::string& name);
  Mesh& CreateMesh(const std::string& name);

  std::vector<Mesh*> meshes_;
};

#endif
