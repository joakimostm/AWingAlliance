#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <memory>
#include <model.h>
#include <iostream>
#include <array>

#include <GL/glew.h>

#include "shaders.h"

struct OBJGroup
{
    explicit OBJGroup(const std::vector<float>& buf,
                      const uint tex_id,
                      const std::string& tex_name);

    const std::vector<float> buffer;
    const std::string texture_name;
    const uint texture_id;
};

struct DrawObject
{
    explicit DrawObject(const OBJGroup& group);
    ~DrawObject();

    // 3:vtx, 3:normal, 3:col, 2:texcoord
    static constexpr int attribs_per_vertex = (3 + 3 + 3 + 2);

    unsigned int vbo, vao;
    int numTriangles;
    bool use_color;
    unsigned int texture_id;
    std::string texture_name;
};

struct BoundingBox
{
    explicit BoundingBox(const std::array<float, 3>& bmin, const std::array<float, 3>& bmax);

    bool is_inside(const glm::vec3& point) const;
    bool is_inside(const BoundingBox& other, const glm::mat4& relative_pose) const;

    std::array<float, 3> bmin;
    std::array<float, 3> bmax;
    std::array<glm::vec3, 8> corners;

    glm::vec3 scale;
    glm::vec3 offset;
    glm::mat4 pose;
};

class Model
{
  public:
    static std::unique_ptr<Model> Create(const std::string& name,
                                         const std::vector<OBJGroup>& groups,
                                         const BoundingBox& bbox);
    explicit Model(const std::string& name,
                   const std::vector<OBJGroup>& groups,
                   const BoundingBox& bbox);

    const BoundingBox& GetBoundingBox() const;
    const std::string& GetName() const;

    void Draw() const;
    void DrawWireframe() const;
    void Draw(const glm::mat4& mvp, const glm::vec3& color, const uint program) const;

  private:
    std::string name;
    std::vector<std::unique_ptr<DrawObject>> drawobjects;
    BoundingBox boundingbox;
};

class TerrainModel
{
  public:
    explicit TerrainModel(const std::vector<uint>& texture_ids,
                          const std::vector<float>& buffer,
                          const std::vector<uint>& indices,
                          const float max_height,
                          const uint program);
    ~TerrainModel();

    // 3:vtx, 3:normal, 2:texcoord
    static constexpr int attribs_per_vertex = (3 + 3 + 2);

    void Draw(const glm::mat4& mvp, const uint program) const;

  private:
    const std::vector<uint> texture_ids;
    const unsigned int num_indices;
    uint vao, vboVertices, vboIndices;
};
