#ifndef VEGA_HPP
#define VEGA_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <Vega/Application.hpp>

#include <Vega/ImGui/ImGuiLayer.hpp>

//#include <Vega/Component/Transform.hpp>
//#include <Vega/Component/Camera.hpp>
//#include <Vega/Component/Shader.hpp>

int main(int argc, char **argv) {
  auto app = Vega::Make();

  app->Run();

  delete app;
}

#endif