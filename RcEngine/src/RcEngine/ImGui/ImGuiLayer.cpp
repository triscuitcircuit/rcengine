//
// Created by Tristan Zippert on 5/23/21.
//

#include "rcpch.h"
#include "ImGuiLayer.h"


#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "Platform/OpenGL/imgui_impl_glfw.h"

#include "GLFW/glfw3.h"
#include "RcEngine/Core/Application.h"

//temp
#include "external/glad/include/glad/glad.h"



namespace RcEngine{

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {

    }
    ImGuiLayer::~ImGuiLayer(){

    }


    void ImGuiLayer::OnUpdate() {

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time < 0.0 ? (time - m_Time): (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    void ImGuiLayer::OnAttach() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        //temp imgui keycodes
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        Application& app = Application::Get();
        GLFWwindow * window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window,true);

        ImGui_ImplOpenGL3_Init("#version 410");
    }
    void ImGuiLayer::OnDetach() {

    }

#define BIND_DIS(dispatcher,event,fn) dispatcher.Dispatch<event>(RC_BIND_EVENT_TO_FUNCTION(ImGuiLayer::fn))

    void ImGuiLayer::OnEvent(Event &event) {
        EventDispatcher dispatcher(event);

        BIND_DIS(dispatcher,MouseButtonPressedEvent,OnMouseButtonPressedEvent);
        BIND_DIS(dispatcher,MouseButtonReleasedEvent,OnMouseButtonReleasedEvent);
        BIND_DIS(dispatcher, MouseScrolledEvent,OnMouseScrolledEvent);
        BIND_DIS(dispatcher, MouseMovedEvent, OnMouseMoveEvent);


        BIND_DIS(dispatcher,KeyPressedEvent,OnKeyPressedEvent);
        BIND_DIS(dispatcher,KeyReleasedEvent,OnKeyReleased);
        BIND_DIS(dispatcher,WindowResizeEvent,OnWindowResize);


    }


    // Actions
#define KEY_CODE_MOD_RL(GUIio, fn, GLFW_L_KC, GLFW_R_KC) io.fn[GLFW_L_KC]||io.fn[GLFW_R_KC]

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();

        io.KeysDown[e.GetKeyCode()] = true;



        io.KeyCtrl = KEY_CODE_MOD_RL(io,KeysDown,GLFW_KEY_LEFT_CONTROL,GLFW_KEY_RIGHT_CONTROL);
        io.KeyShift = KEY_CODE_MOD_RL(io,KeysDown,GLFW_KEY_LEFT_SHIFT,GLFW_KEY_RIGHT_SHIFT);
        io.KeyAlt = KEY_CODE_MOD_RL(io,KeysDown,GLFW_KEY_LEFT_ALT,GLFW_KEY_RIGHT_ALT);
        io.KeySuper = KEY_CODE_MOD_RL(io,KeysDown,GLFW_KEY_LEFT_SUPER,GLFW_KEY_RIGHT_SUPER);
        return false;
    }

    bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();

        io.KeysDown[e.GetKeyCode()] = false;


        return false;
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMoveEvent(MouseMovedEvent &e){
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(),e.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += e.GetXOffset();
        io.MouseWheel += e.GetYOffset();

        return false;
    }
    bool ImGuiLayer::OnKeyTyped(KeyTypedEvent &e){
        ImGuiIO& io = ImGui::GetIO();

        int keycode = e.GetKeyCode();

        io.AddInputCharacter((unsigned short)keycode);

        return false;
    }

    bool ImGuiLayer::OnWindowResize(WindowResizeEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(),e.GetHeight());

        io.DisplayFramebufferScale = ImVec2(1.0f,1.0f);

        glViewport(0,0,e.GetWidth(),e.GetHeight());

        return false;
    }
}