// 通用音频加载接口
#pragma once

#include "Audio/AudioBuffer.h"
#include <string>

namespace mango_engine
{
	namespace Loader
	{
		class IAudioLoader
		{
		public:
			virtual ~IAudioLoader() = default;
			virtual bool CanLoad(const std::string& filePath) = 0;
			// 加载音频文件，返回一个AudioBuffer对象
			virtual AudioBuffer Load(const std::string& filePath) = 0;
		};
	}
}