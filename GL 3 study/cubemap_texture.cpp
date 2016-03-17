#include "cubemap_texture.h"

#include <SOIL.h>

#include "utils.h"


// Targets of textures
static const GLenum types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z  };


CubemapTexture::CubemapTexture ( const string & Directory, 
  const string & PosXFilename, 
  const string & NegXFilename, 
  const string & PosYFilename, 
  const string & NegYFilename, 
  const string & PosZFilename, 
  const string & NegZFilename )
{
  auto it = Directory.end ( );
  it--;
  string BaseDir = "";//( *it == '/' ) ? Directory : Directory + "/";

  m_images[0].fileName = PosXFilename;
  m_images[1].fileName = NegXFilename;
  m_images[2].fileName = PosYFilename;
  m_images[3].fileName = NegYFilename;
  m_images[4].fileName = PosZFilename;
  m_images[5].fileName = NegZFilename;

  m_textureObj = 0;
}

CubemapTexture::~CubemapTexture ( )
{
  if ( m_textureObj != 0 ) 
  {
    glDeleteTextures ( 1, &m_textureObj );
  }
}

bool CubemapTexture::Load ( )
{
  glGenTextures ( 1, &m_textureObj );
  glBindTexture ( GL_TEXTURE_CUBE_MAP, m_textureObj );

  unsigned char* pImage = nullptr;

  for ( unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS ( types ); i++ ) 
  {
    pImage = SOIL_load_image ( m_images[i].fileName.c_str ( ), &m_images[i].width, &m_images[i].height, 0, SOIL_LOAD_RGBA );

    if ( !pImage )
    {
      printf ( "Error loading image\n" );
      return false;
    }

    glTexImage2D ( types[i], 0, GL_RGB, m_images[i].width, m_images[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage );

    delete pImage;
  }

  glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

  return true;
}

void CubemapTexture::Bind ( GLenum TextureUnit )
{
  glActiveTexture ( TextureUnit );
  glBindTexture ( GL_TEXTURE_CUBE_MAP, m_textureObj );
}
