// MangoEngine.hpp
#pragma once
#include <vector>

#include <Utils/MangoPtr_SDL.h>
#include <Renderer/SDLRenderer.hpp>
#include <Scene/GameObject.hpp>

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
            // 初始化游戏对象
            m_gameObjects.emplace_back(Utils::Vector2{ 100.f, 100.f }, Utils::Vector2{ 50.f, 50.f }, Renderer::Color{ 1.0f, 0.0f, 0.0f, 1.0f }, 300.0f);
            m_gameObjects.emplace_back(Utils::Vector2{ 300.f, 100.f }, Utils::Vector2{ 50.f, 50.f }, Renderer::Color{ 0.0f, 1.0f, 0.0f, 1.0f }, 500.0f);
            m_gameObjects.emplace_back(Utils::Vector2{ 500.f, 100.f }, Utils::Vector2{ 50.f, 50.f }, Renderer::Color{ 0.0f, 0.0f, 1.0f, 1.0f }, 100.0f);
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

            m_renderer.endFrame();
        }
    
    private:
        decltype(make_sdl_window("", 0, 0, 0)) m_window;
        // decltype(make_sdl_renderer(nullptr, nullptr)) m_renderer;
        Renderer::SDLRenderer m_renderer;

        std::vector<Scene::GameObject> m_gameObjects; // 游戏对象列表

        bool m_Running{false};

        float m_RectX = 100.0f;
        float m_RectY = 100.0f;
        float m_MoveSpeed = 300.0f; // 每秒移动的像素数

    };
}