#ifdef __cplusplus
"C" {
#endif//__cplusplus

#include "shvulkan/shVulkan.h"

#include <memory.h>
#include <string.h>
#include <stdio.h>



uint8_t shFindValidationLayer(
	const char* validation_layer_name
) {
	shVkError(validation_layer_name == NULL, "invalid validation layer name memory", return 0);

	uint32_t          available_layer_count = 0;
	VkLayerProperties layer_properties[SH_MAX_STACK_VALIDATION_LAYER_COUNT] = { 0 };

	vkEnumerateInstanceLayerProperties(&available_layer_count, NULL);

	shVkError(
		available_layer_count == 0, 
		"no validation layers are installed", 
		return 0
	);
	shVkError(
		available_layer_count > SH_MAX_STACK_VALIDATION_LAYER_COUNT,
		"reached max stack validation layer count",
		return 0
	);
	
	vkEnumerateInstanceLayerProperties(&available_layer_count, layer_properties);

	for (uint32_t i = 0; i < available_layer_count; i++) {
		if (strcmp(layer_properties[i].layerName, validation_layer_name) == 0) {
            return 1;
        }
    }

	return 0;
}

const char* shTranslateVkResult(
	VkResult vk_result
) {
    switch (vk_result)
    {
    case VK_ERROR_DEVICE_LOST:
        return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_FRAGMENTATION:
        return "VK_ERROR_FRAGMENTATION";
    case VK_ERROR_FRAGMENTED_POOL:
        return "VK_ERROR_FRAGMENTED_POOL";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_INITIALIZATION_FAILED:
        return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
        return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
    case VK_ERROR_INVALID_SHADER_NV:
        return "VK_ERROR_INVALID_SHADER_NV";
    case VK_ERROR_LAYER_NOT_PRESENT:
        return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_MEMORY_MAP_FAILED:
        return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_ERROR_NOT_PERMITTED_EXT:
        return "VK_ERROR_NOT_PERMITTED_EXT";
    case VK_ERROR_OUT_OF_DATE_KHR:
        return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case VK_ERROR_SURFACE_LOST_KHR:
        return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_TOO_MANY_OBJECTS:
        return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_UNKNOWN:
        return "VK_ERROR_UNKNOWN";
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return "VK_ERROR_VALIDATION_FAILED_EXT";
    case VK_EVENT_RESET:
        return "VK_EVENT_RESET";
    case VK_EVENT_SET:
        return "VK_EVENT_SET";
    case VK_INCOMPLETE:
        return "VK_INCOMPLETE";
    case VK_NOT_READY:
        return "VK_NOT_READY";
#if 0//not supported by lavapipe
    case VK_OPERATION_DEFERRED_KHR:
        return "VK_OPERATION_DEFERRED_KHR";
    case VK_OPERATION_NOT_DEFERRED_KHR:
        return "VK_OPERATION_NOT_DEFERRED_KHR";
    case VK_PIPELINE_COMPILE_REQUIRED_EXT:
        return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
#endif//0
    case VK_SUBOPTIMAL_KHR:
        return "VK_SUBOPTIMAL_KHR";
    case VK_SUCCESS:
        return "VK_SUCCESS";
#if 0//not supported by lavapipe
    case VK_THREAD_DONE_KHR:
        return "VK_THREAD_DONE_KHR";
    case VK_THREAD_IDLE_KHR:
        return "VK_THREAD_IDLE_KHR";
#endif//0
    case VK_TIMEOUT:
        return "VK_TIMEOUT";
    default:
        return "Unhandled VkResult";
    }
}



uint8_t shCreateInstance(
	const char*    application_name, 
	const char*    engine_name, 
	const uint8_t  enable_validation_layers, 
	const uint32_t extension_count, 
	const char**   pp_extension_names,
	VkInstance*    p_instance
) {
	shVkError(p_instance         == NULL, "invalid instance memory",          return 0);
	shVkError(application_name   == NULL, "invalid application name memory",  return 0);
	shVkError(engine_name        == NULL, "invalid engine name memory",       return 0);
	
	shVkError(
		extension_count > 0 && pp_extension_names == NULL, 
		"invalid extension names memory",   
		return 0
	);

	VkApplicationInfo application_info = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO, //sType;
		NULL,                               //pNext;
		application_name,                   //pApplicationName;
		VK_MAKE_VERSION(0, 1, 0),           //applicationVersion;
		engine_name,                        //pEngineName;
		VK_MAKE_VERSION(0, 1, 0),           //engineVersion;
		SH_VULKAN_VERSION,                  //apiVersion;
	};


	VkInstanceCreateInfo instance_create_info = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	//sType;
		NULL,                                   //pNext;
		0,                                      //flags;
		&application_info,                      //pApplicationInfo;
		0,                                      //enabledLayerCount;
		NULL,                                   //ppEnabledLayerNames;
		extension_count,                        //enabledExtensionCount;
		pp_extension_names                      //ppEnabledExtensionNames;
	};

	if (enable_validation_layers) {
		const char* khronos_validation = "VK_LAYER_KHRONOS_validation";
		if (shFindValidationLayer(khronos_validation)) {
			instance_create_info.enabledLayerCount = 1;
			instance_create_info.ppEnabledLayerNames = &khronos_validation;
		}
	}
	
	shVkResultError(
		vkCreateInstance(&instance_create_info, VK_NULL_HANDLE, p_instance),
		"error creating vkinstance", return 0
	);

	return 1;
}

uint8_t shGetPhysicalDeviceQueueFamilies(
	VkPhysicalDevice         physical_device,
	VkSurfaceKHR             surface,
	uint32_t*                p_queue_family_count,
	uint32_t*                p_graphics_queue_family_count,
	uint32_t*                p_surface_queue_family_count,
	uint32_t*                p_compute_queue_family_count,
	uint32_t*                p_transfer_queue_family_count,
	uint32_t*                p_graphics_queue_family_indices,
	uint32_t*                p_surface_queue_family_indices,
	uint32_t*                p_compute_queue_family_indices,
	uint32_t*                p_transfer_queue_family_indices,
	VkQueueFamilyProperties* p_queue_families_properties
) {
	shVkError(physical_device == NULL, "invalid physical device memory",    return 0);

	uint32_t                queue_family_count = 0;
	VkQueueFamilyProperties queue_families_properties[SH_MAX_STACK_QUEUE_FAMILY_COUNT];

	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families_properties);

	uint32_t graphics_queue_family_count  = 0;
	uint32_t surface_queue_family_count   = 0;
	uint32_t compute_queue_family_count   = 0;
	uint32_t transfer_queue_family_count  = 0;
			 
	uint32_t graphics_queue_family_indices[SH_MAX_STACK_QUEUE_FAMILY_COUNT]  = { 0 };
	uint32_t surface_queue_family_indices [SH_MAX_STACK_QUEUE_FAMILY_COUNT]  = { 0 };
	uint32_t compute_queue_family_indices [SH_MAX_STACK_QUEUE_FAMILY_COUNT]  = { 0 };
	uint32_t transfer_queue_family_indices [SH_MAX_STACK_QUEUE_FAMILY_COUNT] = { 0 };
	
	for (uint32_t queue_family_idx = 0; queue_family_idx < queue_family_count; queue_family_idx++) {
			
		if (surface != NULL) {
			uint32_t surface_support = 0;
			vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_idx, surface, &surface_support);
			if (surface_support) {
				surface_queue_family_indices[surface_queue_family_count] = queue_family_idx;
				surface_queue_family_count++;
			}
		}
		
		if (queue_families_properties[queue_family_idx].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphics_queue_family_indices[graphics_queue_family_count] = queue_family_idx;
			graphics_queue_family_count++;
		}

		if (queue_families_properties[queue_family_idx].queueFlags & VK_QUEUE_COMPUTE_BIT) {
			compute_queue_family_indices[compute_queue_family_count] = queue_family_idx;
			compute_queue_family_count++;
		}

		if (queue_families_properties[queue_family_idx].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			transfer_queue_family_indices[transfer_queue_family_count] = queue_family_idx;
			transfer_queue_family_count++;
		}

	}
	
	if (p_queue_family_count != NULL) {
		(*p_queue_family_count) = queue_family_count;
	}
	if (p_graphics_queue_family_count != NULL) {
		(*p_graphics_queue_family_count) = graphics_queue_family_count;
	}
	if (p_surface_queue_family_count) {
		(*p_surface_queue_family_count) = surface_queue_family_count;
	}
	if (p_compute_queue_family_count != NULL) {
		(*p_compute_queue_family_count) = compute_queue_family_count;
	}
	if (p_transfer_queue_family_count != NULL) {
		(*p_transfer_queue_family_count) = transfer_queue_family_count;
	}
	if (p_graphics_queue_family_indices) {
		memcpy(p_graphics_queue_family_indices, graphics_queue_family_indices, sizeof(graphics_queue_family_indices));
	}
	if (p_surface_queue_family_indices) {
		memcpy(p_surface_queue_family_indices, surface_queue_family_indices, sizeof(surface_queue_family_indices));
	}
	if (p_compute_queue_family_indices) {
		memcpy(p_compute_queue_family_indices, compute_queue_family_indices, sizeof(compute_queue_family_indices));
	}
	if (p_transfer_queue_family_indices) {
		memcpy(p_transfer_queue_family_indices, transfer_queue_family_indices, sizeof(transfer_queue_family_indices));
	}
	if (p_queue_families_properties) {
		memcpy(p_queue_families_properties, queue_families_properties, sizeof(queue_families_properties));
	}

	return 1;
}

uint8_t shGetQueueFamilySurfaceSupport(
	VkPhysicalDevice physical_device, 
	uint32_t         queue_family_index, 
	VkSurfaceKHR     surface,
	uint8_t*         p_support
) {
	shVkError(physical_device == NULL, "invalid physical device memory", return 0);
	shVkError(p_support       == NULL, "invalid support value memory",   return 0);

	VkBool32 supported = 0;
	vkGetPhysicalDeviceSurfaceSupportKHR(
		physical_device,
		queue_family_index,
		surface,
		&supported
	);

	return (uint8_t)supported;
}

