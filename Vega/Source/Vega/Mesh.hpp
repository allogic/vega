#ifndef VEGA_MESH_HPP
#define VEGA_MESH_HPP

#include <glad/glad.h>

#include <vector>

#include <Vega/Vertex.hpp>

namespace Vega {
  class Mesh final {
  public:
    explicit Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

    void Bind() const;

  private:
    unsigned int mSize;

    unsigned int mVao = 0;
    unsigned int mVbo = 0;
    unsigned int mEbo = 0;
  };
}

#endif