#ifndef VEGA_APPLICATION_HPP
#define VEGA_APPLICATION_HPP

#include <string>

#include <Vega/Window.hpp>
#include <Vega/Layer.hpp>
#include <Vega/LayerStack.hpp>

namespace Vega {
  class ImGuiLayer;

  class Application {
  public:
    explicit Application(const std::string &title = "Vega",
                         unsigned int width = 1280,
                         unsigned int height = 720,
                         unsigned int major = 4,
                         unsigned int minor = 3,
                         unsigned int antialiasing = 0);

    virtual ~Application() = default;

    inline void PushLayer(Layer *layer) { mLayerStack.Push(layer); }
    inline void PopLayer(Layer *layer) { mLayerStack.Pop(layer); }

    inline Window &GetWindow() { return *mWindow; }

    void Run();

  public:
    inline static Application &GetApplication() { return *sInstance; }

  private:
    unsigned int mMaxFPS = 60;

    double mFPSLimit = 1.0 / mMaxFPS;

    Window *mWindow;

    ImGuiLayer *mImGuiLayer;

    LayerStack mLayerStack;

  private:
    static Application *sInstance;
  };

  extern Application *Make();
}

#endif