uint8_t shSelectPhysicalDevice(
	VkInstance                        instance, 
	VkSurfaceKHR                      surface, 
	VkQueueFlags                      requirements,
	VkPhysicalDevice*                 p_physical_device,
	VkPhysicalDeviceProperties*       p_physical_device_properties,
	VkPhysicalDeviceFeatures*         p_physical_device_features,
	VkPhysicalDeviceMemoryProperties* p_physical_device_memory_properties
) {
	shVkError(instance                            == NULL, "invalid instance memory",                          return 0);
	shVkError(p_physical_device                   == NULL, "invalid physical device memory",                   return 0);
	shVkError(requirements                        == 0,    "invalid requirement flags",                        return 0);

	uint32_t         physical_device_count = 0;
	VkPhysicalDevice physical_devices[SH_MAX_STACK_PHYSICAL_DEVICE_COUNT] = { 0 };

	vkEnumeratePhysicalDevices(instance, &physical_device_count, NULL);
	vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices);
	
	shVkError(physical_device_count == 0, "no vulkan compatible gpu has been found", return 0);
	shVkError(physical_device_count > SH_MAX_STACK_PHYSICAL_DEVICE_COUNT, "reached max physical device count", return 0);

	uint32_t          suitable_physical_device_count = 0;
	VkPhysicalDevice  suitable_physical_devices      [SH_MAX_STACK_PHYSICAL_DEVICE_COUNT] = { 0 };

	uint32_t graphics_queue_families_count[SH_MAX_STACK_PHYSICAL_DEVICE_COUNT] = { 0 };//for multiple device
	uint32_t surface_queue_families_count [SH_MAX_STACK_PHYSICAL_DEVICE_COUNT] = { 0 };
	uint32_t compute_queue_families_count [SH_MAX_STACK_PHYSICAL_DEVICE_COUNT] = { 0 };
	uint32_t transfer_queue_families_count[SH_MAX_STACK_PHYSICAL_DEVICE_COUNT] = { 0 };

	for (uint32_t physical_device_idx = 0; physical_device_idx < physical_device_count; physical_device_idx++) {
		shGetPhysicalDeviceQueueFamilies(
			physical_devices[physical_device_idx], 
			surface,
			NULL,
			&graphics_queue_families_count[physical_device_idx],
			&surface_queue_families_count[physical_device_idx],
			&compute_queue_families_count[physical_device_idx],
			&transfer_queue_families_count[physical_device_idx],
			NULL, NULL, NULL, NULL, NULL
		);
	}

	for (uint32_t physical_device_idx = 0; physical_device_idx < physical_device_count; physical_device_idx++) {
		VkPhysicalDevice physical_device      = physical_devices[physical_device_idx];

		uint32_t graphics_queue_family_count  = graphics_queue_families_count[physical_device_idx];
		uint32_t surface_queue_family_count   = surface_queue_families_count [physical_device_idx];
		uint32_t compute_queue_family_count   = compute_queue_families_count [physical_device_idx];
		uint32_t transfer_queue_family_count  = transfer_queue_families_count[physical_device_idx];

		uint32_t graphics_bit = ((requirements & VK_QUEUE_GRAPHICS_BIT) == 0) ? 1 : graphics_queue_family_count;
		uint32_t surface_bit  = (surface == NULL)                             ? 1 : surface_queue_family_count ;
		uint32_t compute_bit  = ((requirements & VK_QUEUE_COMPUTE_BIT) == 0)  ? 1 : compute_queue_family_count;
		uint32_t transfer_bit = ((requirements & VK_QUEUE_TRANSFER_BIT) == 0) ? 1 : transfer_queue_family_count;
		
		if (graphics_bit && surface_bit && compute_bit && transfer_bit) {
			suitable_physical_devices[suitable_physical_device_count] = physical_device;
			suitable_physical_device_count++;
		}

	}

	shVkError(suitable_physical_device_count == 0, "no suitable gpu has been found", return 0);

	uint32_t scores[SH_MAX_STACK_PHYSICAL_DEVICE_COUNT] = { 0 };
	for (uint32_t suitable_device_idx = 0; suitable_device_idx < suitable_physical_device_count; suitable_device_idx++) {

		VkPhysicalDeviceProperties physical_device_properties = { 0 };
		vkGetPhysicalDeviceProperties(physical_devices[suitable_device_idx], &physical_device_properties);

		VkPhysicalDeviceFeatures device_features = { 0 };
		vkGetPhysicalDeviceFeatures(physical_devices[suitable_device_idx], &device_features);

		if (requirements & VK_QUEUE_COMPUTE_BIT) {
			scores[suitable_device_idx] += physical_device_properties.limits.maxMemoryAllocationCount;
			scores[suitable_device_idx] += physical_device_properties.limits.maxComputeSharedMemorySize;
			scores[suitable_device_idx] += physical_device_properties.limits.maxComputeWorkGroupInvocations;
		}
		if (requirements & VK_QUEUE_GRAPHICS_BIT) {
			scores[suitable_device_idx] += physical_device_properties.limits.maxMemoryAllocationCount;
			scores[suitable_device_idx] += physical_device_properties.limits.maxVertexInputAttributes;
			scores[suitable_device_idx] += physical_device_properties.limits.maxVertexInputBindings;
			scores[suitable_device_idx] += physical_device_properties.limits.maxVertexInputAttributeOffset;
			scores[suitable_device_idx] += physical_device_properties.limits.maxVertexInputBindingStride;
			scores[suitable_device_idx] += physical_device_properties.limits.maxVertexOutputComponents;
			scores[suitable_device_idx] += physical_device_properties.limits.maxComputeWorkGroupInvocations;
		}

	}

	for (uint32_t suitable_device_idx = 0; suitable_device_idx < suitable_physical_device_count; suitable_device_idx++) {
		if ((suitable_device_idx + 1) > suitable_physical_device_count) {
			break;
		}
		if (scores[suitable_device_idx] > scores[suitable_device_idx + 1]) {
			(*p_physical_device) = physical_devices[suitable_device_idx];
		}
		else {
			(*p_physical_device) = physical_devices[suitable_device_idx + 1];
		}
	}

	if (p_physical_device_properties != NULL) {
		vkGetPhysicalDeviceProperties(*p_physical_device, p_physical_device_properties);
	}
	if (p_physical_device_features != NULL) {
		vkGetPhysicalDeviceFeatures(*p_physical_device, p_physical_device_features);
	}
	if (p_physical_device_memory_properties != NULL) {
		vkGetPhysicalDeviceMemoryProperties(*p_physical_device, p_physical_device_memory_properties);
	}

	return 1;
}

uint8_t shGetPhysicalDeviceSurfaceCapabilities(
	VkPhysicalDevice          physical_device,
	VkSurfaceKHR              surface,
	VkSurfaceCapabilitiesKHR* p_surface_capabilities
) {
	shVkError(physical_device == NULL, "invalid physical device memory", return 0);
	shVkError(surface == NULL, "invalid surface memory", return 0);
	shVkError(p_surface_capabilities == NULL, "invalid surface capabilities memory", return 0);

	shVkResultError(
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, p_surface_capabilities),
		"failed getting physical device surface capabilities",
		return 0
	);

	return 1;
}

uint8_t shQueryForDeviceQueueInfo(
	uint32_t                 queue_family_index, 
	uint32_t                 queue_count,
	float*                   p_queue_priorities,
	uint8_t                  protected,
	VkDeviceQueueCreateInfo* p_device_queue_info
) {
	shVkError(queue_count         == 0,    "invalid queue count",              return 0);
	shVkError(p_queue_priorities  == NULL, "invalid queue priorities memory",  return 0);
	shVkError(p_device_queue_info == NULL, "invalid device queue info memory", return 0);

	VkDeviceQueueCreateFlagBits flags = protected ? VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT : 0;

	VkDeviceQueueCreateInfo device_queue_info = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, //sType;
		NULL,                                       //pNext;
		flags,                                      //flags;
		queue_family_index,                         //queueFamilyIndex;
		queue_count,                                //queueCount;
		p_queue_priorities                          //pQueuePriorities;	
	};
	
	(*p_device_queue_info) = device_queue_info;

	return 1;
}

uint8_t shSetLogicalDevice(
	VkPhysicalDevice         physical_device,
	VkDevice*                p_device, 
	uint32_t                 extension_count, 
	char**                   pp_extension_names, 
	uint32_t                 device_queue_count, 
	VkDeviceQueueCreateInfo* p_device_queue_infos
) {
	shVkError(physical_device        == NULL,                           "invalid physical device memory",      return 0);
	shVkError(p_device               == NULL,                           "invalid device memory",               return 0);
	shVkError(extension_count        > 0 && pp_extension_names == NULL, "invalid extensions names memory",     return 0);
	shVkError(device_queue_count     == 0,                              "invalid device queue count",          return 0);
	shVkError(p_device_queue_infos   == NULL,                           "invalid device queue infos memory",   return 0);

	VkDeviceCreateInfo device_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, //sType;
		NULL,                                 //pNext;
		0,                                    //flags;
		device_queue_count,                   //queueCreateInfoCount;
		p_device_queue_infos,                 //pQueueCreateInfos;
		0,                                    //enabledLayerCount;
		NULL,                                 //ppEnabledLayerNames;
		extension_count,                      //enabledExtensionCount;
		pp_extension_names,                   //ppEnabledExtensionNames;
		NULL                                  //pEnabledFeatures;
	};
	
	shVkResultError(
		vkCreateDevice(physical_device, &device_create_info, NULL, p_device),
		"error creating logical device", return 0
	);

	return 1;
}

uint8_t shGetDeviceQueues(
	VkDevice  device, 
	uint32_t  queue_count, 
	uint32_t* p_queue_family_indices,
	VkQueue*  p_queues
) {
	shVkError(device                 == NULL, "invalid device memory",               return 0);
	shVkError(p_queue_family_indices == NULL, "invalid queue family indices memory", return 0);
	shVkError(p_queues               == NULL, "invalid queues memory",               return 0);

	for (uint32_t queue_idx = 0; queue_idx < queue_count; queue_idx++) {
		vkGetDeviceQueue(
			device,
			p_queue_family_indices[queue_idx],
			queue_idx,
			&p_queues[queue_idx]
		);
	}

	return 1;
}

uint8_t shCreateSwapchain(
	VkDevice                 device, 
	VkPhysicalDevice         physical_device,
	VkSurfaceKHR             surface,
	VkFormat                 image_format,
	VkFormat*                p_image_format,
	uint32_t                 swapchain_image_count,
	VkSharingMode            image_sharing_mode,
	uint8_t                  vsync,
	VkSwapchainKHR*          p_swapchain
) {
	shVkError(device == NULL,             "invalid device memory",         return 0);
	shVkError(swapchain_image_count == 0, "invalid swapchain image count", return 0);

	uint32_t                 format_count                                              = 0;
	VkSurfaceFormatKHR       surface_formats[SH_MAX_STACK_DEVICE_SURFACE_FORMAT_COUNT] = { 0 };
	uint8_t                  format_found                                              = 0;
	VkSurfaceFormatKHR       surface_format                                            = { 0 };

	VkSurfaceCapabilitiesKHR surface_capabilities                                      = { 0 };
	VkCompositeAlphaFlagsKHR composite_alpha                                           = 0;

	shVkResultError(
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, NULL),
		"error getting surface available formats", 
		return 0
	);
	shVkError(
		format_count > SH_MAX_STACK_DEVICE_SURFACE_FORMAT_COUNT, 
		"reached max stack surface format count", 
		return 0
	);
	shVkResultError(
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, surface_formats),
		"error getting surface available formats", 
		return 0
	);

	for (uint32_t i = 0; i < format_count; i++) {
		if (surface_formats[i].format == image_format) {
			surface_format = surface_formats[i];
			format_found++;
			break;
		}
	}
	if (!format_found) {
		surface_format = surface_formats[0];
	}

	shVkResultError(
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities),
		"failed getting surface capabilities",
		return 0
	);
	
	VkCompositeAlphaFlagBitsKHR composite_alpha_flags[4] = {
	    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	    VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
	    VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
	    VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};
	for (uint32_t i = 0; i < 4; i++) {
	    if (surface_capabilities.supportedCompositeAlpha & composite_alpha_flags[i]) {
			composite_alpha = composite_alpha_flags[i];
	        break;
	    }
	}

	if (p_image_format != NULL) {
		(*p_image_format) = surface_format.format;
	}

	uint32_t         present_mode_count                                     = 0;
	VkPresentModeKHR present_modes[SH_MAX_STACK_SURFACE_PRESENT_MODE_COUNT] = { 0 };
	VkPresentModeKHR present_mode                                           = VK_PRESENT_MODE_MAX_ENUM_KHR;
	shVkError(
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, NULL),
		"present mode count exceeds max stack count",
		return 0
	);
	shVkError(
		present_mode_count > SH_MAX_STACK_SURFACE_PRESENT_MODE_COUNT,
		"reached max stack surface present mode count",
		return 0
	);
	shVkError(
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes),
		"present mode count exceeds max stack count",
		return 0
	);
	if (!vsync) {
		for (uint32_t present_mode_idx = 0; present_mode_idx < present_mode_count; present_mode_idx++) {
			if (present_modes[present_mode_idx] == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				break;
			}
		}
	}
	if (present_mode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
		present_mode = VK_PRESENT_MODE_FIFO_KHR;
	}
	

	VkSwapchainCreateInfoKHR swapchain_create_info = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  //sType;
		NULL,                                         //pNext;
		0,                                            //flags;
		surface,                                      //surface;
		swapchain_image_count,                        //minImageCount;
		surface_format.format,                        //imageFormat;
		surface_format.colorSpace,                    //imageColorSpace;
		(VkExtent2D) {
			surface_capabilities.currentExtent.width,
			surface_capabilities.currentExtent.height
		},                                            //imageExtent;
		1,                                            //imageArrayLayers;
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,          //imageUsage;
		image_sharing_mode,                           //imageSharingMode;
		0,                                            //queueFamilyIndexCount;
		NULL,                                         //pQueueFamilyIndices;
		surface_capabilities.currentTransform,        //preTransform;
		composite_alpha,                              //compositeAlpha;
		present_mode,                                 //presentMode;
		1,                                            //clipped;
		0,                                            //oldSwapchain;
	};

	shVkResultError(
		vkCreateSwapchainKHR(device, &swapchain_create_info, NULL, p_swapchain),
		"error creating swapchain", return 0
	);

	return 1;
}


uint8_t shCombineMaxSamples(
	VkPhysicalDeviceProperties physical_device_properties, 
	uint32_t                   sample_count, 
	uint8_t                    combine_color_sample, 
	uint8_t                    combine_depth_sample, 
	uint32_t*                  p_sample_count
) {
	shVkError(p_sample_count == NULL, "invalid sample count memory",        return 0);
	shVkError(sample_count   == 0, "invalid starting sample count value 0", return 0);

	//fill bits, example: decimal 8 = 0b1000 ---> 0b1111 = 15
	VkSampleCountFlags _sample_count = 0;
	for (uint8_t i = 0; i < 32; i++) {
		if (sample_count & (1 << i)) {
			_sample_count |= (1 << i);
			break;
		}
		_sample_count |= (1 << i);
	}

	if (combine_color_sample) {
		_sample_count &= physical_device_properties.limits.framebufferColorSampleCounts;
	}
	if (combine_depth_sample) {
		_sample_count &= physical_device_properties.limits.framebufferDepthSampleCounts;
	}

	if (_sample_count & VK_SAMPLE_COUNT_64_BIT) { (*p_sample_count) = VK_SAMPLE_COUNT_64_BIT; return 1; }
	if (_sample_count & VK_SAMPLE_COUNT_32_BIT) { (*p_sample_count) = VK_SAMPLE_COUNT_32_BIT; return 1; }
	if (_sample_count & VK_SAMPLE_COUNT_16_BIT) { (*p_sample_count) = VK_SAMPLE_COUNT_16_BIT; return 1; }
	if (_sample_count & VK_SAMPLE_COUNT_8_BIT)  { (*p_sample_count) = VK_SAMPLE_COUNT_8_BIT;  return 1; }
	if (_sample_count & VK_SAMPLE_COUNT_4_BIT)  { (*p_sample_count) = VK_SAMPLE_COUNT_4_BIT;  return 1; }
	if (_sample_count & VK_SAMPLE_COUNT_2_BIT)  { (*p_sample_count) = VK_SAMPLE_COUNT_2_BIT;  return 1; }
	if (_sample_count & VK_SAMPLE_COUNT_1_BIT)  { (*p_sample_count) = VK_SAMPLE_COUNT_1_BIT;  return 1; }

	return 0;
}

