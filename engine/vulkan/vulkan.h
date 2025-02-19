#ifndef VEGA_ENGINE_VULKAN_H
#define VEGA_ENGINE_VULKAN_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/forward.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

int32_t vulkan_find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags memory_property_flags);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_H
