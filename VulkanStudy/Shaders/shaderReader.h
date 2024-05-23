#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

std::vector<char> shaderReader(std::string filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (file.is_open())
	{
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> fileCode(fileSize);
		file.seekg(0);
		file.read(fileCode.data(), fileSize);
		file.close();
		return fileCode;
	}
}