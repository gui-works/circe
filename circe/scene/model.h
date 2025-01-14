/// Copyright (c) 2020, FilipeCN.
///
/// The MIT License (MIT)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///
///\file model.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2020-17-10
///
///\brief

#ifndef CIRCE_CIRCE_GL_SCENE_MODEL_H
#define CIRCE_CIRCE_GL_SCENE_MODEL_H

#include <hermes/storage/array_of_structures.h>
#include <hermes/common/file_system.h>
#include <circe/gl/storage/vertex_array_object.h>
#include <circe/gl/storage/vertex_buffer.h>
#include <circe/gl/storage/index_buffer.h>
#include <circe/gl/graphics/shader.h>
#include <circe/scene/shape_options.h>

namespace circe {

/// Stores mesh data in interleaved fashion
class Model {
public:
  // ***********************************************************************
  //                          STATIC METHODS
  // ***********************************************************************
  static Model fromFile(const hermes::Path &path, shape_options options = shape_options::none);
  // ***********************************************************************
  //                           CONSTRUCTORS
  // ***********************************************************************
  Model();
  Model(Model &&other) noexcept;
  virtual ~Model();
  // ***********************************************************************
  //                            OPERATORS
  // ***********************************************************************
  Model &operator=(hermes::AoS &&data);
  Model &operator=(const hermes::AoS &data);
  Model &operator=(const std::vector<i32> &indices);
  Model &operator=(const std::vector<f32> &vertex_data);
  Model &operator=(Model &&other) noexcept;
  Model &operator=(const Model &other);
  friend std::ostream &operator<<(std::ostream &o, const Model &model);
  // ***********************************************************************
  //                             METHODS
  // ***********************************************************************
  template<typename T>
  hermes::AoSFieldView<T> attributeAccessor(const std::string &attribute_name) {
    return data_.field<T>(attribute_name);
  }
  template<typename T>
  hermes::AoSFieldView<T> attributeAccessor(u64 attribute_index) {
    return data_.field<T>(attribute_index);
  }
  template<typename T>
  hermes::ConstAoSFieldView<T> attributeAccessor(const std::string &attribute_name) const {
    return data_.field<T>(attribute_name);
  }
  template<typename T>
  hermes::ConstAoSFieldView<T> attributeAccessor(u64 attribute_index) const {
    return data_.field<T>(attribute_index);
  }
  template<typename T>
  T &attributeValue(u64 attribute_index, u64 vertex_index) { return data_.valueAt<T>(attribute_index, vertex_index); }
  template<typename T>
  u64 pushAttribute(const std::string &attribute_name) {
    return data_.pushField<T>(attribute_name);
  }
  const hermes::AoS &data() const { return data_; }
  const std::vector<i32> &indices() const { return indices_; }
  hermes::GeometricPrimitiveType primitiveType() const { return element_type_; }
  void resize(u64 new_size);
  void setIndices(std::vector<i32> &&indices);
  void setPrimitiveType(hermes::GeometricPrimitiveType primitive_type);
  u64 elementCount() const;

  hermes::bbox3 boundingBox() const;
  void fitToBox(const hermes::bbox3 &box = hermes::bbox3::unitBox());

protected:
  hermes::AoS data_;
  std::vector<i32> indices_;
  hermes::GeometricPrimitiveType element_type_{hermes::GeometricPrimitiveType::TRIANGLES};
};

}

#endif // CIRCE_CIRCE_GL_SCENE_MODEL_H
