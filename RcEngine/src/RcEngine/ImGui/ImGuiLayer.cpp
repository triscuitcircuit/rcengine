//
// Created by Tristan Zippert on 5/23/21.
//
#include "rcpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "GLFW/glfw3.h"
#include "RcEngine/Core/Application.h"

//temp
#include "../include/glad/glad.h"

#include "../imguizmo/ImGuizmo.h"


inline static int Black				= 0x00000000;
inline static int White				= 0xFFFFFF00;

inline static int AlphaTransparent	= 0x00;
inline static int Alpha20			= 0x33;
inline static int Alpha40			= 0x66;
inline static int Alpha50			= 0x80;
inline static int Alpha60			= 0x99;
inline static int Alpha80			= 0xCC;
inline static int Alpha90			= 0xE6;
inline static int AlphaFull			= 0xFF;

namespace RcEngine{

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {

    }
    ImGuiLayer::~ImGuiLayer(){

    }

    void ImGuiLayer::OnAttach() {
        RC_PROFILE_FUNCTION();

        //setup imgui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enables keyboard controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // enables keyboard controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.Fonts->AddFontFromFileTTF("Assets/Fonts/FiraSans/FiraSans-Bold.ttf",16.0f);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/FiraSans/FiraSans-Regular.ttf",16.0f);

        //setup of imgui style
        ImGui::StyleColorsDark();


        ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

        Application& app = Application::Get();
        GLFWwindow * window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window,true);

        ImGui_ImplOpenGL3_Init("#version 410");
    }
    void ImGuiLayer::OnDetach() {
        RC_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void ImGuiLayer::Begin() {
        RC_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }
    void ImGuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(),(float)app.GetWindow().GetHeight());

        //Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            GLFWwindow * backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

    }
    void ImGuiLayer::OnImGuiRender() {
        {
            ImGui::Begin("Renderer");
#ifdef IMGUI_IMPL_OPENGL_LOADER_GLAD
            ImGui::Text("OpenGL");
            ImGui::Text(
                    "Vendor: %s", glGetString(GL_VENDOR)
            );
            ImGui::Text(
                    "Renderer: %s", glGetString(GL_RENDERER)
            );
            ImGui::Text(
                    "Version: %s",glGetString(GL_VERSION)
                    );
            int max_size;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max_size);
            ImGui::Text("Max texture size: %i x %i",max_size, max_size);
            ImGui::TextWrapped("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

#elif CPPMetal
#include <TargetConditionals.h>
            id<MTLDevice> obj = MTLCreateSystemDefaultDevice();
            ImGui::Text("Could not detect renderer");
#else
            ImGui::Text("Could not detect renderer");
#endif
            ImGui::End();
        }

        static bool show = false;
        //ImGui::ShowDemoWindow(&show);
    }
    void ImGuiLayer::SetDarkThemeColors(){
        auto& style = ImGui::GetStyle();
        auto& colors = ImGui::GetStyle().Colors;

       // Set Imgui framerounding
       style.FrameRounding = 0.0f;
        style.GrabRounding = 2.5f;
        style.FrameBorderSize = 1.0f;
        style.PopupRounding = 0.0f;
        style.TabRounding = 0.0f;

        style.WindowMenuButtonPosition = 1;

        colors[ImGuiCol_Text]                   = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.56f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.76f, 0.28f, 0.44f, 0.67f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
        colors[ImGuiCol_Separator]              = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.07f, 0.07f, 0.07f, 0.51f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.86f, 0.23f, 0.43f, 0.67f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.19f, 0.19f, 0.19f, 0.57f);
        colors[ImGuiCol_TabUnfocused]           = ImVec4(0.05f, 0.05f, 0.05f, 0.90f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.13f, 0.13f, 0.13f, 0.74f);
        colors[ImGuiCol_DockingPreview]         = ImVec4(0.47f, 0.47f, 0.47f, 0.47f);
        colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }

}