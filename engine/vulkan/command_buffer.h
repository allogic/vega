#ifndef VEGA_ENGINE_VULKAN_COMMAND_BUFFER_H
#define VEGA_ENGINE_VULKAN_COMMAND_BUFFER_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/forward.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

VkCommandBuffer vulkan_command_buffer_begin(void);
void vulkan_command_buffer_end(VkCommandBuffer command_buffer);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_COMMAND_BUFFER_H
