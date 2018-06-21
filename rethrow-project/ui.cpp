#include "ui.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "lib/imgui/imgui.h"
#include "io.h"
#include "window.h"

#include "shader-base.h"
#include "shader-indexed-triangle.h"

namespace ui
{


double time = 0.0f;
bool mouse_pressed[3] = { false, false, false };
float mouse_wheel = 0.0f;


void render_ui(ImDrawData* draw_data);
void set_clipboard_text(void* user_data, const char* text);
const char* get_clipboard_text(void* user_data);

void init()
{
	ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab]        = SDLK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]  = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]    = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow]  = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp]     = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown]   = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home]       = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End]        = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete]     = SDLK_DELETE;
    io.KeyMap[ImGuiKey_Backspace]  = SDLK_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter]      = SDLK_RETURN;
    io.KeyMap[ImGuiKey_Escape]     = SDLK_ESCAPE;
    io.KeyMap[ImGuiKey_A]          = SDLK_a;
    io.KeyMap[ImGuiKey_C]          = SDLK_c;
    io.KeyMap[ImGuiKey_V]          = SDLK_v;
    io.KeyMap[ImGuiKey_X]          = SDLK_x;
    io.KeyMap[ImGuiKey_Y]          = SDLK_y;
    io.KeyMap[ImGuiKey_Z]          = SDLK_z;

    io.RenderDrawListsFn  = render_ui;
    io.SetClipboardTextFn = set_clipboard_text;
    io.GetClipboardTextFn = get_clipboard_text;
    io.ClipboardUserData  = NULL;
}

void cleanup()
{
	ImGui::DestroyContext();
}


bool ui_owns_keyboard;
bool ui_owns_mouse;
void handle_events(SDL_Event* event)
{
    ImGuiIO& io = ImGui::GetIO();

    switch (event->type)
    {

    case SDL_MOUSEWHEEL:
    {
        if (event->wheel.y > 0)
            mouse_wheel = 1;
        else if (event->wheel.y < 0)
            mouse_wheel = -1;
		else
			mouse_wheel = 0;
    } break;

    case SDL_MOUSEBUTTONDOWN:
    {
        if (event->button.button == SDL_BUTTON_LEFT) mouse_pressed[0] = true;
        if (event->button.button == SDL_BUTTON_RIGHT) mouse_pressed[1] = true;
        if (event->button.button == SDL_BUTTON_MIDDLE) mouse_pressed[2] = true;
    } break;

    case SDL_TEXTINPUT:
    {
        io.AddInputCharactersUTF8(event->text.text);
    } break;

    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
        int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
        io.KeysDown[key] = (event->type == SDL_KEYDOWN);
        io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
        io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
        io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
        io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
    } break;

    }

    ui_owns_mouse = io.WantCaptureMouse;
    ui_owns_keyboard = io.WantCaptureKeyboard;
}


void begin_frame()
{
    ImGuiIO& io = ImGui::GetIO();

	window::update_screen_size(); //very likely not needed, test later.
	const int screen_w 		= window::screen_width;
	const int screen_h 		= window::screen_height;
	const int drawable_w 	= window::drawable_width;
	const int drawable_h 	= window::drawable_height;

	io.DisplaySize = ImVec2((float)screen_w, (float)screen_h);
	io.DisplayFramebufferScale = ImVec2(screen_w > 0 ? ((float)drawable_w / screen_w) : 0,
										screen_h > 0 ? ((float)drawable_h / screen_h) : 0);


	double current_time = SDL_GetTicks() / 1000.0f;
	io.DeltaTime = time > 0.0 ? (float)(current_time - time) : (float)(1.0f / 60.0f);
    time = current_time;

    int mx, my;
    Uint32 mouseMask = SDL_GetMouseState(&mx, &my);
    if (SDL_GetWindowFlags(window::the_window) & SDL_WINDOW_MOUSE_FOCUS)
        io.MousePos = ImVec2((float) mx, (float) my);
    else
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    io.MouseDown[0] = mouse_pressed[0] || (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    io.MouseDown[1] = mouse_pressed[1] || (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = mouse_pressed[2] || (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
    mouse_pressed[0] = mouse_pressed[1] = mouse_pressed[2] = false;

    io.MouseWheel = mouse_wheel;
    mouse_wheel = 0.0f;

    //if (is_in_camera_mode) return;

    SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);
    ImGui::NewFrame();

	//this will probably have to be changed
    ImGui::SetNextWindowPos(ImVec2(600, 20), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiSetCond_FirstUseEver);

    ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoCollapse);
}

void end_frame()
{
	//if (is_in_camera_mode) return;
	ImGui::End();
	ImGui::Render();
}

void render_ui(ImDrawData* draw_data)
{
	ImGuiIO& io = ImGui::GetIO();

    int width 	= (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int height 	= (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (width == 0 || height == 0) //dont render if window is minimised
        return;

    //scale according to ratio of screen_wh and drawable_wh
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    //blending
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //stuff
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_SCISSOR_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //sending to shader and drawing
    using namespace gl::shader::indexed_triangle;

    const GLuint shader_ID = gl::shader::indexed_triangle::ID;

    const glm::mat4 ortho_transform = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
    gl::shader::set_mat4(shader_ID, "transform", ortho_transform);


    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;


		/*this works because Vertex and ImDrawVert are practically identical:
		 *
		 *struct ImDrawVert			struct Vertex
		 *{							{
		 *    ImVec2  pos;				Vec2 position;
		 *    ImVec2  uv;				Vec2 uv;
		 *    ImU32   col;				Vec4_bytes color; (== u32)
		 *};						};
        */
        set_vertex_data((Vertex*)cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size);
        set_index_data(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size);

        //drawing
        int offset = 0;
        for (int i = 0; i < cmd_list->CmdBuffer.Size; i++)
        {
            const ImDrawCmd* cmd = &cmd_list->CmdBuffer[i];
            if (cmd->UserCallback)
            {
                cmd->UserCallback(cmd_list, cmd);
            }
            else
            {
            	glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->TextureId); //ABSTRACT THIS OUT ASAP!!!
                glScissor((int) cmd->ClipRect.x, (int)(height - cmd->ClipRect.w), (int)(cmd->ClipRect.z - cmd->ClipRect.x), (int)(cmd->ClipRect.w - cmd->ClipRect.y));

                gl::shader::indexed_triangle::draw(cmd->ElemCount, offset);
            }
            offset += cmd->ElemCount;
        }
    }

    glDisable(GL_SCISSOR_TEST);
    //glDisable(GL_BLEND); blend is permanetly turned on for now
}

void set_clipboard_text(void* user_data, const char* text)
{
    SDL_SetClipboardText(text);
}

const char* get_clipboard_text(void* user_data)
{
    return SDL_GetClipboardText();
}


}