#pragma once

#define VK_USE_PLATFORM_WIN_32
#include <vulkan/vulkan.h>
#include "source/initWindow.h"
#include "Shaders/shaderReader.h"
#include "Models/vertexInfo.h"

#define VMA_IMPLEMENTATION
#include <VMA/vk_mem_alloc.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

VkInstance instance;
VkPhysicalDevice physDevice;
VkDevice device;
VkQueue queue;
VkSurfaceKHR surface;
VkSwapchainKHR swapchain;
VkRenderPass renPass;
VkDescriptorSetLayout descriptSetLayout;
VkPipelineLayout pipeLayout;
VkPipeline graphPipeline0;
VkPipeline graphPipeline1;
VkPipeline graphPipeline2;
VkDescriptorPool descriptPool;
VkDescriptorSet dstSet;
VkCommandPool cmdPool;
VkCommandBuffer cmdBuffer;

std::vector<VkImage> swapImages;
std::vector<VkImageView> swapImageViews;
std::vector<VkFramebuffer> frameBuffers;

VkShaderModule vsModule;
VkShaderModule fsModule;
VkShaderModule polyVSmodule;
VkShaderModule polyFSmodule;
VkShaderModule sphereVSmodule;
VkShaderModule sphereFSmodule;

VkSemaphore waitSemaphore;
VkSemaphore signalSemaphore;
VkFence fence;

VmaAllocator allocator;

VkBuffer vertexBuffer;
VmaAllocation vertexBufferAlloc;
VkBuffer indexBuffer;
VmaAllocation indexBufferAlloc;
VkBuffer vertexBuffer1;
VmaAllocation vertexBufferAlloc1;
VkBuffer indexBuffer1;
VmaAllocation indexBufferAlloc1;
VkBuffer vertexBuffer2;
VmaAllocation vertexBufferAlloc2;
VkBuffer indexBuffer2;
VmaAllocation indexBufferAlloc2;

VkImage textureImage;
VmaAllocation textureImageAlloc;
VkImageView textureImageView;
VkSampler textureSampler;
VkImage textureImage1;
VmaAllocation textureImageAlloc1;
VkImageView textureImageView1;
VkSampler textureSampler1;

VkBuffer uniformBuffer0;
VmaAllocation uniformBufferAlloc0;
VmaAllocationInfo uniformBufferAllocInfo0 = {};

VkImage depthImage;
VmaAllocation depthImageAlloc;
VkImageView depthImageView;

#ifdef NDEBUG
std::vector<const char*> validLayer{ };
#else
std::vector<const char*> validLayer{ "VK_LAYER_KHRONOS_validation" };
#endif

VkFormat rendering_format;

std::vector<const char*> ExtsIns{ "VK_KHR_surface", "VK_KHR_win32_surface" };
std::vector<const char*> ExtDev{ "VK_KHR_swapchain" };

std::vector<char> vsCode = shaderReader("Shaders/vert.spv");
std::vector<char> fsCode = shaderReader("Shaders/frag.spv");
std::vector<char> polyVSCode = shaderReader("Shaders/polyVS.spv");
std::vector<char> polyFSCode = shaderReader("Shaders/polyFS.spv");
std::vector<char> sphereVSCode = shaderReader("Shaders/sphereVS.spv");
std::vector<char> sphereFSCode = shaderReader("Shaders/sphereFS.spv");

VkDeviceSize offsets[] = { 0 };

struct uniformBufferObject0 {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 lightPos;
}ubo0;

void createShaderModeles(const std::vector<char> &code, VkShaderModule* sModule)
{
	const VkShaderModuleCreateInfo smci{ .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
	.codeSize = code.size(),
	.pCode = reinterpret_cast<const uint32_t*>(code.data()) };

	vkCreateShaderModule(device, &smci, nullptr, sModule);
}

void createAllocator()
{
	const VmaAllocatorCreateInfo allocatorCI{
	.physicalDevice = physDevice,
	.device = device,
	.instance = instance };

	vmaCreateAllocator(&allocatorCI, &allocator);
}

VkCommandBuffer startTimeCommands()
{
	const VkCommandBufferAllocateInfo cmdBufferAI{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	.pNext = nullptr,
	.commandPool = cmdPool,
	.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	.commandBufferCount = 1 };
	vkAllocateCommandBuffers(device, &cmdBufferAI, &cmdBuffer);

	const VkCommandBufferBeginInfo cmdBI{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };
	vkBeginCommandBuffer(cmdBuffer, &cmdBI);

	return cmdBuffer;
}

void endTimeCommands()
{
	vkEndCommandBuffer(cmdBuffer);

	const VkSubmitInfo SI = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
	.commandBufferCount = 1,
	.pCommandBuffers = &cmdBuffer };

	vkQueueSubmit(queue, 1, &SI, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);
}

