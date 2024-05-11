#pragma once

#include "EngineStatus.h"
#include "RenderUpload.h"
#include "ShaderProgram.h"

namespace diamond_engine
{
	class RenderUploadObject
	{
	public:
		explicit RenderUploadObject(const std::vector<RenderUpload>& uploads);
		void setUploads(const std::vector<RenderUpload>& uploads);
		const std::vector<RenderUpload>& getUploads() const;
		std::vector<RenderUpload>& getUploads();

	private:
		std::vector<RenderUpload> m_uploads;
	};
}
