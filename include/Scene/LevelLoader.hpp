#pragma once
#include <string>
#include <vector>
#include <Scene/GameObject.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

namespace mango_engine {
    namespace Scene {

        class LevelLoader {
        public:
            // 读取 JSON 并填充对象列表
            static bool LoadLevel(const std::string& filePath, std::vector<GameObject>& outObjects, Resource::Texture::Ptr playerTexture) {
                std::ifstream file(filePath);
                if (!file.is_open()) {
                    SDL_Log("Failed to open level file: %s", filePath.c_str());
                    return false;
                }

                try {
                    nlohmann::json j;
                    file >> j;

                    // 1. 加载玩家
                    auto playerData = j["player"];
                    float px = playerData["position"]["x"];
                    float py = playerData["position"]["y"];
                    float pSpeed = playerData["speed"];
                    
                    GameObject player(Utils::Vector2{px, py}, Utils::Vector2{50.0f, 50.0f}, Renderer::Color{0.0f, 1.0f, 0.0f, 1.0f}, pSpeed);
                    if (playerTexture) {
                        player.SetTexture(playerTexture);
                    }
                    outObjects.push_back(player);

                    // 2. 加载敌人 (循环读取数组)
                    if (j.contains("enemies")) {
                        for (const auto& enemyData : j["enemies"]) {
                            float ex = enemyData["position"]["x"];
                            float ey = enemyData["position"]["y"];
                            float ew = enemyData["size"]["x"];
                            float eh = enemyData["size"]["y"];
                            float es = enemyData["speed"];
                            
                            Renderer::Color color;
                            color.r = enemyData["color"]["r"];
                            color.g = enemyData["color"]["g"];
                            color.b = enemyData["color"]["b"];
                            color.a = enemyData["color"]["a"];

                            outObjects.emplace_back(Utils::Vector2{ex, ey}, Utils::Vector2{ew, eh}, color, es);
                        }
                    }

                    return true;
                } catch (const std::exception& e) {
                    SDL_Log("Failed to parse JSON: %s", e.what());
                    return false;
                }
            }
        };
    }
}