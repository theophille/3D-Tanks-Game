#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "tank.h"

namespace game
{
    class FollowCamera {
    public:
        glm::vec3 initialForward;
        glm::vec3 initialRight;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        float distanceToTarget;

        FollowCamera(Tank* follow, const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            distanceToTarget = glm::distance(follow->getPosition(), center - position);
            Set(position, center, up);
        }

        ~FollowCamera() {}

        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            initialForward = forward;
            right = glm::cross(forward, up);
            initialRight = right;
            this->up = glm::cross(right, forward);
        }

        void SetPos(const glm::vec3& position)
        {
            this->position = position;
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += glm::normalize(glm::vec3(forward)) * distance;
        }

        void TranslateUpward(float distance)
        {
            position += glm::normalize(glm::vec3(up)) * distance;
        }

        void TranslateRight(float distance)
        {
            position += glm::normalize(glm::vec3(right)) * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle, right);
            glm::vec4 rotVec = glm::normalize(rotMat * glm::vec4(forward, 1.0f));
            forward = glm::normalize(glm::vec3(rotVec));
            up = glm::cross(right, forward);
        }

        void RotateCameraByAngle_OY(float angle) {
            glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
            glm::vec4 rotVecFw = normalize(rotMat * glm::vec4(initialForward, 1.0f));
            glm::vec4 rotVecRg = normalize(rotMat * glm::vec4(initialRight, 1.0f));

            forward = glm::normalize(glm::vec3(rotVecFw));
            right = glm::normalize(glm::vec3(rotVecRg));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OY(float angle)
        {
            glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
            glm::vec4 rotVecFw = normalize(rotMat * glm::vec4(forward, 1.0f));
            glm::vec4 rotVecRg = normalize(rotMat * glm::vec4(right, 1.0f));

            forward = glm::normalize(glm::vec3(rotVecFw));
            right = glm::normalize(glm::vec3(rotVecRg));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle, forward);
            glm::vec4 rotVecRg = normalize(rotMat * glm::vec4(right, 1.0f));
            glm::vec4 rotVecUp = normalize(rotMat * glm::vec4(up, 1.0f));

            right = glm::normalize(glm::vec3(rotVecRg));
            up = glm::normalize(glm::vec3(rotVecUp));
        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateAroundSubject(float angle) {
            TranslateForward(distanceToTarget);
            RotateCameraByAngle_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }
    };
}
