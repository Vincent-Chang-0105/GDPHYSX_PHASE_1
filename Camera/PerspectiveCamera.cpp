#include "PerspectiveCamera.h"
#include <iostream>

PerspectiveCamera::PerspectiveCamera() : MyCamera()
{
	this->window_height = 700;
	this->window_width = 700;
	this->theta = 45.5f;
	this->phi = 89.5f;
	this->aspectRatio = window_height / window_width;
	this->SetProjectionMatrix(60.f);
	viewMatrix = glm::mat4(1.0f); // Initialize as identity matrix
}

glm::mat4 PerspectiveCamera::getViewProjection() const
{
	return projectionMatrix * viewMatrix;
}

void PerspectiveCamera::SetProjectionMatrix(float viewAngle)
{
	//dont edit anymore
	this->projectionMatrix = glm::perspective(glm::radians(viewAngle), this->aspectRatio, 0.1f, 500.0f);
}

//void PerspectiveCamera::Update(GLFWwindow* window, float time)
//{
//    float cameraSpeed = 2.5 * time;
//
//    // Calculate rotation around the origin
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//    {
//        // Rotate left around the origin
//        float angle = cameraSpeed;
//        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, up);
//        this->orientation = glm::vec3(rotation * glm::vec4(orientation, 1.0));
//        this->setCameraPosition(this->orientation);
//    }
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//    {
//        // Rotate right around the origin
//        float angle = -cameraSpeed;
//        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, up);
//        this->orientation = glm::vec3(rotation * glm::vec4(orientation, 1.0));
//    }
//
//    // Move forward/backward relative to the orientation
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//    {
//        this->cameraPos += cameraSpeed * this->orientation;
//        std::cout << "w pressed" << std::endl;
//        this->setCameraPosition(this->cameraPos);
//    }
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//    {
//        this->cameraPos -= cameraSpeed * this->orientation;
//        std::cout << "s pressed" << std::endl;
//        this->setCameraPosition(this->cameraPos);
//    }
//
//    // Update the view matrix
//    this->SetViewMatrix();
//}






