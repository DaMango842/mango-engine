#pragma once
#include <string>

namespace mango_engine {
    namespace Scene {
        class AnimationClip {
        public:
            std::string name;
            int startFrame{0};
            int endFrame{0};
            float frameDuration{0.1f}; // 每帧持续时间
            bool loop{true}; // 是否循环播放

            AnimationClip() = default;

            AnimationClip(const std::string& clipName, int start, int end, float duration, bool isLoop = true)
                : name(clipName), startFrame(start), endFrame(end), frameDuration(duration), loop(isLoop) {}
        };
    }
}