uint8_t shGetSwapchainImages(
	VkDevice       device,
	VkSwapchainKHR swapchain,
	uint32_t*      p_swapchain_image_count,
	VkImage*       p_swapchain_images
) {
	shVkError(swapchain               == NULL, "invalid swapchain memory",   return 0);
	shVkError(device                  == NULL, "invalid device memory",      return 0);
	shVkError(p_swapchain_image_count == NULL, "invalid image count memory", return 0);
	shVkError(p_swapchain_images      == NULL, "invalid images memory",      return 0);

	shVkResultError(
		vkGetSwapchainImagesKHR(device, swapchain, p_swapchain_image_count, NULL),
		"failed getting swapchain image count",
		return 0
	);
	shVkResultError(
		vkGetSwapchainImagesKHR(device, swapchain, p_swapchain_image_count, p_swapchain_images),
		"failed getting swapchain images",
		return 0
	);

	return 1;
}

uint8_t shCreateImageView(
	VkDevice              device, 
	VkImage               image, 
	VkImageViewType       view_type, 
	VkImageAspectFlagBits image_aspect, 
	uint32_t              mip_levels, 
	VkFormat              format, 
	VkImageView*          p_image_view
) {
	shVkError(device       == NULL, "invalid device memory",     return 0);
	shVkError(image        == NULL, "invalid image memory",      return 0);
	shVkError(mip_levels   == 0,    "invalid mip levels value",  return 0);
	shVkError(p_image_view == NULL, "invalid image view memory", return 0);

	VkImageSubresourceRange subresource_range = {
		image_aspect, //aspectMask
		0,            //baseMipLevel;
		mip_levels,   //levelCount;
		0,            //baseArrayLayer;
		1             //layerCount;
	};
	
	VkImageViewCreateInfo image_view_create_info = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,   //sType;
			NULL,                                       //pNext;
			0,                                          //flags;
			image,                                      //image;
			view_type,                                  //viewType;
			format,                                     //format;
			VK_COMPONENT_SWIZZLE_IDENTITY,              //components;
			0,                                          //subresourceRange
	};
	image_view_create_info.subresourceRange = subresource_range;

	shVkResultError(
		vkCreateImageView(device, &image_view_create_info, NULL, p_image_view),
		"error creating image view", return 0
	);

	return 1;
}

uint8_t shCreateSwapchainImageViews(
	VkDevice     device,
	VkFormat     format,
	uint32_t     swapchain_image_count,
	VkImage*     p_swapchain_images,
	VkImageView* p_swapchain_image_views
) {
	shVkError(device                  == NULL, "invalid device memory",           return 0);
	shVkError(swapchain_image_count   == 0,    "invalid swapchain image count",   return 0);
	shVkError(p_swapchain_images      == NULL, "invalid swapchain images memory", return 0);
	shVkError(p_swapchain_image_views == NULL, "invalid swapchain image views",   return 0);

	for (uint32_t image_idx = 0; image_idx < swapchain_image_count; image_idx++) {
		shCreateImageView(
			device,
			p_swapchain_images[image_idx],
			VK_IMAGE_VIEW_TYPE_2D, 
			VK_IMAGE_ASPECT_COLOR_BIT, 
			1,
			format,
			&p_swapchain_image_views[image_idx]
		);
	}

	return 1;
}

uint8_t shCreateCommandPool(
	VkDevice       device, 
	uint32_t       queue_family_index,
	VkCommandPool* p_cmd_pool
) {
	shVkError(device     == NULL, "invalid device memory",       return 0);
	shVkError(p_cmd_pool == NULL, "invalid command pool memory", return 0);

	VkCommandPoolCreateInfo cmd_pool_create_info = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,			//sType;
		NULL,												//pNext;
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,	//flags;
		queue_family_index									//queueFamilyIndex;
	};
	
	shVkResultError(
		vkCreateCommandPool(device, &cmd_pool_create_info, NULL, p_cmd_pool),
		"error creating command pool", return 0
	);

	return 1;
}

uint8_t shAllocateCommandBuffers(
	VkDevice         device, 
	VkCommandPool    cmd_pool,
	uint32_t         cmd_buffer_count,
	VkCommandBuffer* p_cmd_buffer
) {
	shVkError(device       == NULL, "invalid device memory",         return 0);
	shVkError(cmd_pool     == NULL, "invalid command pool memory",   return 0);
	shVkError(p_cmd_buffer == NULL, "invalid command buffer memory", return 0);
	
	VkCommandBufferAllocateInfo cmd_buffer_allocate_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,	//sType;
		NULL,											//pNext;
		cmd_pool,										//commandPool;
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,				//level;
		cmd_buffer_count								//commandBufferCount;
	};

	shVkResultError(
		vkAllocateCommandBuffers(device, &cmd_buffer_allocate_info, p_cmd_buffer),
		"error creating command buffer", return 0
	);

	return 1;
}


uint8_t shCreateRenderpassAttachment(
	VkFormat                 format,
	uint32_t                 sample_count,
	VkAttachmentLoadOp       load_treatment,
	VkAttachmentStoreOp      store_treatment,
	VkAttachmentLoadOp       stencil_load_treatment,
	VkAttachmentStoreOp      stencil_store_treatment,
	VkImageLayout            initial_layout,
	VkImageLayout            final_layout,
	VkAttachmentDescription* p_attachment_description
) {
	shVkError(p_attachment_description == NULL, "invalid attachment description memory", return 0);
	
	VkAttachmentDescription attachment_description = {
		0,                       //flags;
		format,                  //format;
		sample_count,            //samples;
		load_treatment,          //loadOp;
		store_treatment,         //storeOp;
		stencil_load_treatment,  //stencilLoadOp;
		stencil_store_treatment, //stencilStoreOp;
		initial_layout,          //initialLayout;
		final_layout,            //finalLayout;
	};

	(*p_attachment_description) = attachment_description;

	return 1;
}

uint8_t shCreateRenderpassAttachmentReference(
	uint32_t               attachment_idx,
	VkImageLayout          layout,
	VkAttachmentReference* p_attachment_reference
) {
	shVkError(p_attachment_reference == NULL, "invalid attachment reference memory", return 0);

	VkAttachmentReference attachment_reference = {
		attachment_idx, //attachment;
		layout          //layout;
	};

	(*p_attachment_reference) = attachment_reference;

	return 1;
}

uint8_t shCreateSubpass(
	VkPipelineBindPoint    bind_point,
	uint32_t               input_attachment_count,
	VkAttachmentReference* p_input_attachments_reference,
	uint32_t               color_attachment_count,
	VkAttachmentReference* p_color_attachments_reference,
	VkAttachmentReference* p_depth_stencil_attachment_reference,
	VkAttachmentReference* p_resolve_attachment_reference,
	uint32_t               preserve_attachment_count,
	uint32_t*              p_preserve_attachments,
	VkSubpassDescription*  p_subpass
) {
	shVkError(p_subpass == NULL, "invalid subpass memory", return 0);

	shVkError(
		input_attachment_count != 0 && p_input_attachments_reference == NULL,
		"invalid input attachment memory",
		return 0
	);
	shVkError(
		color_attachment_count != 0 && p_color_attachments_reference == NULL,
		"invalid color attachment memory",
		return 0
	);
	shVkError(
		preserve_attachment_count != 0 && p_preserve_attachments == NULL,
		"invalid color attachment memory",
		return 0
	);

	VkSubpassDescription subpass_description = {
		0,                                    //flags;
		bind_point,                           //pipelineBindPoint;
		input_attachment_count,               //inputAttachmentCount;
		p_input_attachments_reference,        //pInputAttachments;
		color_attachment_count,               //colorAttachmentCount;
		p_color_attachments_reference,        //pColorAttachments;
		p_resolve_attachment_reference,       //pResolveAttachments;
		p_depth_stencil_attachment_reference, //pDepthStencilAttachment;
		preserve_attachment_count,            //preserveAttachmentCount;
		p_preserve_attachments                //pPreserveAttachments;
	};

	if (p_subpass != NULL) {
		(*p_subpass) = subpass_description;
	}

	return 1;
}

uint8_t shCreateRenderpass(
	VkDevice                 device,
	uint32_t                 attachment_count,
	VkAttachmentDescription* p_attachments_descriptions,
	uint32_t                 subpass_count,
	VkSubpassDescription*    p_subpasses,
	VkRenderPass*            p_renderpass
) {
	shVkError(device                     == NULL,  "invalid device memory",                  return 0);
	shVkError(attachment_count           == 0,     "invalid attachment count",               return 0);
	shVkError(p_attachments_descriptions == NULL, "invalid attachments descriptions memory", return 0);
	shVkError(subpass_count              == 0,     "invalid subpass count",                  return 0);
	shVkError(p_subpasses                == NULL,  "invalid subpasses memory",               return 0);
	shVkError(p_renderpass               == NULL,  "invalid renderpass memory",              return 0);

	VkRenderPassCreateInfo renderpass_create_info = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, //sType;
		NULL,                                      //pNext;
		0,                                         //flags;
        attachment_count,                          //attachmentCount;
        p_attachments_descriptions,                //pAttachments;
        subpass_count,                             //subpassCount;
        p_subpasses,                               //pSubpasses;
        0,                                         //dependencyCount;
        NULL                                       //pDependencies;
	};

	shVkResultError(
		vkCreateRenderPass(device, &renderpass_create_info, NULL, p_renderpass),
		"failed creating renderpass",
		return 0
	);

	return 1;
}

uint8_t shCreateFramebuffer(
	VkDevice       device,
	VkRenderPass   renderpass,
	uint32_t       image_view_count,
	VkImageView*   p_image_views,
	uint32_t       x,
	uint32_t       y,
	uint32_t       z,
	VkFramebuffer* p_framebuffer
) {
	shVkError(device           == NULL, "invalid device memory",      return 0);
	shVkError(renderpass       == NULL, "invalid renderpass memory",  return 0);
	shVkError(x                == 0,    "invalid framebuffer x size", return 0);
	shVkError(y                == 0,    "invalid framebuffer y size", return 0);
	shVkError(z                == 0,    "invalid framebuffer z size", return 0);
	shVkError(p_framebuffer    == NULL, "invalid framebuffer memory", return 0);

	VkFramebufferCreateFlags flags = 0;

	if (image_view_count == 0) {
		flags = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT;
	}

	shVkError(image_view_count != 0 && p_image_views == NULL, 
		"invalid image views memory", 
		return 0
	);

	VkFramebufferCreateInfo framebuffer_create_info = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, //sType;
		NULL,                                      //pNext;
		flags,                                     //flags;
		renderpass,                                //renderPass;
		image_view_count,                          //attachmentCount;
		p_image_views,                             //pAttachments;
		x,                                         //width;
		y,                                         //height;
		z                                          //layers;
	};
	
	shVkResultError(
		vkCreateFramebuffer(device, &framebuffer_create_info, NULL, p_framebuffer),
		"failed creating framebuffer",
		return 0
	);

	return 1;
}

uint8_t shWaitDeviceIdle(
	VkDevice device
) {
	shVkError(device == NULL, "invalid device memory", return 0);

	shVkResultError(
		vkDeviceWaitIdle(device),
		"failed waiting for device idle",
		return 0
	);

	return 1;
}

uint8_t shDestroySwapchain(
	VkDevice       device, 
	VkSwapchainKHR swapchain
) {
	shVkError(device    == NULL, "invalid device memory",    return 0);
	shVkError(swapchain == NULL, "invalid swapchain memory", return 0);
	
	shVkError(
		shWaitDeviceIdle(device) == 0, 
		"failed waiting for device idle", 
		return 0
	);

	vkDestroySwapchainKHR(device, swapchain, NULL);

	return 1;
}

uint8_t shDestroyFramebuffers(
	VkDevice       device, 
	uint32_t       framebuffer_count, 
	VkFramebuffer* p_framebuffers
) {
	shVkError(device            == NULL, "invalid device memory",       return 0);
	shVkError(framebuffer_count == 0,    "invalid framebuffer count",   return 0);
	shVkError(p_framebuffers    == NULL, "invalid framebuffers memory", return 0);

	shVkError(
		shWaitDeviceIdle(device) == 0,
		"failed waiting for device idle",
		return 0
	);

	for (uint32_t framebuffer_idx = 0; framebuffer_idx < framebuffer_count; framebuffer_idx++) {
		vkDestroyFramebuffer(device, p_framebuffers[framebuffer_idx], NULL);
	}

	return 1;
}

uint8_t shDestroyImageViews(
	VkDevice     device,
	uint32_t     image_view_count,
	VkImageView* p_image_views
) {
	shVkError(device           == NULL, "invalid device memory",      return 0);
	shVkError(image_view_count == 0,    "invalid image view count",   return 0);
	shVkError(p_image_views    == NULL, "invalid image views memory", return 0);

	shVkError(
		shWaitDeviceIdle(device) == 0,
		"failed waiting for device idle",
		return 0
	);

	for (uint32_t image_view_idx = 0; image_view_idx < image_view_count; image_view_idx++) {
		vkDestroyImageView(device, p_image_views[image_view_idx], NULL);
	}

	return 1;
}

