// MangoEngine.hpp
#pragma once
#include <vector>

#include <Utils/MangoPtr_SDL.h>
#include <Renderer/SDLRenderer.hpp>
#include <Scene/GameObject.hpp>
#include <Scene/LevelLoader.hpp>
#include <Utils/FPSCounter.hpp>
#include <Scene/Camera.hpp>
#include <Input/Input.hpp>

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

                Input::Input::EndFrame();
            }
        }
    private:
        void ProcessInput(SDL_Event* event) {
            while (SDL_PollEvent(event)) {
                if (event->type == SDL_EVENT_QUIT) {
                    m_Running = false;
                }
                if (event->type == SDL_EVENT_KEY_DOWN) {
                    if (event->key.key == SDLK_ESCAPE) {
                        m_Running = false;
                    }

                    auto key = Input::Input::ConvertSDLKey(event->key.key);
                    auto action = event->key.repeat ? Input::KeyAction::Repeat : Input::KeyAction::Press;
                    Input::Input::SetKeyState(key, action);
                } else if (event->type == SDL_EVENT_KEY_UP) {
                    auto key = Input::Input::ConvertSDLKey(event->key.key);
                    Input::Input::SetKeyState(key, Input::KeyAction::Release);
                }
            }
        }

        void Update(float deltaTime) {
            // 游戏逻辑更新

            for (auto& obj : m_gameObjects) {
                obj.Update(deltaTime, m_gameObjects);
            }

            if (!m_gameObjects.empty()) {
                m_camera.Follow(m_gameObjects[0].GetPosition(), deltaTime);
            }
            
        }

        void Render() {
            m_renderer.beginFrame();
            m_renderer.clear({ 0.1f, 0.1f, 0.1f, 1.0f });

            Utils::Vector2 camOffset = m_camera.GetPosition();

            // 绘制内容
            for (auto& obj : m_gameObjects) {
                obj.Draw(m_renderer, camOffset);
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
        
        Scene::Camera m_camera{ 800.0f, 600.0f };
        std::vector<Scene::GameObject> m_gameObjects; // 游戏对象列表

        bool m_Running{false};

    };
}