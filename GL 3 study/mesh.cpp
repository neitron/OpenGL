#pragma once

#include "mesh.h"

Mesh::MeshEntry::MeshEntry ( )
{
  VB = INVALID_OGL_VALUE;
  IB = INVALID_OGL_VALUE;

  numIndices = 0;
  materialIndex = INVALID_MATERIAL;
}

Mesh::MeshEntry::~MeshEntry ( )
{
  if ( VB != INVALID_OGL_VALUE )
  {
    glDeleteBuffers ( 1, &VB );
  }

  if ( IB != INVALID_OGL_VALUE )
  {
    glDeleteBuffers ( 1, &IB );
  }
}

bool Mesh::MeshEntry::Init ( const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices )
{
  numIndices = indices.size ( );

  glGenBuffers ( 1, &VB );
  glBindBuffer ( GL_ARRAY_BUFFER, VB );
  glBufferData ( GL_ARRAY_BUFFER, sizeof ( Vertex )*vertices.size ( ), &vertices[0], GL_STATIC_DRAW );

  glGenBuffers ( 1, &IB );
  glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, IB );
  glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( unsigned int )*numIndices, &indices[0], GL_STATIC_DRAW );

  return true;
}

void Mesh::Clear ( )
{
  for ( unsigned int i = 0; i < m_textures.size ( ); i++ )
  {
    SAFE_DELETE ( m_textures[i] );
  }
}

bool Mesh::LoadMesh ( const std::string& filename )
{
  // Release the previously loaded mesh (if it exists)
  Clear ( );

  Assimp::Importer importer;

  const aiScene* pScene = importer.ReadFile ( filename.c_str ( ), 
    aiPostProcessSteps::aiProcess_Triangulate |
    aiPostProcessSteps::aiProcess_GenSmoothNormals |
    aiPostProcessSteps::aiProcess_FlipUVs );
  
  bool ret = false;

  if ( pScene ) 
  {
    ret = InitFromScene ( pScene, filename );
  }
  else 
  {
    printf ( "Error parsing '%s': '%s'\n", filename.c_str ( ), importer.GetErrorString ( ) );
  }

  return ret;
}

bool Mesh::InitFromScene ( const aiScene* pScene, const std::string& filename )
{
  m_entries.resize ( pScene->mNumMeshes );
  m_textures.resize ( pScene->mNumMaterials );

  // Initialize the meshes in the scene one by one
  for ( unsigned int i = 0; i < m_entries.size ( ); i++ ) 
  {
    const aiMesh* paiMesh = pScene->mMeshes[i];
    InitMesh ( i, paiMesh );
  }

  return InitMaterials ( pScene, filename );
}

void Mesh::InitMesh ( unsigned int index, const aiMesh* paiMesh )
{
  m_entries[index].materialIndex = paiMesh->mMaterialIndex;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  const aiVector3D zero3D ( 0.0f, 0.0f, 0.0f );

  for ( unsigned int i = 0; i < paiMesh->mNumVertices; i++ )
  {
    const aiVector3D* pPos = &( paiMesh->mVertices[i] );
    const aiVector3D* pNormal = &( paiMesh->mNormals[i] );
    const aiVector3D* pTexCoord = paiMesh->HasTextureCoords ( 0 ) ? &( paiMesh->mTextureCoords[0][i] ) : &zero3D;

    Vertex v ( 
      Vector3f ( pPos->x, pPos->y, pPos->z ),
      Vector2f ( pTexCoord->x, pTexCoord->y ),
      Vector3f ( pNormal->x, pNormal->y, pNormal->z ) 
      );

    vertices.push_back ( v );
  }

  for ( unsigned int i = 0; i < paiMesh->mNumFaces; i++ )
  {
    const aiFace& Face = paiMesh->mFaces[i];
    assert ( Face.mNumIndices == 3 );
    indices.push_back ( Face.mIndices[0] );
    indices.push_back ( Face.mIndices[1] );
    indices.push_back ( Face.mIndices[2] );
  }

  m_entries[index].Init ( vertices, indices );
}

bool Mesh::InitMaterials ( const aiScene* pScene, const std::string& filename )
{
  // Извлекаем директорию из полного имени файла
  std::string::size_type slashIndex = filename.find_last_of ( "/" );
  std::string dir;

  if ( slashIndex == std::string::npos )
  {
    dir = ".";
  }
  else if ( slashIndex == 0 )
  {
    dir = "/";
  }
  else 
  {
    dir = filename.substr ( 0, slashIndex );
  }

  bool ret = true;

  // Инициализируем материал
  for ( unsigned int i = 0; i< pScene->mNumMaterials; i++ )
  {
    const aiMaterial* pMaterial = pScene->mMaterials[i];

    m_textures[i] = nullptr;

    if ( pMaterial->GetTextureCount ( aiTextureType::aiTextureType_DIFFUSE ) > 0 )
    {
      aiString path;

      if ( pMaterial->GetTexture ( aiTextureType::aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr ) == AI_SUCCESS )
      {
        std::string fullPath = dir + "/" + path.data;
        m_textures[i] = new Texture ( GL_TEXTURE_2D, fullPath.c_str ( ) );

        if ( !m_textures[i]->Load ( ) )
        {
          printf ( "Error loading texture '%s'\n", fullPath.c_str ( ) );
          delete m_textures[i];
          m_textures[i] = nullptr;
          ret = false;
        }
      }
    }

    // Загружаем белую текстуру если модель не имеет собственной
    if ( !m_textures[i] )
    {
      m_textures[i] = new Texture ( GL_TEXTURE_2D, "../Content/white.png" );
      ret = m_textures[i]->Load ( );
    }
  }

  return ret;
}

void Mesh::Render ( )
{
  glEnableVertexAttribArray ( 0 );
  glEnableVertexAttribArray ( 1 );
  glEnableVertexAttribArray ( 2 );

  for ( unsigned int i = 0; i < m_entries.size ( ); i++ )
  {
    glBindBuffer ( GL_ARRAY_BUFFER, m_entries[i].VB );

    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), 0 );
    glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( const GLvoid* ) 12 );
    glVertexAttribPointer ( 2, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( const GLvoid* ) 20 );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_entries[i].IB );

    const unsigned int materialIndex = m_entries[i].materialIndex;

    if ( materialIndex < m_textures.size ( ) && m_textures[ materialIndex ] )
    {
      m_textures[ materialIndex ]->Bind ( GL_TEXTURE0 );
    }

    glDrawElements ( GL_TRIANGLES, m_entries[i].numIndices, GL_UNSIGNED_INT, 0 );
  }

  glDisableVertexAttribArray ( 2 );
  glDisableVertexAttribArray ( 1 );
  glDisableVertexAttribArray ( 0 );
}