uint8_t shDestroySurface(
	VkInstance instance,
	VkSurfaceKHR surface
) {
	shVkError(instance == NULL, "invalid instance memory",  return 0);
	shVkError(surface  == NULL, "invalid surface memory",   return 0);
	
	vkDestroySurfaceKHR(instance, surface, NULL);
	
	return 1;
}

uint8_t shDestroyCommandBuffers(
	VkDevice         device,
	VkCommandPool    cmd_pool,
	uint32_t         cmd_buffer_count,
	VkCommandBuffer* p_cmd_buffers
) {
	shVkError(device           == NULL, "invalid device memory",          return 0);
	shVkError(cmd_buffer_count == 0,    "invalid command buffer count",   return 0);
	shVkError(p_cmd_buffers    == NULL, "invalid command buffers memory", return 0);
	
	shVkError(
		shWaitDeviceIdle(device) == 0,
		"failed waiting for device idle",
		return 0
	);

	vkFreeCommandBuffers(device, cmd_pool, cmd_buffer_count, p_cmd_buffers);

	return 1;
}

uint8_t shDestroyCommandPool(
	VkDevice      device,
	VkCommandPool cmd_pool
) {
	shVkError(device   == NULL, "invalid device memory",       return 0);
	shVkError(cmd_pool == NULL, "invalid command pool memory", return 0);

	shVkError(
		shWaitDeviceIdle(device) == 0,
		"failed waiting for device idle",
		return 0
	);

	vkDestroyCommandPool(device, cmd_pool, NULL);

	return 1;
}

uint8_t shDestroyRenderpass(
	VkDevice device, 
	VkRenderPass render_pass
) {
	shVkError(device      == NULL, "invalid device memory",      return 0);
	shVkError(render_pass == NULL, "invalid render pass memory", return 0);
	
	shVkError(
		shWaitDeviceIdle(device) == 0,
		"failed waiting for device idle",
		return 0
	);

	vkDestroyRenderPass(device, render_pass, NULL);

	return 1;
}

uint8_t shDestroyDevice(
	VkDevice device
) {
	shVkError(device == NULL, "invalid device memory", return 0);
	
	shVkError(
		shWaitDeviceIdle(device) == 0,
		"failed waiting for device idle",
		return 0
	);

	vkDestroyDevice(device, NULL);

	return 1;
}

uint8_t shDestroyInstance(
	VkInstance instance
) {
	shVkError(instance == NULL, "invalid instance memory", return 0);

	vkDestroyInstance(instance, NULL);

	return 1;
}

uint8_t shResetCommandBuffer(
	VkCommandBuffer cmd_buffer
) {
	shVkError(cmd_buffer == NULL, "invalid command buffer memory", return 0);

	shVkResultError(
		vkResetCommandBuffer(cmd_buffer, 0),
		"failed resetting command buffer",
		return 0
	);

	return 1;
}

uint8_t shBeginCommandBuffer(
	VkCommandBuffer cmd_buffer
) {
	shVkError(cmd_buffer == NULL, "invalid command buffer", return 0);

	VkCommandBufferBeginInfo command_buffer_begin_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		NULL,
		0,
		NULL
	};

	shVkResultError(
		vkBeginCommandBuffer(cmd_buffer, &command_buffer_begin_info),
		"failed recording command buffer",
		return 0
	);

	return 1;
}

uint8_t shEndCommandBuffer(
	VkCommandBuffer cmd_buffer
) {
	shVkError(cmd_buffer == NULL, "invalid command buffer memory", return 0);

	shVkResultError(
		vkEndCommandBuffer(cmd_buffer),
		"failed ending command buffer",
		return 0
	);

	return 1;
}

uint8_t shCmdDispatch(
	VkCommandBuffer cmd_buffer,
	uint32_t        group_count_x,
	uint32_t        group_count_y,
	uint32_t        group_count_z
) {
	shVkError(cmd_buffer    == NULL, "invalid command buffer memory", return 0);
	shVkError(group_count_x == 0,    "invalid x group count",         return 0);
	shVkError(group_count_y == 0,    "invalid y group count",         return 0);
	shVkError(group_count_z == 0,    "invalid z group count",         return 0);

	vkCmdDispatch(cmd_buffer, group_count_x, group_count_y, group_count_z);

	return 1;
}

uint8_t shQueueSubmit(
	uint32_t             cmd_buffer_count,
	VkCommandBuffer*     p_cmd_buffers,
	VkQueue              queue,
	VkFence              fence,
	uint32_t             semaphores_to_wait_for_count,
	VkSemaphore*         p_semaphores_to_wait_for,
	VkPipelineStageFlags wait_stage,
	uint32_t             signal_semaphore_count,
	VkSemaphore*         p_signal_semaphores
) {
	shVkError(p_cmd_buffers == NULL,	"invalid command buffers memory",	return 0);
	shVkError(queue == NULL,			"invalid queue",					return 0);

	VkSubmitInfo submit_info = {
			VK_STRUCTURE_TYPE_SUBMIT_INFO, //sType;
			NULL,                          //pNext;
			semaphores_to_wait_for_count,  //waitSemaphoreCount;
			p_semaphores_to_wait_for,      //pWaitSemaphores;
			&wait_stage,                   //pWaitDstStageMask;
			cmd_buffer_count,              //commandBufferCount;
			p_cmd_buffers,                 //pCommandBuffers;
			signal_semaphore_count,        //signalSemaphoreCount;
			p_signal_semaphores            //pSignalSemaphores;
	};

	shVkResultError(
		vkQueueSubmit(queue, 1, &submit_info, fence),
		"failed submitting to queue",
		return 0
	);

	return 1;
}


uint8_t shCreateFences(
	VkDevice  device,
	uint32_t  fence_count,
	uint8_t   signaled,
	VkFence*  p_fences
) {
	shVkError(device      == NULL, "invalid device memory", return 0);
	shVkError(fence_count == 0,    "invalid fence count",   return 0);
	shVkError(p_fences    == NULL, "invalid fences memory", return 0);

	VkFenceCreateFlags flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

	VkFenceCreateInfo fence_create_info = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, //sType;
		NULL,                                //pNext;
		flags                                //flags;
	};

	for (uint32_t fence_idx = 0; fence_idx < fence_count; fence_idx++) {
		shVkResultError(
			vkCreateFence(device, &fence_create_info, NULL, &p_fences[fence_idx]),
			"error creating fence", return 0
		);
	}

	return 1;
}

uint8_t shCreateSemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores
) {
	shVkError(device          == NULL, "invalid device memory",     return 0);
	shVkError(semaphore_count == 0,    "invalid semaphore count",   return 0);
	shVkError(p_semaphores    == NULL, "invalid semaphores memory", return 0);

	VkSemaphoreCreateInfo semaphore_create_info = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, //sType;
		NULL,                                    //pNext;
		0                                        //flags;
	};

	for (uint32_t semaphore_idx = 0; semaphore_idx < semaphore_count; semaphore_idx++) {
		shVkResultError(
			vkCreateSemaphore(device, &semaphore_create_info, NULL, &p_semaphores[semaphore_idx]),
			"error creating render semaphore", return 0
		);
	}

	return 1;
}

uint8_t shDestroyFences(
	VkDevice  device,
	uint32_t  fence_count,
	VkFence*  p_fences
) {
	shVkError(device      == NULL, "invalid device memory", return 0);
	shVkError(fence_count == 0,    "invalid fence count",   return 0);
	shVkError(p_fences    == NULL, "invalid fences memory", return 0);

	shVkError(
		shWaitDeviceIdle(device) == 0,
		"failed waiting for device idle",
		return 0
	);

	for (uint32_t fence_idx = 0; fence_idx < fence_count; fence_idx++) {
		vkDestroyFence(device, p_fences[fence_idx], NULL);
	}

	return 1;
}

uint8_t shDestroySemaphores(
	VkDevice      device,
	uint32_t      semaphore_count,
	VkSemaphore*  p_semaphores
) {
	shVkError(device          == NULL, "invalid device memory",     return 0);
	shVkError(semaphore_count == 0,    "invalid semaphore count",   return 0);
	shVkError(p_semaphores    == NULL, "invalid semaphores memory", return 0);

	shVkError(
		shWaitDeviceIdle(device) == 0,
		"failed waiting for device idle",
		return 0
	);

	for (uint32_t semaphore_idx = 0; semaphore_idx < semaphore_count; semaphore_idx++) {
		vkDestroySemaphore(device, p_semaphores[semaphore_idx], NULL);
	}

	return 1;
}

uint8_t shResetFences(
	VkDevice device,
	uint32_t fence_count,
	VkFence* p_fences
) {
	shVkError(device      == NULL, "invalid command buffer memory", return 0);
	shVkError(fence_count == 0,    "invalid fence count",           return 0);
	shVkError(p_fences    == NULL, "invalid fences memory",         return 0);

	shVkResultError(
		vkResetFences(device, fence_count, p_fences),
		"failed locking fences",
		return 0
	);

	return 1;
}

uint8_t shWaitForFences(
	VkDevice device,
	uint32_t fence_count,
	VkFence* p_fences,
	uint8_t  wait_for_all,
	uint64_t timeout_ns
) {
	shVkError(device      == NULL, "invalid command buffer memory", return 0);
	shVkError(fence_count == 0,    "invalid fence count",           return 0);
	shVkError(p_fences    == NULL, "invalid fences memory",         return 0);

	shVkResultError(
		vkWaitForFences(device, fence_count, p_fences, (VkBool32)wait_for_all, timeout_ns),
		"failed waiting for fences",
		return 0
	);

	return 1;
}

uint8_t shWaitForSemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores,
	uint8_t      wait_for_all,
	uint64_t     timeout_ns,
	uint64_t*    p_semaphores_values
) {
	shVkError(device              == NULL, "invalid command buffer memory",    return 0);
	shVkError(semaphore_count     == 0,    "invalid semaphore count",          return 0);
	shVkError(p_semaphores        == NULL, "invalid semaphores memory",        return 0);
	shVkError(p_semaphores_values == NULL, "invalid semaphores values memory", return 0);

	VkSemaphoreWaitInfo semaphore_wait_info = {
		VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
		NULL,
		wait_for_all ? 0 : VK_SEMAPHORE_WAIT_ANY_BIT,
		semaphore_count,
		p_semaphores,
		p_semaphores_values
	};

	shVkResultError(
		vkWaitSemaphores(device, &semaphore_wait_info, timeout_ns),
		"failed waiting for semaphores",
		return 0
	);

	return 1;
}

uint8_t shAcquireSwapchainImage(
	VkDevice       device,
	VkSwapchainKHR swapchain,
	uint64_t       timeout_ns,
	VkSemaphore    acquired_signal_semaphore,
	VkFence        acquired_signal_fence,
	uint32_t*      p_swapchain_image_index
) {
	shVkError(device                    == NULL, "invalid command buffer memory",        return 0);
	shVkError(p_swapchain_image_index   == NULL, "invalid swapchain image index memory", return 0);

	shVkError(
		acquired_signal_semaphore == NULL && acquired_signal_fence == NULL, 
		"semaphore and fence are both null", 
		return 0
	);

	shVkResultError(
		vkAcquireNextImageKHR(
			device, 
			swapchain, 
			timeout_ns, 
			acquired_signal_semaphore, 
			acquired_signal_fence, 
			p_swapchain_image_index
		),
		"failed acquiring swapchain image",
		return 0
	);

	return 1;
}

uint8_t shBeginRenderpass(
	VkCommandBuffer    graphics_cmd_buffer,
	VkRenderPass       renderpass,
	int32_t            render_offset_x,
	int32_t            render_offset_y,
	uint32_t           render_size_x,
	uint32_t           render_size_y,
	uint32_t           clear_value_count,
	VkClearValue*      p_clear_values,
	VkFramebuffer      framebuffer
) {
	shVkError(graphics_cmd_buffer == NULL, "invalid command buffer memory", return 0);
	shVkError(renderpass          == NULL, "invalid renderpass memory",     return 0);
	shVkError(framebuffer         == NULL, "invalid framebuffer memory",    return 0);
	shVkError(render_size_x       == 0,    "invalid render size x",         return 0);
	shVkError(render_size_y       == 0,    "invalid render size y",         return 0);

	shVkError(
		clear_value_count != 0 && p_clear_values == NULL,
		"invalid framebuffer attachments clear values memory",
		return 0
	);

	VkRect2D render_area = {
		{ render_offset_x, render_offset_y },
		{ render_size_x,   render_size_y   }
	};

	VkRenderPassBeginInfo renderpass_begin_info = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, //sType;
		NULL,                                     //pNext;
		renderpass,                               //renderPass;
		framebuffer,                              //framebuffer;
		render_area,                              //renderArea;
		clear_value_count,                        //clearValueCount;
		p_clear_values                            //pClearValues;
	};

	vkCmdBeginRenderPass(
		graphics_cmd_buffer, 
		&renderpass_begin_info, 
		VK_SUBPASS_CONTENTS_INLINE
	);

	return 1;
}

uint8_t shEndRenderpass(
	VkCommandBuffer graphics_cmd_buffer
) {
	shVkError(graphics_cmd_buffer == NULL, "invalid command buffer memory", return 0);

	vkCmdEndRenderPass(graphics_cmd_buffer);

	return 1;
}

