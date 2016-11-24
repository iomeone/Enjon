#include "Graphics/GBuffer.h"
#include "Utils/Errors.h"
#include "Defines.h"
#include <stdio.h>

namespace Enjon { namespace Graphics {

	GBuffer::GBuffer(uint32 _Width, uint32 _Height)
	{
		// Save extensions
		Width  = _Width;
		Height = _Height;
		Viewport = EM::Vec4(0, 0, Width, Height);

	    glGenFramebuffers(1, &FBO);
	    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	    for (u32 i = 0; i < (u32)GBufferTextureType::GBUFFER_TEXTURE_COUNT; i++)
	    {
		    // Bind the diffuse render target
			glBindRenderbufferEXT(GL_RENDERBUFFER, TargetIDs[i]);
			glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_RGBA, Width, Height);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, TargetIDs[i]);

		    // - Diffuse buffer
		    glGenTextures(1, &Textures[i]);
		    glBindTexture(GL_TEXTURE_2D, Textures[i]);
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Width, Height, 0, GL_RGB, GL_FLOAT, NULL);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Textures[i], 0);
		    glBindTexture(GL_TEXTURE_2D, 0);
	    }
	 
	    // - Create and attach depth buffer (renderbuffer)
	    glGenRenderbuffers(1, &DepthBuffer);
	    glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffer);
	    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, Width, Height);
	    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer);

	    // - Finally check if framebuffer is complete
	    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) EU::FatalError("GBuffer::Constructor::Gbuffer could not be created.");

	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::SetViewport(const EM::Vec4& _Viewport)
	{
		Viewport = _Viewport;	
	}

	GBuffer::~GBuffer()
	{

	}

	void GBuffer::Bind()
	{
		// Bind our FBO and set the viewport to the proper size
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport((u32)Viewport.x, (u32)Viewport.y, (u32)Viewport.z, (u32)Viewport.w);

		// Clear the render targets
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH);

		// Specify what to render an start acquiring
		GLenum buffers[(u32)GBufferTextureType::GBUFFER_TEXTURE_COUNT];
		for (auto i = 0; i < (u32)GBufferTextureType::GBUFFER_TEXTURE_COUNT; i++)
		{
			buffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		glDrawBuffers((u32)GBufferTextureType::GBUFFER_TEXTURE_COUNT, buffers);
	}
			
	void GBuffer::Unbind()
	{
		// Stop acquiring and unbind the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glPopAttrib();
	}
}}

