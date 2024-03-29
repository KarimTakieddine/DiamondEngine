#include "RenderUploadObject.h"

namespace diamond_engine
{
	void RenderUploadObject::registerUpload(const RenderUpload& upload)
	{
		m_uploads.push_back(upload);
	}

	const std::vector<RenderUpload>& RenderUploadObject::getUploads() const
	{
		return m_uploads;
	}

	std::vector<RenderUpload>& RenderUploadObject::getUploads()
	{
		return m_uploads;
	}
}