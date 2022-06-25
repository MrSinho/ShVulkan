#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>
#include <shvulkan/shVkCheck.h>
#include <shvulkan/shVkDescriptorStructureMap.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include <math.h>

#ifndef NDEBUG
#define VALIDATION_LAYERS_ENABLED 1
#else
#define VALIDATION_LAYERS_ENABLED 0
#endif//NDEBUG

GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title);

const char* readBinary(const char* path, uint32_t* p_size);

#ifndef alignas
#include "../../external/stdalign.in.h"
#endif//alignas

typedef struct Model {
	alignas(16) float model[4][4];
} Model;
SH_VULKAN_GENERATE_DESCRIPTOR_STRUCTURE_MAP(Model) //required only for dynamic descriptors


typedef struct Light {
	alignas(16) float position[4];
	alignas(16) float color[4];
} Light;

#define THREAD_COUNT 1

int main(void) {

	ShVkCore core = { 0 };
	const char* application_name = "shvulkan example";
	const uint32_t width = 720;
	const uint32_t height = 480;
	GLFWwindow* window = createWindow(width, height, application_name);
	{
		uint32_t instance_extension_count = 0;
		const char** pp_instance_extensions = glfwGetRequiredInstanceExtensions(&instance_extension_count);
		shCreateInstance(&core, application_name, "shvulkan engine", VALIDATION_LAYERS_ENABLED, instance_extension_count, pp_instance_extensions);
//#ifdef _WIN32
//		HWND win32_window = glfwGetWin32Window(window);
//		shCreateWindowSurface(&core, width, height, NULL, (void*)&win32_window);
//#elif defined __linux__
//		Window xlib_window = glfwGetX11Window(window);
//		shCreateWindowSurface(&core, (void*)XOpenDisplay(NULL), &xlib_window);
//		//printf("Window surface: %p", &core.surface.surface);
//#endif//WIN32
		glfwCreateWindowSurface(core.instance, window, NULL, &core.surface.surface);
		core.surface.width = width;
		core.surface.height = height;

		shSelectPhysicalDevice(&core, SH_VK_CORE_GRAPHICS);
		shSetLogicalDevice(&core);
		shInitSwapchainData(&core);
		shInitDepthData(&core);
		shCreateRenderPass(&core);
		shSetFramebuffers(&core);
		shCreateGraphicsCommandBuffers(&core, THREAD_COUNT);
		shSetSyncObjects(&core);
		shGetGraphicsQueue(&core);
	}


	VkBuffer triangle_vertex_buffer = NULL;
	VkDeviceMemory triangle_vertex_buffer_memory = NULL; 
	#define TRIANGLE_VERTEX_COUNT 24
	float triangle[TRIANGLE_VERTEX_COUNT] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};
	{
		shCreateVertexBuffer(core.device, TRIANGLE_VERTEX_COUNT * 4, &triangle_vertex_buffer);
		shAllocateVertexBufferMemory(core.device, core.physical_device, triangle_vertex_buffer, &triangle_vertex_buffer_memory);
		shBindVertexBufferMemory(core.device, triangle_vertex_buffer, 0, triangle_vertex_buffer_memory);
		shWriteVertexBufferMemory(core.device, triangle_vertex_buffer_memory, 0, TRIANGLE_VERTEX_COUNT * 4, triangle);
	}


	
	#define QUAD_VERTEX_COUNT 32
	float quad[QUAD_VERTEX_COUNT] = {
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};
	#define QUAD_INDEX_COUNT 6
	uint32_t indices[QUAD_INDEX_COUNT] = {
		0, 1, 2,
		2, 3, 0
	};
	VkBuffer quad_vertex_buffer, quad_index_buffer;
	VkDeviceMemory quad_vertex_buffer_memory, quad_index_buffer_memory;
	{
		shCreateVertexBuffer(core.device, QUAD_VERTEX_COUNT * 4, &quad_vertex_buffer);
		shCreateIndexBuffer(core.device, QUAD_INDEX_COUNT * 4, &quad_index_buffer);

		shAllocateVertexBufferMemory(core.device, core.physical_device, quad_vertex_buffer, &quad_vertex_buffer_memory);
		shAllocateIndexBufferMemory(core.device, core.physical_device, quad_index_buffer, &quad_index_buffer_memory);

		shBindVertexBufferMemory(core.device, quad_vertex_buffer, 0, quad_vertex_buffer_memory);
		shBindIndexBufferMemory(core.device, quad_index_buffer, 0, quad_index_buffer_memory);

		shWriteVertexBufferMemory(core.device, quad_vertex_buffer_memory, 0, QUAD_VERTEX_COUNT * 4, quad);
		shWriteIndexBufferMemory(core.device, quad_index_buffer_memory, 0, QUAD_INDEX_COUNT * 4, indices);
	}
	
	float projection[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	float view[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	void* push_constants_data[128 / sizeof(void*)];
	memcpy(push_constants_data, projection, 64);
	memcpy(&push_constants_data[64 / sizeof(void*)], view, 64);


	float model0_data[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	float model1_data[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	ModelDescriptorStructureMap model_map = shVkCreateModelDescriptorStructures(core.physical_device_properties, 2);
	for (uint32_t i = 0; i < model_map.structure_count; i++) {
		Model* p_model = shVkGetModelDescriptorStructure(model_map, i, 0);
		if (i == 0) {
			memcpy(p_model->model, model0_data, 64);
		}
		else {
			memcpy(p_model->model, model1_data, 64);
		}
	}
	shVkMapModelDecriptorStructures(&model_map);

	Light light = {
		{ 0.0f,  2.0f, 0.0f, 1.0f }, //light position
		{ 0.0f, 0.45f, 0.9f, 1.0f }	 //light color
	};

	ShVkPipeline* p_pipeline = shAllocateShVkPipeline();
	ShVkFixedStates fixed_states = { 0 };
	{
		shSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, 128, &p_pipeline->push_constant_range);

		shPipelineCreateDescriptorBuffer(core.device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 0, sizeof(Light), p_pipeline);
		shPipelineCreateDynamicDescriptorBuffer(core.device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 1, model_map.structure_size, 2, p_pipeline);

		shPipelineAllocateDescriptorBufferMemory(core.device, core.physical_device, 0, p_pipeline);
		shPipelineAllocateDescriptorBufferMemory(core.device, core.physical_device, 1, p_pipeline);

		shPipelineBindDescriptorBufferMemory(core.device, 0, 0, p_pipeline);
		shPipelineBindDescriptorBufferMemory(core.device, 1, 0, p_pipeline);

		shPipelineDescriptorSetLayout(core.device, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);
		shPipelineDescriptorSetLayout(core.device, 1, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, p_pipeline);

		shPipelineCreateDescriptorPool(core.device, 0, p_pipeline);
		shPipelineCreateDescriptorPool(core.device, 1, p_pipeline);

		shPipelineAllocateDescriptorSet(core.device, 0, p_pipeline);
		shPipelineAllocateDescriptorSet(core.device, 1, p_pipeline);

		uint32_t vertex_shader_size = 0;
		uint32_t fragment_shader_size = 0;
		char* vertex_code = (char*)readBinary("../examples/shaders/bin/mesh.vert.spv", &vertex_shader_size);
		char* fragment_code = (char*)readBinary("../examples/shaders/bin/mesh.frag.spv", &fragment_shader_size);
		shPipelineCreateShaderModule(core.device, vertex_shader_size, vertex_code, p_pipeline);
		shPipelineCreateShaderStage(core.device, VK_SHADER_STAGE_VERTEX_BIT, p_pipeline);

		shPipelineCreateShaderModule(core.device, fragment_shader_size, fragment_code, p_pipeline);
		shPipelineCreateShaderStage(core.device, VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);
		
		free(vertex_code);
		free(fragment_code);

		shSetVertexInputAttribute(0, SH_VEC3_SIGNED_FLOAT, 0, 12, &fixed_states);
		shSetVertexInputAttribute(1, SH_VEC3_SIGNED_FLOAT, 12, 12, &fixed_states);
		shSetVertexInputAttribute(2, SH_VEC2_SIGNED_FLOAT, 24, 8, &fixed_states);

		shFixedStatesSetVertexInputRate(VK_VERTEX_INPUT_RATE_VERTEX, 0, &fixed_states);
		shFixedStatesSetVertexInputState(&fixed_states);

		shSetFixedStates(core.device, core.surface.width, core.surface.height, SH_FIXED_STATES_POLYGON_MODE_FACE | SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, &fixed_states);
		shSetupGraphicsPipeline(core.device, core.render_pass, fixed_states, p_pipeline);
	}

	uint32_t available_vram, process_used_vram = 0;
	shGetMemoryBudgetProperties(core.physical_device, &available_vram, &process_used_vram, NULL);

	uint32_t frame_index = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		shFrameReset(&core, 0);

		shFrameBegin(&core, 0, &frame_index);

		shBindPipeline(core.p_graphics_commands[0].cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

		shPipelinePushConstants(core.p_graphics_commands[0].cmd_buffer, push_constants_data, p_pipeline);

		shPipelineUpdateDescriptorSets(core.device, p_pipeline);

		shPipelineWriteDescriptorBufferMemory(core.device, 0, &light, p_pipeline);
		shPipelineBindDescriptorSet(core.p_graphics_commands[0].cmd_buffer, 0, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);
		
		
		Model* p_model0 = shVkGetModelDescriptorStructure(model_map, 0, 1);
		shPipelineWriteDynamicDescriptorBufferMemory(core.device, 1, p_model0->model, p_pipeline);
		shPipelineBindDynamicDescriptorSet(core.p_graphics_commands[0].cmd_buffer, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);
		
		shBindVertexBuffer(core.p_graphics_commands[0].cmd_buffer, 0, &quad_vertex_buffer);
		shBindIndexBuffer(core.p_graphics_commands[0].cmd_buffer, 0, &quad_index_buffer);
		shDrawIndexed(core.p_graphics_commands[0].cmd_buffer, QUAD_INDEX_COUNT);
		
		Model* p_model1 = shVkGetModelDescriptorStructure(model_map, 1, 1);
		shPipelineWriteDynamicDescriptorBufferMemory(core.device, 1, p_model1->model, p_pipeline);
		shPipelineBindDynamicDescriptorSet(core.p_graphics_commands[0].cmd_buffer, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);
		
		triangle[9] = (float)sin(glfwGetTime());
		shWriteVertexBufferMemory(core.device, triangle_vertex_buffer_memory, 0, TRIANGLE_VERTEX_COUNT * 4, triangle);
		shBindVertexBuffer(core.p_graphics_commands[0].cmd_buffer, 0, &triangle_vertex_buffer);
		shDraw(core.p_graphics_commands[0].cmd_buffer, TRIANGLE_VERTEX_COUNT / (fixed_states.vertex_binding_description.stride / 4));

		shEndPipeline(p_pipeline);
		shFrameEnd(&core, 0, frame_index);
	}
	glfwTerminate();

	shVkReleaseModelDescriptorStructureMap(&model_map);

	shPipelineClearDescriptorBufferMemory(core.device, 0, p_pipeline);
	shPipelineClearDescriptorBufferMemory(core.device, 1, p_pipeline);
	
	shPipelineRelease(core.device, p_pipeline);

	shClearVertexBufferMemory(core.device, triangle_vertex_buffer, triangle_vertex_buffer_memory);
	shClearVertexBufferMemory(core.device, quad_vertex_buffer, quad_vertex_buffer_memory);
	shClearIndexBufferMemory(core.device, quad_index_buffer, quad_index_buffer_memory);

	shVulkanRelease(&core);

	return 0;
}



GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title) {
	shVkError(!glfwInit(), "error initializing glfw", return NULL);
	shVkError(glfwVulkanSupported() == GLFW_FALSE, "vulkan not supported by glfw", return NULL);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	return glfwCreateWindow(width, height, title, NULL, NULL);
}

#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER
#include <stdlib.h>
const char* readBinary(const char* path, uint32_t* p_size) {
	FILE* stream = fopen(path, "rb");
	if (stream == NULL) {
		return NULL;
	}
	fseek(stream, 0, SEEK_END);
	uint32_t code_size = ftell(stream);
	fseek(stream, 0, SEEK_SET);
	char* code = (char*)calloc(1, code_size);
	if (code == NULL) {
		fclose(stream);
		return NULL;
	}
	fread(code, code_size, 1, stream);
	*p_size = code_size;
	fclose(stream);
	return code;
}

#ifdef __cplusplus
}
#endif//__cplusplus