uint8_t shDraw(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        vertex_count,
	uint32_t        first_vertex,
	uint32_t        instance_count,
	uint32_t        first_instance
) {
	shVkError(graphics_cmd_buffer == NULL, "invalid command buffer memory", return 0);

	vkCmdDraw(
		graphics_cmd_buffer,
		vertex_count,
		instance_count,
		first_vertex,
		first_instance
	);

	return 1;
}

uint8_t shDrawIndexed(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        index_count,
	uint32_t        instance_count,
	uint32_t        first_index,
	int32_t         vertex_offset,
	uint32_t        first_instance
) {
	shVkError(graphics_cmd_buffer == NULL, "invalid command buffer memory", return 0);

	vkCmdDrawIndexed(
		graphics_cmd_buffer, 
		index_count,
		instance_count,
		first_index,
		vertex_offset,
		first_instance
	);

	return 1;
}

uint8_t shQueuePresentSwapchainImage(
	VkQueue        present_queue,
	uint32_t       semaphores_to_wait_for_count,
	VkSemaphore*   p_semaphores_to_wait_for,
	VkSwapchainKHR swapchain,
	uint32_t       swapchain_image_idx
) {
	shVkError(present_queue == NULL, "invalid present queue memory", return 0);
	shVkError(swapchain == NULL,     "invalid swapchain memory",     return 0);

	VkResult swapchain_result = VK_SUCCESS;

	VkPresentInfoKHR present_info = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		NULL,
		semaphores_to_wait_for_count,
		p_semaphores_to_wait_for,
		1,
		&swapchain,
		&swapchain_image_idx,
		&swapchain_result
	};

	shVkResultError(
		vkQueuePresentKHR(present_queue, &present_info),
		"failed presenting swapchain image",
		return 0
	);
	shVkResultError(
		swapchain_result,
		"swapchain result failure",
		return 0
	);

	return 1;
}



uint8_t shCreateBuffer(
	VkDevice           device,
	uint32_t           size,
	VkBufferUsageFlags usage,
	VkSharingMode      sharing_mode,
	VkBuffer*          p_buffer
) {
	shVkError(device   == NULL, "invalid device memory", return 0);
	shVkError(p_buffer == NULL, "invalid arguments",     return 0);
	shVkError(size     == 0,    "invalid buffer size",   return 0);

	VkBufferCreateInfo buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, //sType;
		NULL,                                 //pNext;
		0,                                    //flags;
		(VkDeviceSize)size,                   //size;
		usage,                                //usage;
		sharing_mode,                         //sharingMode;
		0,                                    //queueFamilyIndexCount;
		NULL                                  //pQueueFamilyIndices;
	};

	shVkResultError(
		vkCreateBuffer(device, &buffer_create_info, NULL, p_buffer),
		"error creating buffer", 
		return 0
	);

	return 1;
}

uint8_t shAllocateBufferMemory(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkBuffer              buffer,
	VkMemoryPropertyFlags property_flags,
	VkDeviceMemory*       p_memory
) {
	shVkError(device			== NULL, "invalid device handle",			return 0);
	shVkError(physical_device	== NULL, "invalid physical device memory",	return 0);
	shVkError(buffer			== NULL, "invalid buffer pointer",			return 0);
	shVkError(p_memory			== NULL, "invalid device memory pointer",	return 0);

	uint32_t memory_type_index = 0;
	shGetMemoryType(device, physical_device, property_flags, &memory_type_index);

	VkMemoryRequirements memory_requirements = { 0 };
	vkGetBufferMemoryRequirements(device, buffer, &memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,	//sType;
		NULL,									//pNext;
		memory_requirements.size,				//allocationSize;
		memory_type_index						//memoryTypeIndex;
	};

	shVkResultError(
		vkAllocateMemory(device, &memory_allocate_info, NULL, p_memory),
		"error allocating memory", 
		return 0
	);

	return 1;
}

uint8_t shCopyBuffer(
	VkCommandBuffer transfer_cmd_buffer,
	VkBuffer        src_buffer,
	uint32_t        src_offset,
	uint32_t        dst_offset,
	uint32_t        size,
	VkBuffer        dst_buffer
) {
	shVkError(transfer_cmd_buffer == NULL, "invalid command buffer",     return 0);
	shVkError(src_buffer          == NULL, "invalid source buffer",      return 0);
	shVkError(size                == 0,    "invalid copy size",          return 0);
	shVkError(dst_buffer          == NULL, "invalid destination buffer", return 0);


	VkBufferCopy region = {
		src_offset,
		dst_offset,
		size
	};
	vkCmdCopyBuffer(
		transfer_cmd_buffer, 
		src_buffer, 
		dst_buffer, 
		1, 
		&region
	);

	return 1;
}

uint8_t shCopyBufferRegions(
	VkCommandBuffer transfer_cmd_buffer,
	VkBuffer        src_buffer,
	uint32_t        region_count,
	uint32_t*       p_src_offsets,
	uint32_t*       p_dst_offsets,
	uint32_t*       p_sizes,
	VkBuffer        dst_buffer
) {
	shVkError(transfer_cmd_buffer == NULL, "invalid command buffer",     return 0);
	shVkError(src_buffer          == NULL, "invalid source buffer",      return 0);
	shVkError(region_count        == 0,    "invalid region count",       return 0);
	shVkError(p_src_offsets       == NULL, "invalid src offsets memory", return 0);
	shVkError(p_dst_offsets       == NULL, "invalid dst offsets memory", return 0);
	shVkError(p_sizes             == NULL, "invalid copy sizes memory",  return 0);
	shVkError(dst_buffer          == NULL, "invalid destination buffer", return 0);

	VkBufferCopy regions[SH_MAX_STACK_BUFFER_REGION_COUNT] = { 0 };

	for (uint32_t region_idx = 0; region_idx < region_count; region_idx++) {
		shVkError(
			p_sizes[region_idx] == 0,
			"invalid copy size",
			return 0
		);
		regions[region_idx].srcOffset = p_src_offsets[region_idx];
		regions[region_idx].dstOffset = p_dst_offsets[region_idx];
		regions[region_idx].size      = p_sizes[region_idx];
	}

	vkCmdCopyBuffer(
		transfer_cmd_buffer, 
		src_buffer, 
		dst_buffer, 
		region_count, 
		regions
	);

	return 1;
}

uint8_t shBindBufferMemory(
	VkDevice       device,
	VkBuffer       buffer,
	uint32_t       offset,
	VkDeviceMemory buffer_memory
) {
	shVkError(device == NULL,			"invalid device memory", return 0);
	shVkError(buffer == NULL,			"invalid buffer handle", return 0);
	shVkError(buffer_memory == NULL,	"invalid buffer memory", return 0);
	
	shVkResultError(
		vkBindBufferMemory(device, buffer, buffer_memory, offset),
		"error binding buffer memory", return 0
	)

	return 1;
}

uint8_t shGetMemoryType(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkMemoryPropertyFlags property_flags,
	uint32_t*             p_memory_type_index
) {
	shVkError(device              == NULL, "invalid device memory",             return 0);
	shVkError(physical_device     == NULL, "invalid physical device memory",    return 0);
	shVkError(p_memory_type_index == NULL, "invalid memory type index pointer", return 0);

	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

	for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {

		if (memory_properties.memoryTypes[i].propertyFlags & (1 << i) &&
			(memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
			*p_memory_type_index = i;
			return 1;
		}

	}
	shVkError(
		0,
		"cannot find suitable memory type for the given buffer", 
		return 0
	);

	return 0;
}

uint8_t shReadMemory(
	VkDevice       device,
	VkDeviceMemory memory,
	uint32_t       offset,
	uint32_t       data_size,
	void*          p_data
) {
	shVkError(device    == NULL, "invalid device memory", return 0);
	shVkError(memory    == NULL, "invalid memory",        return 0);
	shVkError(data_size == 0,    "invalid data size",     return 0);
	shVkError(p_data == NULL,    "invalid memory buffer", return 0);

	void* data;
	shVkResultError(
		vkMapMemory(device, memory, offset, data_size, 0, &data),
		"error mapping memory", return 0
	);
	memcpy(p_data, data, (size_t)data_size);
	vkUnmapMemory(device, memory);

	return 1;
}

uint8_t shWriteMemory(
	VkDevice       device,
	VkDeviceMemory memory,
	uint32_t       offset,
	uint32_t       data_size,
	void*          p_data
) {
	shVkError(device == NULL, "invalid device memory", return 0);
	shVkError(memory == NULL, "invalid memory", return 0);
	shVkError(data_size == 0, "invalid data size", return 0);
	shVkError(p_data == NULL, "invalid memory buffer", return 0);

	void* data;
	shVkResultError(
		vkMapMemory(device, memory, offset, data_size, 0, &data),
		"error mapping memory", return 0
	);
	memcpy(data, p_data, (size_t)data_size);
	vkUnmapMemory(device, memory);

	return 1;
}

uint8_t shClearBufferMemory(
	VkDevice       device,
	VkBuffer       buffer,
	VkDeviceMemory memory
) {
	shVkError(device == NULL, "invalid device handle", return 0);
	shVkError(buffer == NULL, "invalid buffer memory", return 0);
	shVkError(memory == NULL, "invalid device memory", return 0);

	shVkResultError(
		vkDeviceWaitIdle(device),
		"failed waiting device idle",
		return 0
	);
	vkDestroyBuffer(device, buffer, NULL);
	vkFreeMemory(device, memory, NULL);

	return 1;
}

uint8_t shCreateImage(
	VkDevice              device,
	VkImageType           type,
	uint32_t              x,
	uint32_t              y,
	uint32_t              z,
	VkFormat              format,
	uint32_t              mip_levels,
	VkSampleCountFlagBits sample_count,
	VkImageUsageFlags     usage,
	VkSharingMode         sharing_mode,
	VkImage*              p_image
) {
	shVkError(device       == NULL, "invalid device memory",   return 0);
	shVkError(x            == 0,    "invalid image x size",    return 0);
	shVkError(y            == 0,    "invalid image y size",    return 0);
	shVkError(z            == 0,    "invalid image z size",    return 0);
	shVkError(mip_levels   == 0,    "invalid mip level count", return 0);
	shVkError(sample_count == 0,    "invalid sample count",    return 0);
	shVkError(p_image      == NULL, "invalid image memory",    return 0);

	VkExtent3D image_extent = {
		x, y, z
	};

	VkImageCreateInfo image_create_info = {
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,	//sType;			
		NULL,									//pNext;
		0,										//flags;
		type,									//imageType;
		format,									//format;
		image_extent,							//extent;
		mip_levels,								//mipLevels;
		1,										//arrayLayers;
		sample_count,							//samples;
		VK_IMAGE_TILING_OPTIMAL,				//tiling;
		usage,									//usage;
		VK_SHARING_MODE_EXCLUSIVE,				//sharingMode;
		0,										//queueFamilyIndexCount;
		NULL									//pQueueFamilyIndices;
	};
	shVkResultError(
		vkCreateImage(device, &image_create_info, NULL, p_image),
		"error creating image", 
		return 0
	);

	return 1;
}

uint8_t shAllocateImageMemory(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkImage               image,
	VkMemoryPropertyFlags memory_property_flags,
	VkDeviceMemory*       p_image_memory
) {
	shVkError(device          == NULL, "invalid device memory",    return 0);
	shVkError(physical_device == NULL, "invalid physical device ", return 0);
	shVkError(image           == NULL, "invalid image",            return 0);
	shVkError(p_image_memory  == NULL, "invalid image memory",     return 0);

	VkMemoryRequirements memory_requirements = { 0 };
	vkGetImageMemoryRequirements(device, image, &memory_requirements);

	uint32_t memory_type_index = 0;
	shVkError(
		shGetMemoryType(device, physical_device, memory_property_flags, &memory_type_index) == 0,
		"failed detecting memory type",
		return 0
	);

	VkMemoryAllocateInfo memory_allocate_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		NULL,
		memory_requirements.size,
		memory_type_index
	};

	shVkResultError(
		vkAllocateMemory(device, &memory_allocate_info, NULL, p_image_memory),
		"error allocating image memory",
		return 0
	);

	return 1;
}

uint8_t shBindImageMemory(
	VkDevice       device,
	VkImage        image,
	uint32_t       offset,
	VkDeviceMemory image_memory
) {
	shVkError(device       == NULL, "invalid device memory", return 0);
	shVkError(image        == NULL, "invalid image",         return 0);
	shVkError(image_memory == NULL, "invalid image memory",  return 0);

	shVkResultError(
		vkBindImageMemory(device, image, image_memory, offset),
		"error binding image memory",
		return 0
	);

	return 1;
}

uint8_t shClearImageMemory(
	VkDevice       device,
	VkImage        image,
	VkDeviceMemory image_memory
) {
	shVkError(device       == NULL, "invalid device memory", return 0);
	shVkError(image        == NULL, "invalid image",         return 0);
	shVkError(image_memory == NULL, "invalid image memory",  return 0);

	shVkResultError(
		vkDeviceWaitIdle(device),
		"failed waiting device idle",
		return 0
	);

	vkDestroyImage(device, image, NULL);
	vkFreeMemory(device, image_memory, NULL);

	return 0;
}

