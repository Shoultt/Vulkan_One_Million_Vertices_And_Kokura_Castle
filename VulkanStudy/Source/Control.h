#include "Source/initWindow.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraPosFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

VkClearValue colorValue;

float yaw = -90.0f;	
float pitch = 0.0f;
float lastX = (float_t)getWindowWidth() / 2.0;
float lastY = (float_t)getWindowHeight() / 2.0;
float fov = 45.0f;
double xPosCursor = 0, yPosCursor = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool hiddenCursor = 0;
bool firstTime = true;

static void cursorPositionCallbacks(GLFWwindow* window, double xPosf, double yPosf)
{
		float xpos = static_cast<float>(xPosf);
		float ypos = static_cast<float>(yPosf);

		if (firstTime == true)
		{
			lastX = xpos;
			lastY = ypos;
			firstTime = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
};

void scrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	if (hiddenCursor == 0)
	{
		fov -= (float)yoffset;
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > 45.0f)
			fov = 45.0f;
	}
}

void keyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (hiddenCursor == 0)
		{ 
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			hiddenCursor += 1;
		}
		else
		{
			glfwGetCursorPos(window, &xPosCursor, &yPosCursor);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			hiddenCursor -= 1;
		}
	}
}

void control()
{
	if (hiddenCursor == 0)
	{
		float cameraSpeed = 5.0f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cameraSpeed *= 10;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp;
	}
	if (hiddenCursor == 0) {glfwSetCursorPosCallback(window, cursorPositionCallbacks);}
	else { glfwSetCursorPosCallback(window, 0); }

	if (hiddenCursor == 0)
		colorValue = { 0.0f, 0.0094f, 0.0656f, 1.0f };
	else
		colorValue = { 0.702f, 0.1412f, 0.1686f, 1.0f };

	glfwSetKeyCallback(window, keyCallbacks);

	glfwSetScrollCallback(window, scrollCallbacks);
}