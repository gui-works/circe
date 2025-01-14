/*
 * Copyright (c) 2017 FilipeCN
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
*/

#ifndef CIRCE_IO_FRAMEBUFFER_H
#define CIRCE_IO_FRAMEBUFFER_H

#include <circe/gl/texture/texture.h>
#include <hermes/common/size.h>

namespace circe::gl {

/// Holds a opengl's framebuffer object
class Framebuffer {
public:
  // ***********************************************************************
  //                          STATIC METHODS
  // ***********************************************************************
  static void disable();
  // ***********************************************************************
  //                           CONSTRUCTORS
  // ***********************************************************************
  Framebuffer();
  /// \param resolution in pixels
  explicit Framebuffer(const hermes::size2 &resolution);
  /// \param resolution in pixels
  explicit Framebuffer(const hermes::size3 &resolution);
  virtual ~Framebuffer();
  // ***********************************************************************
  //                             SIZE
  // ***********************************************************************
  [[nodiscard]] hermes::size2 size() const;
  /// \param resolution in pixels
  void resize(const hermes::size2 &resolution);
  /// \param resolution in pixels
  void resize(const hermes::size3 &resolution);
  // ***********************************************************************
  //                             RENDER BUFFER
  // ***********************************************************************
  void setRenderBufferStorageInternalFormat(GLenum format);
  // ***********************************************************************
  //                             METHODS
  // ***********************************************************************
  void enable() const;
  ///
  /// \param textureId
  /// \param target
  /// \param attachmentPoint
  void attachColorBuffer(GLuint textureId, GLenum target,
                         GLenum attachmentPoint = GL_COLOR_ATTACHMENT0,
                         GLint mip_level = 0) const;
  ///
  /// \param texture
  /// \param attachment_point
  void attachTexture(const Texture &texture, GLenum attachment_point = GL_COLOR_ATTACHMENT0, GLint mip_level = 0) const;
  /// \note call glDrawBuffers
  /// \param buffers
  void setOutputBuffers(const std::vector<GLenum> &buffers) const;
  ///
  /// \param mask
  /// \param filter
  void blit(GLbitfield mask, GLenum filter = GL_NEAREST) const;
  // ***********************************************************************
  //                             RENDERING
  // ***********************************************************************
  void render(const std::function<void()> &render_function) const;
  // ***********************************************************************
  //                             PUBLIC FIELDS
  // ***********************************************************************
  circe::Color clear_color{circe::Color::Black()};
  GLbitfield clear_bitfield_mask{GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT};
private:
  hermes::size3 size_in_pixels_;
  GLuint framebuffer_object_{0};
  GLuint render_buffer_object_{0};

  GLenum render_buffer_internal_format_{GL_DEPTH_COMPONENT};
};

} // circe namespace

#endif // CIRCE_IO_FRAMEBUFFER_H