uint8_t shGetMemoryBudgetProperties(
	VkPhysicalDevice                           physical_device,
	VkPhysicalDeviceMemoryBudgetPropertiesEXT* p_memory_budget_properties
) {
	shVkError(physical_device            == NULL, "invalid physical device memory",          return 0);
	shVkError(p_memory_budget_properties == NULL, "invalid memory budget properties memory", return 0);


	VkPhysicalDeviceMemoryBudgetPropertiesEXT memory_budget_properties = {
		VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT,
		NULL
	};

	VkPhysicalDeviceMemoryProperties2 memory_properties_2 = {
		VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
		&memory_budget_properties,
	};
	vkGetPhysicalDeviceMemoryProperties2(physical_device, &memory_properties_2);

	uint32_t memory_budget = 0;
	uint32_t process_used_memory = 0;

	for (uint32_t i = 0; i < VK_MAX_MEMORY_HEAPS; i++) {
		memory_budget += (uint32_t)memory_budget_properties.heapBudget[i];
		process_used_memory += (uint32_t)memory_budget_properties.heapUsage[i];
	}
	
	if (p_memory_budget_properties != NULL) {
		(*p_memory_budget_properties) = memory_budget_properties;
	}

	return 1;
}

uint8_t shBindVertexBuffers(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        first_binding,
	uint32_t        binding_count,
	VkBuffer*       p_vertex_buffers,
	VkDeviceSize*   p_vertex_offsets
) {
	shVkError(graphics_cmd_buffer == NULL, "invalid command buffer memory", return 0);
	shVkError(binding_count       == 0,    "invalid binding count",         return 0);
	shVkError(p_vertex_buffers    == NULL, "invalid vertex buffers memory", return 0);
	shVkError(p_vertex_offsets    == NULL, "invalid vertex offsets memory", return 0);

	vkCmdBindVertexBuffers(graphics_cmd_buffer, first_binding, binding_count, p_vertex_buffers, p_vertex_offsets);

	return 1;
}

uint8_t shBindIndexBuffer(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        index_offset,
	VkBuffer        index_buffer
) {
	shVkError(graphics_cmd_buffer == NULL, "invalid command buffer memory", return 0);
	shVkError(index_buffer        == NULL, "invalid index buffer memory",   return 0);

	vkCmdBindIndexBuffer(graphics_cmd_buffer, index_buffer, index_offset, VK_INDEX_TYPE_UINT32);
	
	return 1;
}



uint8_t shSetVertexBinding(
	uint32_t                           binding,
	uint32_t                           size,
	VkVertexInputRate                  input_rate,
	VkVertexInputBindingDescription*   p_vertex_input_binding
) {
	shVkError(p_vertex_input_binding == NULL, "invalid vertex input binding memory", return 0);

	VkVertexInputBindingDescription vertex_input_binding = {
		binding,
		size,
		input_rate
	};

	(*p_vertex_input_binding) = vertex_input_binding;

	return 1;
}

uint8_t shSetVertexAttribute(
	uint32_t                           location,
	uint32_t                           binding,
	VkFormat                           format,
	uint32_t                           offset,
	VkVertexInputAttributeDescription* p_vertex_input_attribute
) {
	shVkError(p_vertex_input_attribute == NULL, "invalid vertex input attribute memory", return 0);
	
	VkVertexInputAttributeDescription vertex_input_attribute = {
		location,
		binding,
		format,
		offset
	};

	(*p_vertex_input_attribute) = vertex_input_attribute;

    return 1;
}

uint8_t shSetVertexInputState(
	uint32_t                              vertex_binding_count, 
	VkVertexInputBindingDescription*      p_vertex_bindings, 
	uint32_t                              vertex_attribute_count, 
	VkVertexInputAttributeDescription*    p_vertex_attributes, 
	VkPipelineVertexInputStateCreateInfo* p_vertex_input_state
) {
	VkPipelineVertexInputStateCreateInfo vertexInput = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, //sType;
		NULL,                                                      //pNext;
		0,                                                         //flags;
		vertex_binding_count,                                      //vertexBindingDescriptionCount;
		p_vertex_bindings,                                         //pVertexBindingDescriptions;
		vertex_attribute_count,                                    //vertexAttributeDescriptionCount;
		p_vertex_attributes,                                       //pVertexAttributeDescriptions;
	};

	*p_vertex_input_state = vertexInput;

    return 1;
}

uint8_t shCreateInputAssembly(
	VkPrimitiveTopology                     primitive_topology, 
	VkBool32                                primitive_restart_enable, 
	VkPipelineInputAssemblyStateCreateInfo* p_input_assembly
) {
	shVkError(p_input_assembly == NULL, "invalid input assembly state memory", return 0);

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,	//sType;
		NULL,															//pNext;
		0,																//flags;
		primitive_topology,												//topology;
		primitive_restart_enable										//primitiveRestartEnable;
	};		

	*p_input_assembly = inputAssemblyStateCreateInfo;

    return 1;
}

uint8_t shCreateRasterizer(
	VkPolygonMode                           polygon_mode, 
	VkCullModeFlagBits                      cull_mode,
	VkPipelineRasterizationStateCreateInfo* p_rasterizer
) {
	shVkError(p_rasterizer == NULL, "invalid rasterizer memory", return 0);

	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, //sType;
		NULL,                                                       //pNext;
		0,                                                          //flags;
		VK_FALSE,                                                   //depthClampEnable;
		VK_FALSE,                                                   //rasterizerDiscardEnable;
		polygon_mode,                                               //polygonMode;
		cull_mode,                                                  //cullMode
		VK_FRONT_FACE_CLOCKWISE,                                    //frontFace
		VK_FALSE,                                                   //depthBiasEnable 
		0.0f,                                                       //depthBiasConstantFactor;
		0.0f,                                                       //depthBiasClamp;
		0.0f,                                                       //depthBiasSlopeFactor; 
		1.0f                                                        //lineWidth;
	};

	*p_rasterizer = rasterizationStateCreateInfo;

    return 1;
}

uint8_t shSetMultisampleState(
	VkSampleCountFlagBits                 sample_count,
	float                                 min_sample_shading_size,
	VkPipelineMultisampleStateCreateInfo* p_multisample_state
) {
	shVkError(sample_count        == 0,    "invalid sample count",             return 0);
	shVkError(p_multisample_state == NULL, "invalid multisample state memory", return 0);

	VkBool32 sample_shading_enable = min_sample_shading_size == 0.0f ? VK_FALSE : VK_TRUE;

	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, //sType;
		NULL,                                                     //pNext;
		0,                                                        //flags;
		sample_count,                                             //rasterizationSamples;
		sample_shading_enable,                                    //sampleShadingEnable;
		min_sample_shading_size,                                  //minSampleShading;
		NULL,                                                     //pSampleMask;
		VK_FALSE,                                                 //alphaToCoverageEnable;
		VK_FALSE                                                  //alphaToOneEnable;
	};

	*p_multisample_state = multisampleStateCreateInfo;

    return 1;
}

uint8_t shSetViewport(
	uint32_t                           viewport_pos_x,
	uint32_t                           viewport_pos_y,
	uint32_t                           viewport_width, 
	uint32_t                           viewport_height, 
	VkViewport*                        p_viewport,
	uint32_t                           scissors_pos_x,
	uint32_t                           scissors_pos_y,
	uint32_t                           scissors_width,
	uint32_t                           scissors_height,
	VkRect2D*                          p_scissors, 
	VkPipelineViewportStateCreateInfo* p_viewport_state
) {
	shVkError(viewport_width   == 0,    "invalid viewport width",         return 0);
	shVkError(viewport_height  == 0,    "invalid viewport height",        return 0);
	shVkError(p_viewport       == NULL, "invalid viewport memory",        return 0);
	shVkError(scissors_width   == 0,    "invalid scissors width",         return 0);
	shVkError(scissors_height  == 0,    "invalid scissors height",        return 0);
	shVkError(p_scissors       == NULL, "invalid scissors memory",        return 0);
	shVkError(p_viewport_state == NULL, "invalid viewport state pointer", return 0);

	VkViewport viewport = {
		(float)viewport_pos_x,  //x; 
		(float)viewport_pos_y,  //y;
		(float)viewport_width,  //width;
		(float)viewport_height, //height;
		0.0f, 					//minDepth;
		1.0f					//maxDepth;
	};

	(*p_viewport) = viewport;

	VkRect2D scissors = {
		{
			scissors_pos_x, 
			scissors_pos_y
		},                   //offset
		{
			scissors_width, 
			scissors_height
		}                   //extent
	};

	(*p_scissors) = scissors;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, //sType;
		NULL,                                                  //pNext;
		0,                                                     //flags;
		1,                                                     //viewportCount;
		p_viewport,                                            //pViewports;
		1,                                                     //scissorCount;
		p_scissors                                             //pScissors;
	};

	*p_viewport_state = viewportStateCreateInfo;

	return 1;
}

uint8_t shColorBlendSettings(
	VkPipelineColorBlendAttachmentState *p_color_blend_attachment, 
	VkPipelineColorBlendStateCreateInfo* p_color_blend_state
) {
	shVkError(p_color_blend_attachment == NULL, "invalid color blend attachment state memory", return 0);
	shVkError(p_color_blend_state      == NULL, "invalid color blend state memory",            return 0);
	
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
		VK_FALSE,                //blendEnable;
		0.0f,                    //srcColorBlendFactor;
		0.0f,                    //dstColorBlendFactor;
		0.0f,                    //colorBlendOp;
		0.0f,                    //srcAlphaBlendFactor;
		0.0f,                    //dstAlphaBlendFactor;
		VK_FALSE,                //alphaBlendOp;
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT //colorWriteMask;
	};
	*p_color_blend_attachment = colorBlendAttachmentState;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, //sType
		NULL,                                                     //pNext
		0,                                                        //flags
		VK_FALSE,                                                 //logicOpEnable
		VK_LOGIC_OP_COPY,                                         //logicOp
		1,                                                        //attachmentCount
		p_color_blend_attachment,                                 //pAttachments
		{0.0f, 0.0f, 0.0f}                                        //blendConstants
	};

	*p_color_blend_state = colorBlendStateCreateInfo;

    return 1;
}

uint8_t shCreateShaderModule(
	VkDevice        device, 
	uint32_t        size,
	char*           code,
	VkShaderModule* p_shader_module
) {
	shVkError(device          == NULL, "invalid device memory",             return 0);
	shVkError(size            == 0,    "invalid shader module size",        return 0);
	shVkError(code            == NULL, "invalid shader module code memory", return 0);
	shVkError(p_shader_module == NULL, "invalid shader module memory",      return 0);

	VkShaderModuleCreateInfo shader_module_create_info = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, //sType;
		NULL,                                        //pNext;
		0,                                           //flags;
		size,                                        //codeSize;
		(uint32_t*)(code)                            //pCode;
	};

	shVkResultError(
		vkCreateShaderModule(device, &shader_module_create_info, NULL, p_shader_module),
		"error creating shader module", return 0
	);

    return 1;
}

uint8_t shCreateShaderStage(
	VkShaderModule                   shader_module, 
	VkShaderStageFlags               shader_stage_flag, 
	VkPipelineShaderStageCreateInfo* p_shader_stage
) {
	shVkError(p_shader_stage == NULL, "invalid shader stage memory", return 0);

	VkPipelineShaderStageCreateInfo shader_stage_create_info = {
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, //sType;
		NULL,                                                //pNext;
		0,                                                   //flags;
		shader_stage_flag,                                   //stage;
		shader_module,                                       //module;
		"main",                                              //pName;
		NULL,                                                //pSpecializationInfo;
	};

	(*p_shader_stage) = shader_stage_create_info;

    return 1;
}

uint8_t shSetPushConstants(
	VkShaderStageFlags   shader_stage, 
	uint32_t             offset, 
	uint32_t             size, 
	VkPushConstantRange* p_push_constant_range
) {
	shVkError(size                  == 0, "invalid push constant size",            return 0);
	shVkError(p_push_constant_range == NULL, "invalid push constant range memory", return 0);
	
	VkPushConstantRange push_constant_range = {
		shader_stage, //stageFlags;
		offset,       //offset;
		size,         //size;
	};

	(*p_push_constant_range) = push_constant_range;

	return 1;
}


uint8_t shCreateDescriptorSetLayoutBinding(
	uint32_t                      binding,
	VkDescriptorType              descriptor_type,
	uint32_t                      descriptor_count,
	VkShaderStageFlags            shader_stage,
	VkDescriptorSetLayoutBinding* p_binding
) {
	shVkError(descriptor_count == 0,    "invalid descriptor count",                     return 0);
	shVkError(p_binding        == NULL, "invalid descriptor set layout binding memory", return 0);

	VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {
		binding,          //binding;
		descriptor_type,  //descriptorType
		descriptor_count, //descriptorCount;
		shader_stage,     //stageFlags;
		NULL              //pImmutableSamplers;
	};

	*p_binding = descriptor_set_layout_binding;

	return 1;
}

