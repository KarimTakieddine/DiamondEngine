#include "RenderUploadObject.h"

namespace diamond_engine
{
	RenderUploadObject::RenderUploadObject(const std::vector<RenderUpload>& uploads) : m_uploads(uploads) { }

	void RenderUploadObject::setUploads(const std::vector<RenderUpload>& uploads)
	{
		m_uploads = uploads;
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