#ifndef VEGA_ENGINE_VULKAN_SWAP_CHAIN_H
#define VEGA_ENGINE_VULKAN_SWAP_CHAIN_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/forward.h>

#include <vulkan/vulkan.h>

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_swap_chain_alloc(void);
void vulkan_swap_chain_free(void);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void vulkan_swap_chain_images_alloc(void);
void vulkan_swap_chain_image_views_alloc(void);

void vulkan_swap_chain_images_free(void);
void vulkan_swap_chain_image_views_free(void);

#endif // VEGA_ENGINE_VULKAN_SWAP_CHAIN_H
