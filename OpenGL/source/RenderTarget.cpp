#include "RenderTarget.h"
#include "gl_core_4_4.h"
#include <vector>

namespace aie {

RenderTarget::RenderTarget()
	: m_width(0),
	m_height(0),
	m_targetCount(0),
	m_targets(nullptr) {
}

RenderTarget::RenderTarget(unsigned int targetCount, unsigned int width, unsigned int height, Texture::Format* f)
	: m_width(0),
	m_height(0),
	m_targetCount(0),
	m_targets(nullptr) {
	initialise(targetCount, width, height, f);
}

bool RenderTarget::initialise(unsigned int targetCount, unsigned int width, unsigned int height, Texture::Format* f) {

	// setup and bind a framebuffer object
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// create and attach textures
	if (targetCount > 0) {

		m_targets = new Texture[targetCount];

		std::vector<GLenum> drawBuffers = {};

		for (unsigned int i = 0; i < targetCount; ++i) {

			m_targets[i].create(width, height, f[i]);

			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
								 m_targets[i].getHandle(), 0);
		}

		glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());
	}

	// setup and bind a 24bit depth buffer as a render buffer
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
						  width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, m_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

		// cleanup
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		delete[] m_targets;
		m_targets = nullptr;
		glDeleteRenderbuffers(1, &m_rbo);
		glDeleteFramebuffers(1, &m_fbo);
		m_rbo = 0;
		m_fbo = 0;

		return false;
	}

	// success
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_targetCount = targetCount;
	m_width = width;
	m_height = height;

	return true;
}

RenderTarget::~RenderTarget() {
	delete[] m_targets;
	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteFramebuffers(1, &m_fbo);
}

void RenderTarget::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void RenderTarget::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace aie