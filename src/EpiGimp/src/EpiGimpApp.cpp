/*
** EpiGimp Copyright (C) 2020 Maxime Houis
** This program comes with ABSOLUTELY NO WARRANTY.
** This is free software, and you are welcome to redistribute it
** under certain conditions; see LICENSE for details.
*/
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "imgui-SFML.h"
#include "EpiGimpApp.hpp"
#include "Engine/UI/UiWindow.hpp"
#include "Engine/UI/AWidget.hpp"
#include "Engine/UI/Widget/WButton.hpp"
#include "Engine/UI/Widget/WMenuItem.hpp"
#include "Engine/UI/Window/MainMenuBar.hpp"
#include <iostream>
#include <functional>

usa::EpiGimpApp::EpiGimpApp(int ac, char **av) :
    Application{ ac, av }
{
}

auto usa::EpiGimpApp::init() -> void
{
    plop.loadFromFile("./red.jpg");
}

auto usa::EpiGimpApp::deinit() -> void
{
}

auto usa::EpiGimpApp::tick(float) -> void
{
}


    //     if (ImGui::BeginMainMenuBar())
    //     {
    //         if (ImGui::BeginMenu("File"))
    //         {
    //             ImGui::EndMenu();
    //         }
    //         if (ImGui::BeginMenu("Edit"))
    //         {
    //             if (ImGui::MenuItem("Undo", "CTRL+Z"))
    //             {
    //             }
    //             if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
    //             {
    //             } // Disabled item
    //             ImGui::Separator();
    //             if (ImGui::MenuItem("Cut", "CTRL+X"))
    //             {
    //             }
    //             if (ImGui::MenuItem("Copy", "CTRL+C"))
    //             {
    //             }
    //             if (ImGui::MenuItem("Paste", "CTRL+V"))
    //             {
    //             }
    //             ImGui::EndMenu();
    //         }
    //         ImGui::EndMainMenuBar();
    //     }
    // }
void pute()
{
    std::cout << "pute" << std::endl;
}

void f1(int, int)
{
    std::cout << "plopiplopi" << std::endl;
}

