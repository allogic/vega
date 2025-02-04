#ifndef VEGA_ENGINE_VULKAN_COMMAND_POOL_H
#define VEGA_ENGINE_VULKAN_COMMAND_POOL_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

extern VkCommandPool g_vulkan_command_pool;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_command_pool_alloc(void);
void vulkan_command_pool_free(void);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_COMMAND_POOL_H
