#pragma once
#include "InitVulkan.h"

void destroy()
{
	vkDeviceWaitIdle(device);

	vkDestroyImageView(device, depthImageView, nullptr);
	vmaDestroyImage(allocator, depthImage, depthImageAlloc);

	vkDestroyImageView(device, textureImageView, nullptr);
	vmaDestroyImage(allocator, textureImage, textureImageAlloc);
	vkDestroySampler(device, textureSampler, nullptr);
	vkDestroyImageView(device, textureImageView1, nullptr);
	vmaDestroyImage(allocator, textureImage1, textureImageAlloc1);
	vkDestroySampler(device, textureSampler1, nullptr);

	vmaDestroyBuffer(allocator, vertexBuffer2, vertexBufferAlloc2);
	vmaDestroyBuffer(allocator, indexBuffer2, indexBufferAlloc2);
	vmaDestroyBuffer(allocator, vertexBuffer1, vertexBufferAlloc1);
	vmaDestroyBuffer(allocator, indexBuffer1, indexBufferAlloc1);
	vmaDestroyBuffer(allocator, vertexBuffer, vertexBufferAlloc);
	vmaDestroyBuffer(allocator, indexBuffer, indexBufferAlloc);
	vmaDestroyBuffer(allocator, uniformBuffer0, uniformBufferAlloc0);
	vmaDestroyAllocator(allocator);

	vkDestroySemaphore(device, waitSemaphore, nullptr);
	vkDestroySemaphore(device, signalSemaphore, nullptr);
	vkDestroyFence(device, fence, nullptr);
	for (int i = 0; i < swapImageViews.size(); i++)
	vkDestroyFramebuffer(device, frameBuffers[i], nullptr);
	vkDestroyCommandPool(device, cmdPool, nullptr);
	vkDestroyDescriptorPool(device, descriptPool, nullptr);
	vkDestroyPipelineLayout(device, pipeLayout, nullptr);
	vkDestroyPipeline(device, graphPipeline2, nullptr);
	vkDestroyPipeline(device, graphPipeline1, nullptr);
	vkDestroyPipeline(device, graphPipeline0, nullptr);
	vkDestroyDescriptorSetLayout(device, descriptSetLayout, nullptr);
	vkDestroyRenderPass(device, renPass, nullptr);
	for (int i = 0; i < swapImageViews.size(); i++)
	vkDestroyImageView(device, swapImageViews[i], nullptr);
	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);

	glfwTerminate();
}
