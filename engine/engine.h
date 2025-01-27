#ifndef VEGA_ENGINE_H
#define VEGA_ENGINE_H

#include <vega/core/core.h>

#include <vega/engine/scene.h>

#include <vega/engine/asset/loader.h>

#include <vega/engine/component/camera.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/entity/scene_editor.h>

#include <vega/engine/platform/window.h>

#include <vega/engine/vulkan/buffer.h>
#include <vega/engine/vulkan/command_buffer.h>
#include <vega/engine/vulkan/forward.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/image.h>
#include <vega/engine/vulkan/swap_chain.h>
#include <vega/engine/vulkan/vulkan.h>
#include <vega/engine/vulkan/renderer.h>
#include <vega/engine/vulkan/vertex.h>

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void engine_alloc(void);
void engine_free(void);

#endif // VEGA_ENGINE_H
