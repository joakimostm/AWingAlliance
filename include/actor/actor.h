#pragma once

#include <glm/glm.hpp>

#include "model.h"

namespace actor
{
class Actor
{
  public:
    void SetPosition(const glm::vec3& pos);
    void SetDirection(const glm::vec3& dir);
    void SetPose(const glm::vec3& pos, const glm::vec3& dir);

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetDirection() const;
    const glm::mat4 GetPose() const;
    const float GetMaxSpeed() const;

    const Model* GetModel() const;
    const glm::vec3& GetColor() const;

    virtual void Update(const float dt) = 0;

  protected:
    explicit Actor(const glm::vec3& p, const glm::vec3& d, const Model* mdl);
    glm::vec3 pos;
    glm::vec3 dir;
    const Model* model;
    glm::vec3 color = { 0.0f, 0.0f, 0.7f };
    const float max_speed = 50.0f;
};
}  // namespace actor
