#include <stdio.h>

#include "shadow_map_fbo.h"

ShadowMapFBO::ShadowMapFBO ( )
{
  m_fbo = 0;
  m_shadowMap = 0;
}

ShadowMapFBO::~ShadowMapFBO ( )
{
  if ( m_fbo != 0 )
  {
    glDeleteFramebuffers ( 1, &m_fbo );
  }

  if ( m_shadowMap != 0 )
  {
    glDeleteFramebuffers ( 1, &m_shadowMap );
  }
}

bool ShadowMapFBO::Init ( unsigned int windowWidth, unsigned int windowHeight )
{
  // Создаем FBO
  glGenFramebuffers ( 1, &m_fbo );

  // Создаем буфер глубины
  glGenTextures   ( 1, &m_shadowMap );

  glBindTexture   ( GL_TEXTURE_2D, m_shadowMap );

  glTexImage2D    ( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0/*border*/, GL_DEPTH_COMPONENT, GL_FLOAT, NULL/*startInit*/ );

  glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

  glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, m_fbo );
  glFramebufferTexture2D ( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0/*mipMapLevel*/ );

  // Отключаем запись в буфер цвета
  glDrawBuffer ( GL_NONE );

  GLenum status = glCheckFramebufferStatus ( GL_FRAMEBUFFER );
  if ( status != GL_FRAMEBUFFER_COMPLETE )
  {
    printf ( "FB error, status: 0x%x\n", status );
    return false;
  }

  return true;
}

void ShadowMapFBO::BindForWriting ( )
{
  glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, m_fbo );
}

void ShadowMapFBO::BindForReading ( GLenum textureUnit )
{
  glActiveTexture ( textureUnit );
  glBindTexture ( GL_TEXTURE_2D, m_shadowMap );
}