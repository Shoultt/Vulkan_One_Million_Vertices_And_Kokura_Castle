#pragma once

#include "Source/VulkanMonotony.h"
#define STB_IMAGE_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#include <CGLTF/cgltf.h>
#include <vector>

void fillBuffer(uint32_t inputStride, void* inputData, uint32_t outputStride, void* outputData, uint32_t numElements, uint32_t elementSize) {
	uint8_t* output = static_cast<uint8_t*>(outputData);
	uint8_t* input = (uint8_t*)inputData;
	for (uint32_t i = 0; i < numElements; ++i) {
		for (uint32_t b = 0; b < elementSize; ++b) {
			output[b] = input[b];
		}
		output += outputStride;
		input += inputStride;
	}
}

class modelReader
{
private:
	cgltf_data* data = 0;
	uint32_t indicesCount;
	uint32_t indexDataSize;
	void* indexData;
	uint64_t vertexDataSize;
	uint8_t* vertexData;
	void* positionData;
	void* normalData;
	void* texcoordData;
public:
	void load(const char* filename, bool normals, bool texCoords, int count)
	{
		cgltf_options options = {};
		cgltf_result error = cgltf_parse_file(&options, filename, &data);
		if (error == cgltf_result_success)
		{
			error = cgltf_load_buffers(&options, data, "Models");
			if (error == cgltf_result_success)
			{
				uint8_t* bufferBase = (uint8_t*)data->meshes[0].primitives[0].indices->buffer_view->buffer->data;
				indexDataSize = data->meshes[0].primitives[0].indices->buffer_view->size;
				indexData = bufferBase + data->meshes[0].primitives[0].indices->buffer_view->offset;
				indicesCount = data->meshes[0].primitives[0].indices->count;

				uint64_t outputStride = sizeof(float) * count;
				uint64_t numVertices = data->meshes[0].primitives[0].attributes->data->count;
				vertexDataSize = outputStride * numVertices;
				vertexData = new uint8_t[vertexDataSize];

				for (uint64_t i = 0; i < data->meshes[0].primitives[0].attributes_count; ++i) {
					cgltf_attribute* attribute = data->meshes[0].primitives[0].attributes + i;
					bufferBase = (uint8_t*)attribute->data->buffer_view->buffer->data;
					uint64_t inputStride = attribute->data->stride;
					if (attribute->type == cgltf_attribute_type_position) {
						positionData = bufferBase + attribute->data->buffer_view->offset;
						fillBuffer(inputStride, positionData, outputStride, vertexData, numVertices, sizeof(float) * 3);
					}
					else if (attribute->type == cgltf_attribute_type_normal && normals == true) {
						normalData = bufferBase + attribute->data->buffer_view->offset;
						fillBuffer(inputStride, normalData, outputStride, vertexData + (sizeof(float) * 3), numVertices, sizeof(float) * 3);
					}
					else if (attribute->type == cgltf_attribute_type_texcoord && texCoords == true) {
						texcoordData = bufferBase + attribute->data->buffer_view->offset;
						fillBuffer(inputStride, texcoordData, outputStride, vertexData + (sizeof(float) * 6), numVertices, sizeof(float) * 2);
					}
				}
			}
		}
	}
	uint32_t getIndicesCount()
	{
		return indicesCount;
	}

	uint64_t getIndexSize()
	{
		return indexDataSize;
	}

	void* getIndexData()
	{
		return indexData;
	}

	uint64_t getVertexSize()
	{
		return vertexDataSize;
	}

	uint8_t* getVertexData()
	{
		return vertexData;
	}

	void destroyModel()
	{
		cgltf_free(data);
	}
};