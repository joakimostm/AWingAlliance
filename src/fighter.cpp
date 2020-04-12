#include <fighter.h>
#include <keyboard.h>

namespace actor
{
Fighter::Fighter(const glm::vec3& p, const glm::vec3& d, const Model* mdl)
  : Actor(p, d, mdl),
    currentSpeed(0.0),
    currentTurnSpeed(0.0),
    maxSpeed(50.0),
    maxTurnSpeed(1.0 * glm::pi<float>()),
    bDrawBBox(true),
    bDrawSpline(false)
{
    s = Spline(p, dir, 0.0, 5.0);
}
void Fighter::Update_Roaming(float t)
{
    if (t - s.start_time > s.duration)
    {
        s.UpdateRandom(pos, dir);
        s.UploadPoints();
    }

    float u = (t - s.start_time) / s.duration;  // [0, 1]
    std::pair<glm::vec3, glm::vec3> interp = s(u);
    pos = interp.first;
    dir = glm::normalize(interp.second);
}

void Update(const float speedChange, const float turnChange, const float dt)
{
}

void Fighter::Update(const std::bitset<8>& keyboardInfo, float dt)
{
    glm::vec3 rightVector = glm::cross(dir, glm::vec3(0.0, 1.0, 0.0));

    // std::cout << "dt: " << dt << '\n';
    if (keyboardInfo.test(KeyboardMapping::LEFTARROW))
    {
        dir = glm::vec3(glm::rotate(glm::mat4(1.0f), maxTurnSpeed * dt, glm::vec3(0, 1, 0)) *
                        glm::vec4(dir, 1.0));
    }
    if (keyboardInfo.test(KeyboardMapping::RIGHTARROW))
    {
        dir = glm::vec3(glm::rotate(glm::mat4(1.0f), -maxTurnSpeed * dt, glm::vec3(0, 1, 0)) *
                        glm::vec4(dir, 1.0));
    }
    if (keyboardInfo.test(KeyboardMapping::UPARROW) && dir.y > -0.9)
    {
        dir = glm::vec3(glm::rotate(glm::mat4(1.0f), -maxTurnSpeed * dt, rightVector) *
                        glm::vec4(dir, 1.0));
    }
    if (keyboardInfo.test(KeyboardMapping::DOWNARROW) && dir.y < 0.9)
    {
        dir = glm::vec3(glm::rotate(glm::mat4(1.0f), maxTurnSpeed * dt, rightVector) *
                        glm::vec4(dir, 1.0));
    }

    if (keyboardInfo.test(KeyboardMapping::W))
        pos += glm::vec3(maxSpeed * dt * dir);
    if (keyboardInfo.test(KeyboardMapping::S))
        pos -= glm::vec3(maxSpeed * dt * dir);
    if (keyboardInfo.test(KeyboardMapping::SPACEBAR))
        pos = glm::vec3(0.0);
}

void Fighter::Draw(glm::mat4 camprojMat)
{
    // Construct Model Matrix from Position and Viewing Direction
    mdlMatrix = glm::inverse(glm::lookAt(glm::vec3(0.0, 0.0, 0.0), -dir, glm::vec3(0.0, 1.0, 0.0)));
    mdlMatrix = glm::translate(glm::mat4(1.0F), pos) * mdlMatrix;

    glm::mat4 mvp = camprojMat * mdlMatrix;
    model->Draw(mvp, color);
    // if (bDrawBBox)
    // {
    //     glUniform3f(glGetUniformLocation(program, "uniform_color"), 0.0, 1.0, 0.0);
    //     glUniform1i(glGetUniformLocation(program, "bUseColor"), 1);

    //     glm::mat4 mvp2 = mvp * model->boundingbox.pose;
    //     glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE,
    //     glm::value_ptr(mvp2)); glBindVertexArray(Models["cube"]->drawobjects[0]->vao);
    //     glDrawArrays(GL_POINTS, 0, 3 * 13);
    // }
    // if (bDrawSpline)
    // {
    //     glUniform3f(glGetUniformLocation(program, "uniform_color"), 0.0, 0.0, 1.0);
    //     glUniform1i(glGetUniformLocation(program, "bUseColor"), 1);
    //     s.Draw(camprojMat);
    // }
}
}  // namespace actor