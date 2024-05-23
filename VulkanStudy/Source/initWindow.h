#pragma once
#include <GLFW/glfw3.h>

#include "Textures/textureReader.h"
#include <Windows.h>

uint32_t Wwidth;
uint32_t Wheight;
GLFWwindow* window;

void createWindow()
{
	bool rat = 1;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

	if (rat == 1)
	{
		Wwidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
		Wheight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

		window = glfwCreateWindow(Wwidth, Wheight, "Window", glfwGetPrimaryMonitor(), nullptr);
	}
	else
	{
		Wwidth = 1900;
		Wheight = 1000;

		window = glfwCreateWindow(Wwidth, Wheight, "Yes", nullptr, nullptr);
		
		glfwSetWindowPos(window, 10, 50);
	}
}

textureReader textureRead;

void createCursor()
{	
	textureRead.load("Textures/cursor.png");
	stbi_uc* pixels = static_cast<stbi_uc*>(textureRead.getPicture());
	memset(pixels, 0x00, sizeof(pixels));
	GLFWimage cursorImage{
	.width = textureRead.getTextureWidth(),
	.height = textureRead.getTextureHeight(),
	.pixels = pixels };
	GLFWcursor* cursor = glfwCreateCursor(&cursorImage, 0, 0);
	glfwSetCursor(window, cursor);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	textureRead.destroy();
}

uint32_t getWindowWidth()
{
	return Wwidth;
}

uint32_t getWindowHeight()
{
	return Wheight;
}


