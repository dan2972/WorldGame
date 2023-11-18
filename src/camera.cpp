#include "camera.h"
#include "stdio.h"

void Camera::updateProjection(int width, int height) {
    m_projection = glm::perspective(glm::radians(getZoom()), (float)width / (float)height, 0.1f, 5000.0f);
    m_projView = m_projection * glm::lookAt(m_position, m_position + m_front, m_up);
}

const glm::mat4& Camera::getProjViewMatrix() const{
	return m_projView;
}

float Camera::getZoom() {
    return m_zoom;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    glm::vec3 forward = glm::normalize(glm::vec3{ m_front.x, 0, m_front.z }) * velocity;
    if (direction == Forward) {
        m_position.x += forward.x;
        m_position.z += forward.z;
    } if (direction == Backward) {
        m_position.x -= forward.x;
        m_position.z -= forward.z;
    } if (direction == Left) {
        m_position -= m_right * velocity;
    } if (direction == Right) {
        m_position += m_right * velocity;
    } if (direction == Up) {
        m_position += m_worldUp * velocity;
    } if (direction == Down) {
        m_position -= m_worldUp * velocity;
    }
    // printf("x: %f, y: %f, z: %f\n", m_position.x, m_position.y, m_position.z);
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front{};
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::vec3 Camera::getPosition() {
    return m_position;
}

void Camera::setPosition(const glm::vec3& position) {
    m_position = position;
}