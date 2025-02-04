#ifndef VEGA_ENGINE_VULKAN_INSTANCE_H
#define VEGA_ENGINE_VULKAN_INSTANCE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

extern VkInstance g_vulkan_instance;

#ifdef VEGA_DEBUG
extern char const* g_vulkan_instance_validation_layers[1];
#endif // VEGA_DEBUG

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_instance_alloc(void);
void vulkan_instance_free(void);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_INSTANCE_H
