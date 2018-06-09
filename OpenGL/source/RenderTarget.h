#pragma once

#include "Texture.h"

namespace aie {
	
class RenderTarget {
public:

	RenderTarget();
	RenderTarget(unsigned int targetCount, unsigned int width, unsigned int height, Texture::Format* f);
	virtual ~RenderTarget();

	bool initialise(unsigned int targetCount, unsigned int width, unsigned int height, Texture::Format* f);

	void bind();
	void unbind();

	unsigned int	getWidth() const { return m_width; }
	unsigned int	getHeight() const { return m_height; }

	unsigned int	getFrameBufferHandle() const { return m_fbo; }

	unsigned int	getTargetCount() const { return m_targetCount; }
	const Texture&	getTarget(unsigned int target) const { return m_targets[target]; }

	Texture*		m_targets;

protected:

	unsigned int	m_width;
	unsigned int	m_height;

	unsigned int	m_fbo;
	unsigned int	m_rbo;

	unsigned int	m_targetCount;
	
};

} // namespace aie