#include "gl.h"

#include <GL/glew.h>

#include <cstdio>

#include "shader-manager.h"

#ifdef GL_DEBUG
#include <Windows.h>

static void APIENTRY opengl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (type == GL_DEBUG_TYPE_OTHER_ARB) return;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW_ARB:     fprintf(stderr, "[LOW] ");    break;
	case GL_DEBUG_SEVERITY_MEDIUM_ARB:  fprintf(stderr, "[MEDIUM] "); break;
	case GL_DEBUG_SEVERITY_HIGH_ARB:    fprintf(stderr, "[HIGH] ");   break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR_ARB:               fprintf(stderr, "ERROR: ");               break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: fprintf(stderr, "DEPRECATED_BEHAVIOR: "); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  fprintf(stderr, "UNDEFINED_BEHAVIOR: ");  break;
	case GL_DEBUG_TYPE_PORTABILITY_ARB:         fprintf(stderr, "PORTABILITY: ");         break;
	case GL_DEBUG_TYPE_PERFORMANCE_ARB:         fprintf(stderr, "PERFORMANCE: ");         break;
	case GL_DEBUG_TYPE_OTHER_ARB:               fprintf(stderr, "OTHER: ");               break;
	}

	fprintf(stderr, "id=0x%u %s\n", (unsigned int)id, message);
}
#endif


namespace gl
{


bool gl_init()
{
#ifdef GL_DEBUG
	glewExperimental = GL_TRUE;
#endif
	if (GLenum error = glewInit())
	{
		printf("GLEW failed to init. GLEW_ERR: %i\n", error);
		return false;
	}

#ifdef GL_DEBUG
	if (glDebugMessageCallbackARB)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB(opengl_callback, NULL);
		GLuint ids;
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &ids, true);
	}
#endif
	//////
	
	//enable multisampling for gl (sdl already done)
	glEnable(GL_MULTISAMPLE);

	//enable blending (transparency)
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set viewport
	glViewport(0, 0, 640, 480);

	shader::init_all();

	return true;
}

}