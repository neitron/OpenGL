#ifndef MESH_H
#define MESH_H

#pragma once

#include <vector>
#include <GL/glew.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "utils.h"
#include "math3d.h"
#include "texture.h"



struct Vertex
{
  Vector3f m_pos;
  Vector2f m_tex;
  Vector3f m_normal;
  Vector3f m_tangent;

  Vertex ( ) {  }

  Vertex (
    const Vector3f& pos,
    const Vector2f& tex,
    const Vector3f& normal,
    const Vector3f& tangent ) :
    m_pos     ( pos ),
    m_tex     ( tex ),
    m_normal  ( normal ),
    m_tangent ( tangent )
  {  }
};





class Mesh
{
public:
  Mesh ( ) { };
  ~Mesh ( )
  {
    Clear ( );
  };

  bool LoadMesh ( const std::string& filename );
  void Render ( );

private:

  bool InitFromScene  ( const aiScene* pScene, const std::string& filename );
  void InitMesh       ( unsigned int index, const aiMesh* paiMesh );
  bool InitMaterials  ( const aiScene* pScene, const std::string& filename );

  void Clear ( );

  struct MeshEntry
  {
    MeshEntry ( );
    ~MeshEntry ( );

    bool Init ( const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices );

    GLuint VB;
    GLuint IB;

    unsigned int numIndices;
    unsigned int materialIndex;
  };

  static const unsigned int INVALID_MATERIAL = 0xFFFFFFFF;

  std::vector<MeshEntry>  m_entries;
  std::vector<Texture*>   m_textures;
};


#endif /* MESH_H */