auto usa::EpiGimpApp::draw() -> void
{
    // ImGui::SetNextWindowSize(ImVec2(900, 60));
    // ImGui::SetNextWindowPos(ImVec2(0, 0));
    // ImGui::SetNextWindowCollapsed(false);
    ImGuiWindowFlags window_flags = 0;
    static bool cpt = false;
    static Engine::UiWindow *win = new Engine::MainMenuBar();

    // std::function f = std::bind(f1<int>, 12);
    if (cpt == false)
    {
        Engine::WMenuItem item("Item A");
        item.bindAction(std::bind(pute));
        std::shared_ptr<Engine::AWidget> it = std::make_shared<Engine::WMenuItem>(item);
        win->addWidget(it);

        Engine::WButton button("Button A");
        button.bindAction(std::bind(f1, 5, 2));
        std::shared_ptr<Engine::AWidget> wid = std::make_shared<Engine::WButton>(button);
        win->addWidget(wid);
        cpt = !cpt;
    }

    win->render();
    // static bool plopi = false;
    // ImGui::ShowDemoWindow(&plopi);
    // // ShowExampleAppMainMenuBar();
    // if (ImGui::BeginMainMenuBar())
    // {
    //     if (ImGui::BeginMenu("Menu"))
    //     {
    //         ImGui::MenuItem("File", "CTRL+N");
    //         ImGui::MenuItem("Create");
    //         ImGui::MenuItem("Open", "CTRL+O");
    //         ImGui::MenuItem("Open as Layers", "CTRL+ALT+O");
    //         ImGui::MenuItem("Open Location");
    //         ImGui::MenuItem("Open Recent");
    //         ImGui::Separator();
    //         ImGui::MenuItem("Save", "CTRL+S");
    //         ImGui::MenuItem("Save As...", "Shift+CTRL+S");
    //         ImGui::MenuItem("Save a Copy");
    //         ImGui::MenuItem("Revert");
    //         ImGui::Separator();
    //         ImGui::MenuItem("Export");
    //         ImGui::MenuItem("Export As");
    //         ImGui::MenuItem("Properties");
    //         ImGui::Separator();
    //         ImGui::MenuItem("Close View", "CTRL+W");
    //         ImGui::MenuItem("Close all", "Shift+CTRL+W");
    //         ImGui::MenuItem("Quit", "CTRL+Q");
    //         ImGui::Selectable("Demo", &plopi);
    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("Edit"))
    //     {
    //         ImGui::MenuItem("Undo");
    //         ImGui::MenuItem("Redo");
    //         ImGui::MenuItem("Fade");
    //         ImGui::MenuItem("Undo History");
    //         ImGui::MenuItem("Cut");
    //         ImGui::MenuItem("Copy");
    //         ImGui::MenuItem("Copy Visible");
    //         ImGui::MenuItem("Paste");
    //         ImGui::MenuItem("Paste Into");
    //         ImGui::MenuItem("Past As");
    //         ImGui::MenuItem("Buffer");
    //         ImGui::MenuItem("Clear");
    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("Select"))
    //     {
    //         ImGui::MenuItem("All");
    //         ImGui::MenuItem("None");
    //         ImGui::MenuItem("Invert");
    //         ImGui::MenuIine::WButton>(buttotem("Float");
    //         ImGui::MenuItem("By Color");
    //         ImGui::MenuItem("From Path");
    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("View"))
    //     {
    //         ImGui::MenuItem("Zoom");
    //         ImGui::MenuItem("Fullscreen");
    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::ine::WButton>(butto::WButton>(buttoMenu("Image"))
    //     {
    //         ImGui::MenuItem("Duplicate");
    //         ImGui::MenuItem("Mode");
    //         ImGui::MenuItem("Transform");
    //         ImGui::MenuItem("Canvas Size");
    //         ImGui::MenuItem("None");
    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("Layer"))
    //     {
    //         ImGui::MenuItem("New Layer");
    //         ImGui::MenuItem("New From Visible");
    //         ImGui::MenuItem("New Layer Group");
    //         ImGui::MenuItem("Merge Down");
    //         ImGui::MenuItem("Merge Up");
    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("Colors"))
    //     {

    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("Tools"))
    //     {
    //         ImGui::MenuItem("Selection Tools");
    //         ImGui::MenuItem("Paint Tools");
    //         ImGui::MenuItem("Transform Tools");
    //         ImGui::MenuItem("Color Tools");
    //         ImGui::MenuItem("Color Picker");
    //         ImGui::MenuItem("Zoom");
    //         ImGui::MenuItem("Measure", "SHIFT+M");
    //         ImGui::MenuItem("Text");
    //         ImGui::MenuItem("Toolbox");
    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("Filters"))
    //     {

    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("Windows"))
    //     {

    //         ImGui::EndMenu();
    //     }
    //     if (ImGui::BeginMenu("Help"))
    //     {
    //         ImGui::EndMenu();
    //     }
    //     ImGui::EndMainMenuBar();
    // }

    // //------------------------------------------------

    // // ImGui::SetNextWindowCollapsed(false);
    // ImGui::SetNextWindowSize(ImVec2(50, 900));
    // ImGui::SetNextWindowPos(ImVec2(0, 60));

    // // window_flags |= ImGuiWindowFlags_NoTitleBar;
    // window_flags |= ImGuiWindowFlags_NoMove;
    // window_flags |= ImGuiWindowFlags_NoBackground;
    // if (ImGui::Begin("", NULL, window_flags))
    // {
    //     if (ImGui::Button("A", ImVec2(30,30)))
    //         std::cout << "A" << std::endl;
    //     if (ImGui::Button("B", ImVec2(30,30)))
    //         std::cout << "B" << std::endl;
    //     if (ImGui::Button("C", ImVec2(30,30)))
    //         std::cout << "C" << std::endl;
    //     if (ImGui::Button("D", ImVec2(30,30)))
    //         std::cout << "D" << std::endl;
    //     if (ImGui::Button("E", ImVec2(30,30)))
    //         std::cout << "E" << std::endl;
    //     if (ImGui::Button("G", ImVec2(30,30)))
    //         std::cout << "G" << std::endl;
    //     if (ImGui::Button("H", ImVec2(30,30)))
    //         std::cout << "H" << std::endl;
    //     if (ImGui::Button("I", ImVec2(30,30)))
    //         std::cout << "I" << std::endl;
    //     ImGui::End();
    // }
    // static sf::RectangleShape canvas;
    // canvas.setSize(sf::Vector2f(1000, 500));
    // canvas.setOrigin(500, 250);
    // canvas.setFillColor(sf::Color::White);
    // canvas.setPosition(600, 300);
    // m_window.draw(canvas);
}