uint8_t shCreateDescriptorSetLayout(
	VkDevice                      device, 
	uint32_t                      binding_count,
	VkDescriptorSetLayoutBinding* p_bindings, 
	VkDescriptorSetLayout*        p_descriptor_set_layout
) {
	shVkError(device                  == NULL,  "invalid device memory",                        return 0);
	shVkError(p_bindings              == NULL, "invalid descriptor set layout bindings memory", return 0);
	shVkError(p_descriptor_set_layout == NULL,  "invalid descriptor set layout memory",         return 0);

	VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, //sType;
		NULL,                                                //pNext;
		0,                                                   //flags;
		binding_count,                                       //bindingCount;
		p_bindings                                           //pBindings;
	};

	shVkResultError(
		vkCreateDescriptorSetLayout(device, &descriptor_set_layout_create_info, NULL, p_descriptor_set_layout),
		"error creating descriptor set layout", return 0
	);

	return 1;
}

uint8_t shCreateDescriptorPool(
	VkDevice              device,
	uint32_t              pool_size_count,
	VkDescriptorPoolSize* p_pool_sizes,
	VkDescriptorPool*     p_descriptor_pool
) {
	shVkError(device            == NULL, "invalid device memory",                return 0);
	shVkError(pool_size_count   == 0,    "invalid descriptor pool size count",   return 0);
	shVkError(p_pool_sizes      == NULL, "invalid descriptor pool sizes memory", return 0);
	shVkError(p_descriptor_pool == NULL, "invalid descriptor pool memory",       return 0);
	
	uint32_t max_sets = 0;

	for (uint32_t pool_size_idx = 0; pool_size_idx < pool_size_count; pool_size_idx++) {
		max_sets += p_pool_sizes->descriptorCount;
	}

	VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, //sType;
		NULL,                                          //pNext;
		0,                                             //flags;
		max_sets,                                      //maxSets;
		pool_size_count,                               //poolSizeCount;
		p_pool_sizes                                   //pPoolSizes;
	};

	shVkResultError(
		vkCreateDescriptorPool(device, &descriptor_pool_create_info, NULL, p_descriptor_pool),
		"error creating descriptor pool", return 0
	);

	return 1;
}

uint8_t shSetDescriptorBufferInfo(
	VkBuffer                buffer, 
	uint32_t                buffer_offset,
	uint32_t                set_size, 
	VkDescriptorBufferInfo* p_buffer_info
) {
	shVkError(buffer        == NULL, "invalid descriptor buffer memory",      return 0);
	shVkError(set_size      == 0,    "invalid descriptor set size",           return 0);
	shVkError(p_buffer_info == NULL, "invalid descriptor buffer info memory", return 0);

	VkDescriptorBufferInfo buffer_info = {
		buffer,            //buffer;
		buffer_offset,     //offset;
		set_size,          //range;
	};

	(*p_buffer_info) = buffer_info;

	return 1;
}

uint8_t shAllocateDescriptorSets(
	VkDevice                device, 
	VkDescriptorPool        descriptor_pool,
	VkDescriptorType        descriptor_type,
	uint32_t                binding,
	uint32_t                set_count,
	VkDescriptorSetLayout*  p_descriptor_set_layouts, 
	VkDescriptorSet*        p_descriptor_sets, 
	VkDescriptorBufferInfo* p_buffer_infos, 
	VkWriteDescriptorSet*   p_write_descriptor_sets
) {
	shVkError(device                   == NULL, "invalid device memory",                  return 0);
	shVkError(descriptor_pool          == NULL, "invalid descriptor pool memory",         return 0);
	shVkError(set_count                == 0,    "invalid descriptor set count",           return 0);
	shVkError(p_descriptor_set_layouts == NULL, "invalid descriptor set layouts memory",  return 0);
	shVkError(p_buffer_infos           == NULL, "invalid descriptor buffer infos memory", return 0);
	shVkError(p_descriptor_sets        == NULL, "invalid descriptor sets memory",         return 0);
	shVkError(p_write_descriptor_sets  == NULL, "invalid write descriptor sets memory",   return 0);

	VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, //sType;
		NULL,                                           //pNext;
		descriptor_pool,                                //descriptorPool;
		set_count,                                      //descriptorSetCount;
		p_descriptor_set_layouts                        //pSetLayouts;
	};
	shVkResultError(
		vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, p_descriptor_sets),
		"error allocating descriptor sets", return 0
	);

	for (uint32_t set_idx = 0; set_idx < set_count; set_idx++) {
		VkWriteDescriptorSet write_descriptor_set = {
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, //sType;
			NULL,                                   //pNext;
			p_descriptor_sets[set_idx],             //dstSet;
			binding,                                //dstBinding
			0,                                      //dstArrayElement;
			1,                                      //descriptorCount;
			descriptor_type,                        //descriptorType
			NULL,                                   //pImageInfo;
			&p_buffer_infos[set_idx],               //pBufferInfo;
			NULL									//pTexelBufferView;
		};
		p_write_descriptor_sets[set_idx] = write_descriptor_set;
	}

	return 1;
}

uint8_t shCreatePipelineLayout(
	VkDevice               device,
	uint32_t               push_constant_range_count,
	VkPushConstantRange*   p_push_constants_range,
	uint32_t               descriptor_count,
	VkDescriptorSetLayout* p_descriptor_set_layouts,
	VkPipelineLayout*      p_pipeline_layout
) {
	shVkError(device            == NULL, "invalid device memory",          return 0);
	shVkError(p_pipeline_layout == NULL, "invalid pipeline layout memory", return 0);
	
	shVkError(
		push_constant_range_count != 0 && p_push_constants_range == NULL, 
		"invalid push constants range memory", 
		return 0
	);

	shVkError(
		descriptor_count != 0 && p_descriptor_set_layouts == NULL,
		"invalid descriptor set layouts memory", 
		return 0
	);


	VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, //sType;
		NULL,                                          //pNext;
		0,                                             //flags;
		descriptor_count,                              //setLayoutCount;
		p_descriptor_set_layouts,                      //pSetLayouts;
		push_constant_range_count,                     //pushConstantRangeCount;
		p_push_constants_range                         //pPushConstantRanges;
	};

	shVkResultError(
		vkCreatePipelineLayout(device, &pipeline_layout_create_info, NULL, p_pipeline_layout),
		"error creating main pipeline layout", return 0
	);

	return 1;
}

uint8_t shSetupGraphicsPipeline(
	VkDevice      device, 
	VkRenderPass  renderpass, 
	ShVkPipeline* p_pipeline
) {
	shVkError(device      == NULL, "invalid device memory",            return 0);
	shVkError(renderpass  == NULL, "invalid renderpass memory",        return 0);
	shVkError(p_pipeline  == NULL, "invalid graphics pipeline memory", return 0);

	VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info = {
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, //sType;
		NULL,                                                       //pNext;
		0,                                                          //flags;
		VK_TRUE,                                                    //depthTestEnable;
		VK_TRUE,                                                    //depthWriteEnable;
		VK_COMPARE_OP_LESS,                                         //depthCompareOp;
		VK_FALSE,                                                   //depthBoundsTestEnable;
		VK_FALSE,                                                   //stencilTestEnable;
		0,                                                          //front;
		0,                                                          //back;
		0.0f,                                                       //minDepthBounds;
		1.0f                                                        //maxDepthBounds;
	};

	VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, //sType;
		NULL,                                            //pNext;
		0,                                               //flags;
		p_pipeline->shader_module_count,                 //stageCount;
		p_pipeline->shader_stages,                       //pStages;
		&p_pipeline->vertex_input_state_info,            //pVertexInputState;
		& p_pipeline->input_assembly,                    //pInputAssemblyState;
		NULL,                                            //pTessellationState;
		&p_pipeline->viewport_state,                     //pViewportState;
		&p_pipeline->rasterizer,                         //pRasterizationState;
		&p_pipeline->multisample_state_info,             //pMultisampleState;
		&depth_stencil_state_create_info,                //pDepthStencilState;
		&p_pipeline->color_blend_state,                  //pColorBlendState;
		NULL,                                            //pDynamicState;
		p_pipeline->pipeline_layout,                     //layout;
		renderpass,                                      //renderPass;
		0,                                               //subpass;
		0,                                               //basePipelineHandle;
		0                                                //basePipelineIndex;
	};

	shVkResultError(
		vkCreateGraphicsPipelines(
			device, 
			0, 
			1, 
			&graphics_pipeline_create_info, 
			NULL, 
			&p_pipeline->pipeline
		),
		"error creating graphics pipeline", return 0
	);

    return 1;
}

extern uint8_t shDestroyDescriptorPool(
	VkDevice         device,
	VkDescriptorPool descriptor_pool
) {
	shVkError(device          == NULL, "invalid device memory",          return 0);
	shVkError(descriptor_pool == NULL, "invalid descriptor pool memory", return 0);

	vkDestroyDescriptorPool(device, descriptor_pool, NULL);

	return 1;
}

extern uint8_t shDestroyDescriptorSetLayout(
	VkDevice device,
	VkDescriptorSetLayout descriptor_set_layout
) {
	shVkError(device                == NULL, "invalid device memory",                return 0);
	shVkError(descriptor_set_layout == NULL, "invalid descriptor set layout memory", return 0);

	vkDestroyDescriptorSetLayout(device, descriptor_set_layout, NULL);

	return 1;
}

uint8_t shDestroyShaderModule(
	VkDevice       device,
	VkShaderModule shader_module
) {
	shVkError(device        == NULL, "invalid device memory",        return 0);
	shVkError(shader_module == NULL, "invalid shader module memory", return 0);

	vkDestroyShaderModule(device, shader_module, NULL);

	return 1;
}

uint8_t shDestroyPipelineLayout(
	VkDevice         device,
	VkPipelineLayout pipeline_layout
) {
	shVkError(device          == NULL, "invalid device memory",          return 0);
	shVkError(pipeline_layout == NULL, "invalid pipeline layout memory", return 0);

	vkDestroyPipelineLayout(device, pipeline_layout, NULL);

	return 1;
}

uint8_t shDestroyPipeline(
	VkDevice   device,
	VkPipeline pipeline
) {
	shVkError(device   == NULL, "invalid device memory",   return 0);
	shVkError(pipeline == NULL, "invalid pipeline memory", return 0);

	vkDestroyPipeline(device, pipeline, NULL);

	return 1;
}

uint8_t shClearPipeline(
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	memset(p_pipeline, 0, sizeof(ShVkPipeline));

	return 1;
}

uint8_t shSetupComputePipeline(
	VkDevice      device,
	ShVkPipeline* p_pipeline
) {
	shVkError(device     == NULL, "invalid device memory",            return 0);
	shVkError(p_pipeline == NULL, "invalid compute pipeline pointer", return 0);

	VkComputePipelineCreateInfo pipeline_create_info = {
		VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		NULL,
		0,
		p_pipeline->shader_stages[0],
		p_pipeline->pipeline_layout,
	};

	shVkResultError(
		vkCreateComputePipelines(device, 0, 1, &pipeline_create_info, NULL, &p_pipeline->pipeline),
		"error creating pipeline layout", return 0
	);

    return 1;
}

uint8_t shPipelineSetVertexBinding(
	uint32_t          binding,
	uint32_t          size,
	VkVertexInputRate input_rate,
	ShVkPipeline*     p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);
	
	shVkError(
		p_pipeline->vertex_binding_count == SH_MAX_PIPELINE_VERTEX_BINDING_COUNT,
		"reached max pipeline vertex input binding count",
		return 0
	);

	shVkError(
		shSetVertexBinding(
			binding, 
			size, 
			input_rate, 
			&p_pipeline->vertex_bindings[p_pipeline->vertex_binding_count]
		) == 0,
		"failed setting vertex input binding",
		return 0
	);

	p_pipeline->vertex_binding_count++;

	return 1;
}

uint8_t shPipelineSetVertexAttribute(
	uint32_t      location,
	uint32_t      binding,
	VkFormat      format,
	uint32_t      offset,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);
	
	shVkError(
		p_pipeline->vertex_binding_count == SH_MAX_PIPELINE_VERTEX_ATTRIBUTE_COUNT,
		"reached max pipeline vertex attribute count",
		return 0
	);

	shVkError(
		shSetVertexAttribute(
			location, 
			binding, 
			format, 
			offset, 
			&p_pipeline->vertex_attributes[p_pipeline->vertex_attribute_count]
		) == 0,
		"failed setting vertex input attribute",
		return 0
	);

	p_pipeline->vertex_attribute_count++;

	return 1;
}

uint8_t shPipelineSetVertexInputState(
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shSetVertexInputState(
			p_pipeline->vertex_binding_count,
			p_pipeline->vertex_bindings,
			p_pipeline->vertex_attribute_count,
			p_pipeline->vertex_attributes,
			&p_pipeline->vertex_input_state_info
		) == 0,
		"failed setting vertex input state",
		return 0
	);

	return 1;
}

uint8_t shPipelineCreateInputAssembly(
	VkPrimitiveTopology primitive_topology,
	VkBool32            primitive_restart_enable,
	ShVkPipeline*       p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shCreateInputAssembly(
			primitive_topology,
			primitive_restart_enable,
			&p_pipeline->input_assembly
		) == 0,
		"failed creating input assembly",
		return 0
	);

	return 1;
}