void createBuffer(VkBuffer& buffer, VkDeviceSize bufferSize, void* data, VkBufferUsageFlags flag, VmaAllocation& bufferAlloc)
{
	VkBufferCreateInfo bufferCI{ .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	.size = bufferSize,         
	.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE };

	VmaAllocationCreateInfo bAllocCI{
	.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
	.usage = VMA_MEMORY_USAGE_AUTO };

	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingAllocation = VK_NULL_HANDLE;
	VmaAllocationInfo stagingBufferAllocInfo{};
	vmaCreateBuffer(allocator, &bufferCI, &bAllocCI, &stagingBuffer, &stagingAllocation, &stagingBufferAllocInfo);

	memcpy(stagingBufferAllocInfo.pMappedData, data, bufferCI.size);

	bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | flag;
	bAllocCI.flags = 0;

	vmaCreateBuffer(allocator, &bufferCI, &bAllocCI, &buffer, &bufferAlloc, nullptr);

	VkCommandBuffer cmdBuffer = 
	startTimeCommands();
	{
		VkBufferCopy region{ .size = bufferCI.size };
		vkCmdCopyBuffer(cmdBuffer, stagingBuffer, buffer, 1, &region);
	}
	endTimeCommands();

	vmaDestroyBuffer(allocator, stagingBuffer, stagingAllocation);
	stagingBuffer = VK_NULL_HANDLE;
	stagingAllocation = VK_NULL_HANDLE;
}

void createUnibuffer(VkBuffer& uniformBuffer, VkDeviceSize bufferSize, VmaAllocation& uniformBufferAlloc, VmaAllocationInfo& allocInfo)
{
	const VkBufferCreateInfo bufferCI{ .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	.size = bufferSize,
	.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE };

	const VmaAllocationCreateInfo allocCI{
	.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
	.usage = VMA_MEMORY_USAGE_AUTO };
	vmaCreateBuffer(allocator, &bufferCI, &allocCI, &uniformBuffer, &uniformBufferAlloc, &allocInfo);
}

void uploadTexture(const char* filename, VkImage& textureImage, VmaAllocation& imageAlloc)
{
	textureRead.load(filename);
	uint32_t imageWidth = textureRead.getTextureWidth();
	uint32_t imageHeight = textureRead.getTextureHeight();
	void* getPicture = textureRead.getPicture();
	uint32_t textureSize = textureRead.getTextureSize();

	const VkBufferCreateInfo bufferCI{ .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	.size = textureSize,
	.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE };

	VmaAllocationCreateInfo allocCI{
	.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
	.usage = VMA_MEMORY_USAGE_AUTO };

	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingAllocation = VK_NULL_HANDLE;
	VmaAllocationInfo stagingAllocInfo = {};
	vmaCreateBuffer(allocator, &bufferCI, &allocCI, &stagingBuffer, &stagingAllocation, &stagingAllocInfo);
	memcpy(stagingAllocInfo.pMappedData, getPicture, bufferCI.size);

	VkImageCreateInfo imageCI = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
	imageCI.imageType = VK_IMAGE_TYPE_2D;
	imageCI.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageCI.extent = { imageWidth, imageHeight, 1 };
	imageCI.mipLevels = 1;
	imageCI.arrayLayers = 1;
	imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCI.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	allocCI = { .flags = 0, .usage = static_cast<VmaMemoryUsage>(0) };

	vmaCreateImage(allocator, &imageCI, &allocCI, &textureImage, &imageAlloc, nullptr);

	VkCommandBuffer cmdBuffer =
	startTimeCommands();
	{
		VkImageMemoryBarrier ImageBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
		ImageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		ImageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		ImageBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		ImageBarrier.image = textureImage;
		ImageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ImageBarrier.subresourceRange.levelCount = 1;
		ImageBarrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageBarrier);

		VkBufferImageCopy region{};
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { imageWidth, imageHeight, 1 };
		vkCmdCopyBufferToImage(cmdBuffer, stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		ImageBarrier.image = textureImage;
		ImageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		ImageBarrier.dstAccessMask = VK_ACCESS_NONE;
		ImageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		ImageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_HOST_BIT | VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageBarrier);
	}
	endTimeCommands();
	textureRead.destroy();
	vmaDestroyBuffer(allocator, stagingBuffer, stagingAllocation);
	stagingBuffer = VK_NULL_HANDLE;
	stagingAllocation = VK_NULL_HANDLE;
}

VkImageView createImageViews(VkImage& image, VkFormat format, VkImageAspectFlags imageAspectFlag)
{
	const VkImageViewCreateInfo imageViewCI{ .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	.image = image,
	.viewType = VK_IMAGE_VIEW_TYPE_2D,
	.format = format,
	.subresourceRange{.aspectMask = imageAspectFlag,
	.baseMipLevel = 0,
	.levelCount = 1,
	.baseArrayLayer = 0,
	.layerCount = 1 } };

	VkImageView imageView;
	vkCreateImageView(device, &imageViewCI, nullptr, &imageView);

	return imageView;
}

void createDepthImage()
{
	const VkImageCreateInfo depthImageCI{ .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
	.imageType = VK_IMAGE_TYPE_2D,
	.format = VK_FORMAT_D16_UNORM,
	.extent = {Wwidth, Wheight, 1},
	.mipLevels = 1,
	.arrayLayers = 1,
	.samples = VK_SAMPLE_COUNT_1_BIT,
	.tiling = VK_IMAGE_TILING_OPTIMAL,
	.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED };

	const VmaAllocationCreateInfo depthAllocCI{ .usage = VMA_MEMORY_USAGE_AUTO };

	vmaCreateImage(allocator, &depthImageCI, &depthAllocCI, &depthImage, &depthImageAlloc, nullptr);
}

void createDepthImageView()
{
	depthImageView = createImageViews(depthImage, VK_FORMAT_D16_UNORM, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void createTextureImageView(VkImageView& textureImageView, VkImage& textureImage)
{
	textureImageView = createImageViews(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void createSampler(VkSampler& textureSampler)
{
	const VkSamplerCreateInfo samplerCI = { .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
	.magFilter = VK_FILTER_LINEAR,
	.minFilter = VK_FILTER_LINEAR,
	.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
	.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
	.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
	.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
	.anisotropyEnable = VK_TRUE,
	.maxAnisotropy = 1.0f,
	.compareOp = VK_COMPARE_OP_ALWAYS,
	.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK };

	vkCreateSampler(device, &samplerCI, nullptr, &textureSampler);
}

void createDepthImageLayout()
{
	VkCommandBuffer cmdBuffer =
	startTimeCommands();
	{
		const VkImageMemoryBarrier depthImageMemBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
		.image = depthImage,
		.subresourceRange{
			.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
			.levelCount = 1,
			.layerCount = 1 } };

		vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_HOST_BIT | VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &depthImageMemBarrier);
	}
	endTimeCommands();
}

void graphPipeline_create(VkShaderModule vsModule, VkShaderModule fsModule, VkGraphicsPipelineCreateInfo& pipeInfo, VkPipeline& graphPipeline, VkCullModeFlags cull_mode)
{
	const VkPipelineShaderStageCreateInfo pipeVShaderStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	.stage = VK_SHADER_STAGE_VERTEX_BIT,
	.module = vsModule,
	.pName = "main" };
	const VkPipelineShaderStageCreateInfo pipeFShaderStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
	.module = fsModule,
	.pName = "main" };
	const VkPipelineShaderStageCreateInfo pipeShaderStCIs[]{ pipeVShaderStCI, pipeFShaderStCI };;

	const VkPipelineInputAssemblyStateCreateInfo pipeInputAssemblyStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST };

	const VkViewport viewport{
	.x = 0.0f,
	.y = 0.0f,
	.width = static_cast<float>(Wwidth),
	.height = static_cast<float>(Wheight),
	.minDepth = 0.0f,
	.maxDepth = 1.0f };

	const VkRect2D scissor{
	.offset = {0, 0},
	.extent = { Wwidth, Wheight } };

	const VkPipelineViewportStateCreateInfo pipeViewportStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	.viewportCount = 1,
	.pViewports = &viewport,
	.scissorCount = 1,
	.pScissors = &scissor };

	const VkPipelineRasterizationStateCreateInfo pipeRasterStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	.polygonMode = VK_POLYGON_MODE_FILL,
	.cullMode = cull_mode,
	.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
	.lineWidth = 1.0f };

	const VkPipelineMultisampleStateCreateInfo pipeMultisamplingStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	.minSampleShading = 1.0f };

	const VkPipelineDepthStencilStateCreateInfo pipeDepthStencilStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
	.depthTestEnable = VK_TRUE,
	.depthWriteEnable = VK_TRUE,
	.depthCompareOp = VK_COMPARE_OP_LESS };

	const VkPipelineColorBlendAttachmentState pipeColorBlendAttachmentSt{
	.blendEnable = VK_FALSE,
	.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT };

	const VkPipelineColorBlendStateCreateInfo pipeColorBlendStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	.attachmentCount = 1,
	.pAttachments = &pipeColorBlendAttachmentSt };

	const VkGraphicsPipelineCreateInfo pipelineCI{ .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	.stageCount = 2,
	.pStages = pipeShaderStCIs,
	.pVertexInputState = pipeInfo.pVertexInputState,
	.pInputAssemblyState = &pipeInputAssemblyStCI,
	.pViewportState = &pipeViewportStCI,
	.pRasterizationState = &pipeRasterStCI,
	.pMultisampleState = &pipeMultisamplingStCI,
	.pDepthStencilState = &pipeDepthStencilStCI,
	.pColorBlendState = &pipeColorBlendStCI,
	.layout = pipeLayout,
	.renderPass = renPass };

	vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCI, nullptr, &graphPipeline);

	vkDestroyShaderModule(device, vsModule, nullptr);
	vkDestroyShaderModule(device, fsModule, nullptr);
}