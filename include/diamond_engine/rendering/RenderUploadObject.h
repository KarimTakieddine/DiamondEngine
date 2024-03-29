#pragma once

#include "EngineStatus.h"
#include "RenderUpload.h"
#include "ShaderProgram.h"

namespace diamond_engine
{
	class RenderUploadObject
	{
	public:
		void registerUpload(const RenderUpload& upload);

		const std::vector<RenderUpload>& getUploads() const;
		std::vector<RenderUpload>& getUploads();

	private:
		std::vector<RenderUpload> m_uploads;
	};
}
