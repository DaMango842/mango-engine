// MangoEngine.hpp
#pragma once
#include <vector>

#include <Utils/MangoPtr_SDL.h>
#include <Renderer/SDLRenderer.hpp>
#include <Scene/GameObject.hpp>
#include <Scene/LevelLoader.hpp>
#include <Utils/FPSCounter.hpp>

namespace mango_engine {
    class Application {
    public:
        Application() {
            m_window = make_sdl_window("Mango Engine", 800, 600, SDL_WINDOW_RESIZABLE);
            m_renderer.init(m_window.get());

            InitScene();
        }

        ~Application() {
            m_renderer.shutdown();
        }

        void InitScene() {
            m_gameObjects.clear();

            auto playerTex = Resource::Texture::LoadFromFile(m_renderer.GetRaw(), "assets/images/testChar.png");

            if (!Scene::LevelLoader::LoadLevel("assets/level.json", m_gameObjects, playerTex)) {
                SDL_Log("Failed to load level!");
            }
        }

        void Run() {
            SDL_Event event{};
            m_Running = true;
            Uint64 lastTime = SDL_GetTicks();

            while (m_Running) {
                Uint64 currentTime = SDL_GetTicks();
                float deltaTime = (currentTime - lastTime) / 1000.0f;
                lastTime = currentTime;

                ProcessInput(&event);

                m_fpsCounter.Update();

                Update(deltaTime);
                Render();
            }
        }
    private:
        void ProcessInput(SDL_Event* event) {
            while (SDL_PollEvent(event)) {
                if (event->type == SDL_EVENT_QUIT) m_Running = false;
                if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) m_Running = false;
            }
        }

        void Update(float deltaTime) {
            // 游戏逻辑更新
            const bool* keyStates = SDL_GetKeyboardState(nullptr);
            for (auto& obj : m_gameObjects) {
                obj.Update(deltaTime, keyStates);
            }
            
        }

        void Render() {
            m_renderer.beginFrame();
            m_renderer.clear({ 0.1f, 0.1f, 0.1f, 1.0f });

            // 绘制内容
            for (auto& obj : m_gameObjects) {
                obj.Draw(m_renderer);
            }

            std::string fontPath = "C:\\Windows\\Fonts\\msyh.ttc"; // 替换为实际字体路径

            m_renderer.drawText(m_fpsCounter.GetDisplayString(), { 10.0f, 10.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, fontPath, 24);

            m_renderer.endFrame();
        }
    
    private:
        decltype(make_sdl_window("", 0, 0, 0)) m_window;
        // decltype(make_sdl_renderer(nullptr, nullptr)) m_renderer;
        Renderer::SDLRenderer m_renderer;

        Utils::FPSCounter m_fpsCounter;

        std::vector<Scene::GameObject> m_gameObjects; // 游戏对象列表

        bool m_Running{false};

    };
}