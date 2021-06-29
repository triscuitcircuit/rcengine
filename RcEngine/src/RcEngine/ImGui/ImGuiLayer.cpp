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
#include "include/glad/glad.h"

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
            ImGui::Text("Max texture size: %s",glGetString(GL_MAX_3D_TEXTURE_SIZE));
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
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
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f,0.11f, 1.0f};


        //Header color
        colors[ImGuiCol_Header] = ImVec4{0.2f,0.205f,0.201f,1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Frame Background
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.5f, 0.205f, 0.505f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Tab colorization
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		//Title Colorization
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    }

}