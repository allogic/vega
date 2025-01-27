#ifndef VEGA_ENGINE_VULKAN_SWAP_CHAIN_H
#define VEGA_ENGINE_VULKAN_SWAP_CHAIN_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/forward.h>

#include <vulkan/vulkan.h>

extern VkFormat g_vulkan_swap_chain_depth_format;

extern uint32_t g_vulkan_swap_chain_image_count;

extern vector_t g_vulkan_swap_chain_image_views;

extern image_t g_vulkan_swap_chain_depth_image;

extern VkSwapchainKHR g_vulkan_swap_chain;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_swap_chain_alloc(void);
void vulkan_swap_chain_free(void);

void vulkan_swap_chain_resize_before(void);
void vulkan_swap_chain_resize_after(void);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void vulkan_swap_chain_images_alloc(void);
void vulkan_swap_chain_image_views_alloc(void);

void vulkan_swap_chain_images_free(void);
void vulkan_swap_chain_image_views_free(void);

#endif // VEGA_ENGINE_VULKAN_SWAP_CHAIN_H
