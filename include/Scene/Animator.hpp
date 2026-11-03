#pragma once
#include <Scene/AnimationClip.hpp>
#include <unordered_map>
#include <string>

namespace mango_engine {
    namespace Scene {
        class Animator {
        public:
            Animator() = default;
            
            void AddAnimationClip(const AnimationClip& clip) {
                m_animationClips.insert_or_assign(clip.name, clip);
            }

            void PlayAnimation(const std::string& clipName) {
                // 1. 如果已经在播这个，直接忽略，防止每帧重置
                if (m_currentClipName == clipName) return;

                // 2. 查找是否存在
                auto it = m_animationClips.find(clipName);
                if (it != m_animationClips.end()) {
                    m_currentClipName = clipName;
                    m_currentFrame = it->second.startFrame;
                    m_frameTimer = 0.0f;
                }
            }

            void Update(float deltaTime) {
                // 如果没设置当前动画，直接返回
                if (m_currentClipName.empty()) return;

                auto it = m_animationClips.find(m_currentClipName);
                if (it == m_animationClips.end()) return;

                const auto& clip = it->second;

                m_frameTimer += deltaTime;
                if (m_frameTimer >= clip.frameDuration) {
                    m_frameTimer -= clip.frameDuration;
                    m_currentFrame++;
                    
                    if (m_currentFrame > clip.endFrame) {
                        if (clip.loop) {
                            m_currentFrame = clip.startFrame;
                        } else {
                            m_currentFrame = clip.endFrame; // 停在最后一帧
                        }
                    }
                }
            }

            int GetCurrentFrame() const { return m_currentFrame; }

        private:
            std::unordered_map<std::string, AnimationClip> m_animationClips;
            std::string m_currentClipName = ""; // 用名字代替指针，杜绝野指针
            int m_currentFrame = 0;
            float m_frameTimer = 0.0f;
        };
    }
}