uint8_t shPipelineCreateRasterizer(
	VkPolygonMode      polygon_mode,
	VkCullModeFlagBits cull_mode,
	ShVkPipeline*      p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shCreateRasterizer(
			polygon_mode, 
			cull_mode, 
			&p_pipeline->rasterizer
		) == 0,
		"failed creating pipeline rasterizer",
		return 0
	);

	return 1;
}

uint8_t shPipelineSetMultisampleState(
	VkSampleCountFlagBits sample_count,
	float                 min_sample_shading_size,
	ShVkPipeline*         p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shSetMultisampleState(
			sample_count,
			min_sample_shading_size,
			&p_pipeline->multisample_state_info
		) == 0,
		"failed setting pipeline multisample state",
		return 0
	);

	return 1;
}

uint8_t shPipelineSetViewport(
	uint32_t      viewport_pos_x,
	uint32_t      viewport_pos_y,
	uint32_t      viewport_width, 
	uint32_t      viewport_height, 
	uint32_t      scissors_pos_x,
	uint32_t      scissors_pos_y,
	uint32_t      scissors_width,
	uint32_t      scissors_height,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shSetViewport(
			viewport_pos_x,
			viewport_pos_y,
			viewport_width,
			viewport_height,
			&p_pipeline->viewport,
			scissors_pos_x,
			scissors_pos_y,
			scissors_width,
			scissors_height,
			&p_pipeline->scissors,
			&p_pipeline->viewport_state
		) == 0,
		"failed setting pipeline viewport",
		return 0
	);

	return 1;
}

uint8_t shPipelineColorBlendSettings(
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shColorBlendSettings(
			&p_pipeline->color_blend_attachment,
			&p_pipeline->color_blend_state
		) == 0,
		"failed setting pipeline color blend settings",
		return 0
	);

	return 1;
}

uint8_t shPipelineCreateShaderModule(
	VkDevice        device,
	uint32_t        size,
	char*           code,
	ShVkPipeline*   p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shCreateShaderModule(
			device,
			size,
			code,
			&p_pipeline->shader_modules[p_pipeline->shader_module_count]
		) == 0,
		"failed creating shader module",
		return 0
	);

	p_pipeline->shader_module_count++;

	return 1;
}

uint8_t shPipelineCreateShaderStage(
	VkShaderStageFlags shader_stage,
	ShVkPipeline*      p_pipeline
) {

	shVkError(
		shCreateShaderStage(
			p_pipeline->shader_modules[p_pipeline->shader_stage_count],
			shader_stage,
			&p_pipeline->shader_stages[p_pipeline->shader_stage_count]
		) == 0,
		"failed creating shader stage",
		return 0
	);

	p_pipeline->shader_stage_count++;

	return 1;
}

uint8_t shPipelineSetPushConstants(
	VkShaderStageFlags shader_stage,
	uint32_t           offset,
	uint32_t           size,
	ShVkPipeline*      p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shSetPushConstants(
			shader_stage,
			offset,
			size,
			&p_pipeline->push_constant_range
		) == 0,
		"failed setting push constant info",
		return 0
	);

	return 1;
}

uint8_t shPipelineCreateDescriptorSetLayoutBinding(
	uint32_t           binding,
	VkDescriptorType   descriptor_type,
	uint32_t           descriptor_count,
	VkShaderStageFlags shader_stage,
	ShVkPipeline*      p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);
	
	shVkError(
		binding > SH_MAX_PIPELINE_DESCRIPTOR_SET_LAYOUT_BINDING_COUNT,
		"invalid descriptor set layout binding value",
		return 0
	);

	shCreateDescriptorSetLayoutBinding(
		binding,
		descriptor_type,
		descriptor_count,
		shader_stage,
		&p_pipeline->descriptor_set_layout_bindings[binding]
	);

	p_pipeline->set_layout_binding_count++;

	return 1;
}

uint8_t shPipelineCreateDescriptorSetLayout(
	VkDevice           device, 
	uint32_t           first_binding,
	uint32_t           binding_count,
	uint32_t           set_layout_idx,//set_idx
	ShVkPipeline*      p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		(first_binding + binding_count) > p_pipeline->set_layout_binding_count,
		"invalid descriptor set layout binding range", 
		return 0
	);

	shVkError(
		shCreateDescriptorSetLayout(
			device,
			binding_count,
			&p_pipeline->descriptor_set_layout_bindings[first_binding],
			&p_pipeline->descriptor_set_layouts[set_layout_idx]
		) == 0,
		"failed creating descriptor set layout",
		return 0
	);

	return 1;
}

uint8_t shPipelineCopyDescriptorSetLayout(
	uint32_t      src_set_layout_idx,
	uint32_t      first_dst_set_layout_idx,
	uint32_t      dst_set_layout_count,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		(first_dst_set_layout_idx + dst_set_layout_count) > SH_MAX_PIPELINE_DESCRIPTOR_SET_COUNT,
		"invalid descriptor set layouts range",
		return 0
	);

	for (
			uint32_t set_layout_idx = first_dst_set_layout_idx;
			set_layout_idx < (first_dst_set_layout_idx + dst_set_layout_count);
			set_layout_idx++
		) {

		p_pipeline->descriptor_set_layouts[set_layout_idx] = p_pipeline->descriptor_set_layouts[src_set_layout_idx];
		p_pipeline->descriptor_set_layout_bindings[set_layout_idx] = p_pipeline->descriptor_set_layout_bindings[src_set_layout_idx];
	}

	return 1;
}

uint8_t shPipelineSetDescriptorSetBufferInfos(
	uint32_t      first_set,
	uint32_t      set_count,
	VkBuffer      buffer,
	uint32_t      buffer_offset,
	uint32_t      set_size,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		(first_set + set_count) > SH_MAX_PIPELINE_DESCRIPTOR_SET_COUNT,
		"reached max pipeline descriptor buffer info count",
		return 0
	);

	for (uint32_t set_idx = first_set; set_idx < (first_set + set_count); set_idx++) {
		shVkError(
			shSetDescriptorBufferInfo(
				buffer,
				buffer_offset,
				set_size,
				&p_pipeline->descriptor_buffer_infos[set_idx]
			) == 0,
			"failed setting descriptor buffer info",
			return 0
		);
	}

	return 1;
}

uint8_t shPipelineCreateDescriptorPool(
	VkDevice              device,
	uint32_t              pool_idx,
	VkDescriptorType      descriptor_type,
	uint32_t              set_count,
	ShVkPipeline*         p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory",       return 0);
	shVkError(set_count  == 0,    "invvalid descriptor set count", return 0);

	VkDescriptorPoolSize pool_size = {
		descriptor_type,
		set_count
	};

	shVkError(
		shCreateDescriptorPool(
			device,
			1,
			&pool_size,
			&p_pipeline->descriptor_pools[pool_idx]\
		) == 0,
		"failed creating descriptor pool",
		return 0
	);

	p_pipeline->descriptor_pool_count++;

	return 1;
}

uint8_t shPipelineAllocateDescriptorSets(
	VkDevice         device, 
	uint32_t         pool_idx,
	uint32_t         binding,
	VkDescriptorType descriptor_type, 
	uint32_t         first_set,
	uint32_t         set_count,
	ShVkPipeline*    p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		pool_idx >= p_pipeline->descriptor_pool_count,
		"invalid descriptor pool index",
		return 0
	);

	shVkError(
		(first_set + set_count) > SH_MAX_PIPELINE_DESCRIPTOR_SET_COUNT,
		"invalid descriptor set range",
		return 0
	);

	shVkError(
		shAllocateDescriptorSets(
			device,
			p_pipeline->descriptor_pools[pool_idx],
			descriptor_type,
			binding,
			set_count,
			&p_pipeline->descriptor_set_layouts[first_set],
			&p_pipeline->descriptor_sets[first_set],
			&p_pipeline->descriptor_buffer_infos[first_set],
			&p_pipeline->write_descriptor_sets[first_set]
		) == 0,
		"failed allocating descriptor set",
		return 0
	);

	return 1;
}

uint8_t shPipelineCreateLayout(
	VkDevice      device,
	uint32_t      set_count,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shCreatePipelineLayout(
			device,
			p_pipeline->push_constant_range.size != 0,
			&p_pipeline->push_constant_range,
			set_count,
			p_pipeline->descriptor_set_layouts,
			&p_pipeline->pipeline_layout
		) == 0,
		"failed creating pipeline layout",
		return 0
	);

	p_pipeline->set_count = set_count;

	return 1;
}

uint8_t shPipelinePushConstants(
	VkCommandBuffer cmd_buffer,
	void*           p_data,
	ShVkPipeline*   p_pipeline
) {
	shVkError(cmd_buffer == NULL, "invalid command buffer memory",     return 0);
	shVkError(p_data     == NULL, "invalid push constant data memory", return 0);
	shVkError(p_pipeline == NULL, "invalid pipeline memory",           return 0);

	vkCmdPushConstants(
		cmd_buffer,
		p_pipeline->pipeline_layout,
		p_pipeline->push_constant_range.stageFlags,
		p_pipeline->push_constant_range.offset,
		p_pipeline->push_constant_range.size,
		p_data
	);

	return 1;
}

uint8_t shBindPipeline(
	VkCommandBuffer     cmd_buffer,
	VkPipelineBindPoint bind_point,
	ShVkPipeline*       p_pipeline
) {
	shVkError(cmd_buffer == NULL, "invalid command buffer memory", return 0);
	shVkError(p_pipeline == NULL, "invalid pipeline memory",       return 0);

	vkCmdBindPipeline(cmd_buffer, bind_point, p_pipeline->pipeline);

	return 1;
}

uint8_t shPipelineUpdateDescriptorSets(
	VkDevice      device,
	uint32_t      first_set, 
	uint32_t      set_count,
	ShVkPipeline* p_pipeline
) {
	shVkError(device     == NULL, "invalid device memory",   return 0);
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		(first_set + set_count) > SH_MAX_PIPELINE_DESCRIPTOR_SET_COUNT,
		"invalid descriptors range",
		return 0
	);

	vkUpdateDescriptorSets(
		device,
		set_count,
		&p_pipeline->write_descriptor_sets[first_set],
		0,
		NULL
	);

	return 1;
}

uint8_t shPipelineBindDescriptorSets(
	VkCommandBuffer     cmd_buffer,
	uint32_t            first_set,
	uint32_t            set_count,
	VkPipelineBindPoint bind_point,
	uint32_t            dynamic_descriptors_count,
	uint32_t*           p_dynamic_offsets,
	ShVkPipeline*       p_pipeline
) {
	shVkError(cmd_buffer == NULL, "invalid command buffer memory", return 0);
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		(first_set + set_count) > p_pipeline->set_count,
		"invalid descriptors range",
		return 0
	);

	shVkError(
		dynamic_descriptors_count != 0 && p_dynamic_offsets == NULL,
		"invalid dynamic offsets memory",
		return 0
	);

	vkCmdBindDescriptorSets(
		cmd_buffer,
		bind_point,
		p_pipeline->pipeline_layout,
		0,
		set_count,
		&p_pipeline->descriptor_sets[first_set],
		dynamic_descriptors_count,
		p_dynamic_offsets
	);

	return 1;
}

uint8_t shPipelineDestroyShaderModules(
	VkDevice      device,
	uint32_t      first_module,
	uint32_t      module_count,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		first_module + module_count > p_pipeline->shader_module_count,
		"invalid shader module range",
		return 0
	);

	for (uint32_t module_idx = first_module; module_idx < (first_module + module_count); module_idx++) {
		shVkError(
			shDestroyShaderModule(
				device,
				p_pipeline->shader_modules[module_idx]
			) == 0,
			"failed destroying shader moudule",
			return 0
		);
	}

	p_pipeline->shader_module_count -= module_count;
	
	return 1;
}

uint8_t shPipelineDestroyDescriptorSetLayouts(
	VkDevice      device,
	uint32_t      first_set_layout,
	uint32_t      set_layout_count,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		first_set_layout + set_layout_count > p_pipeline->set_count,
		"invalid descriptor set layout range",
		return 0
	);

	for (uint32_t set_layout_idx = first_set_layout; set_layout_idx < (first_set_layout + set_layout_count); set_layout_idx++) {
		shVkError(
			shDestroyDescriptorSetLayout(
				device,
				p_pipeline->descriptor_set_layouts[set_layout_idx]
			) == 0,
			"failed destroying descriptor set layout",
			return 0
		);
	}

	return 1;
}

uint8_t shPipelineDestroyDescriptorPools(
	VkDevice      device,
	uint32_t      first_pool,
	uint32_t      pool_count,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		first_pool + pool_count > p_pipeline->descriptor_pool_count,
		"invalid descriptor pool range",
		return 0
	);

	for (uint32_t pool_idx = first_pool; pool_idx < (first_pool + pool_count); pool_idx++) {
		shVkError(
			shDestroyDescriptorPool(
				device,
				p_pipeline->descriptor_pools[pool_idx]
			) == 0,
			"failed destroying descriptor pool",
			return 0
		);
	}

	p_pipeline->descriptor_pool_count -= pool_count;

	return 1;
}

uint8_t shPipelineDestroyLayout(
	VkDevice      device,
	ShVkPipeline* p_pipeline
) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	shVkError(
		shDestroyPipelineLayout(
			device, p_pipeline->pipeline_layout
		) == 0,
		"failed destroying pipeline layout",
		return 0
	);

	return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus
