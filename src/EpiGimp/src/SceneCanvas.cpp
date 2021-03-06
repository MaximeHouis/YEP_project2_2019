/*
** EpiGimp Copyright (C) 2020 Maxime Houis
** This program comes with ABSOLUTELY NO WARRANTY.
** This is free software, and you are welcome to redistribute it
** under certain conditions; see LICENSE for details.
*/

#include <imgui.h>
#include <imgui-SFML.h>

#include <algorithm>
#include <iostream>

#include "SceneCanvas.hpp"
#include "CanvasMenus.hpp"

#include "Tools/Pencil.hpp"
#include "Tools/Eraser.hpp"
#include "Tools/MoveLayer.hpp"
#include "Tools/ColorPicker.hpp"

SceneCanvas::SceneCanvas()
        : m_tools{std::make_unique<Pencil>(),
                  std::make_unique<Eraser>(),
                  std::make_unique<MoveLayer>(),
                  std::make_unique<ColorPicker>()}
{
}

SceneCanvas::SceneCanvas(unsigned int width, unsigned int height) : SceneCanvas()
{
    m_layers.emplace_back(width, height);
}

SceneCanvas::SceneCanvas(const std::vector<std::string_view> &files) : SceneCanvas()
{
    for (const auto &file : files)
        m_layers.emplace_back(file);
}

bool SceneCanvas::onCreate(Engine::Application &)
{
    return true;
}

void SceneCanvas::onEvent(const sf::Event &event)
{
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_mouseGrabbed = true;
            m_grabPoint = m_window->mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

            m_cursor.loadFromSystem(sf::Cursor::Hand);
            m_window->setMouseCursor(m_cursor);
        }
        break;
    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_mouseGrabbed = false;

            m_cursor.loadFromSystem(sf::Cursor::Arrow);
            m_window->setMouseCursor(m_cursor);
        }
        break;
    case sf::Event::MouseMoved:
        if (m_mouseGrabbed) {
            sf::View view = m_window->getView();
            const auto dropDelta = m_grabPoint - m_window->mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
            view.move(dropDelta);
            m_window->setView(view);
        }
        break;
    case sf::Event::MouseWheelScrolled: updateView({}, -event.mouseWheelScroll.delta); break;
    default: break;
    }

    if (m_activeTool) {
        m_activeTool->handleEvent(event);
    }
}

void SceneCanvas::onTick(float deltaTime)
{
    m_deltaTime = deltaTime;
    std::string exportPath{};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        updateView({0.f, -1.f});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        updateView({-1.f, 0.f});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        updateView({0.f, 1.f});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        updateView({1.f, 0.f});

    for (auto &layer : m_layers) {
        layer.texture.update(layer.image);

        // Performance cost: the texture ID stays the same but the cache ID changes.
        // Without this line, only the last texture is visible, the others appear blank.
        // TODO: One thing we could do is to only update the texture once a change has been made.
        layer.sprite.setTexture(layer.texture);
    }

    /**
     * create new workspace
     */
    menu.create(m_layers);

    /**
     * open layers from a backup file
     */
    exportPath = menu.getOpenPath();
    if (!exportPath.empty()) {
        if (!menu.open(m_layers, exportPath))
            menu.enableErrorDialog();
    }

    /**
     * open as layers from image
     */
    exportPath = menu.getOpenAsLayerPath();
    if (!exportPath.empty()) {
        if (!menu.openAsLayer(m_layers, exportPath))
            menu.enableErrorDialog();
    }

    /**
     * save layers in a backup file
     */

    exportPath = menu.getSavePath();
    if (!exportPath.empty()) {
        if (!menu.save(m_layers, exportPath))
            menu.enableErrorDialog();
    }

    /**
     * export layers as an Image
     */
    exportPath = menu.getExportPath();
    if (!exportPath.empty()) {
        static const char *extList[4] = {"bmp", "png", "tga", "jpg"};
        std::string extension = exportPath.substr(exportPath.find_last_of('.') + 1);

        for (auto i = 0; i < 4; ++i) {
            if (extList[i] == extension) {
                if (squash().saveToFile(exportPath) == false) {
                    menu.enableErrorDialog();
                }
                break;
            }
            if (i == 3) {
                menu.enableErrorDialog();
            }
        }
    }
}

auto SceneCanvas::updateView(sf::Vector2f delta, const float zoomDelta) const -> void
{
    auto view = m_window->getView();

    delta.x *= MOVE_SPEED * m_deltaTime;
    delta.y *= MOVE_SPEED * m_deltaTime;
    view.move(delta);

    if (zoomDelta > 0)
        view.zoom(1.1f);
    else if (zoomDelta < 0)
        view.zoom(0.9f);

    m_window->setView(view);
}

void SceneCanvas::onDraw()
{
    for (const auto &layer : m_layers) {
        if (!layer.hidden)
            m_window->draw(layer.sprite);
    }

    menu.drawMainMenuBar();
    drawLayerWindow();
    drawToolbox();
}

auto SceneCanvas::drawToolbox() -> void
{
    ImGui::SetNextWindowSize({0, 0});
    ImGui::Begin("Toolbox");
    {
        for (auto &tool : m_tools) {
            if (ImGui::Button(tool->getName(), {100, 0}))
                m_activeTool = tool.get();
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Selected tool: %s", (m_activeTool ? m_activeTool->getName() : "None"));
        if (ImGui::Button("Deselect tool"))
            m_activeTool = nullptr;
        if (m_activeTool) {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            m_activeTool->toolGUI();
        }
    }
    ImGui::End();
}

auto SceneCanvas::drawLayerWindow() -> void
{
    static const ImVec2 SmallButtonSize = {75, 0};
    static const ImVec2 LargeButtonSize = {SmallButtonSize.x * 2 + 10, 0};

    ImGui::SetNextWindowSize({0, 0});
    ImGui::Begin("Layers");
    {
        if (ImGui::Button("New layer", LargeButtonSize))
            m_layers.emplace_back(800, 600);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        std::size_t index = m_layers.size();
        for (auto layer = m_layers.rbegin(); layer != m_layers.rend(); ++layer) {
            ImGui::PushID(std::addressof(*layer));
            {
                if (m_activeLayer == &*layer)
                    ImGui::TextColored({255, 255, 0, 255}, "Layer %ld - Selected", index);
                else
                    ImGui::Text("Layer %ld", index);
                ImGui::BeginGroup();
                {
                    if (ImGui::Button("Up", SmallButtonSize))
                        swapLayers(layer, -1);
                    if (ImGui::Button("Down", SmallButtonSize))
                        swapLayers(layer, 1);
                    if (ImGui::Button(layer->hidden ? "Show" : "Hide", SmallButtonSize))
                        layer->hidden = !layer->hidden;
                }
                ImGui::EndGroup();
                ImGui::SameLine();
                ImGui::Image(layer->texture, {SmallButtonSize.x, SmallButtonSize.x / layer->ratio});

                ImGui::Spacing();
                if (ImGui::Button("Select layer", LargeButtonSize)) {
                    for (auto &tool : m_tools)
                        tool->setActiveLayer(&*layer);
                    m_activeLayer = &*layer;
                }

                if (ImGui::Button("Delete layer", LargeButtonSize))
                    m_layers.erase(std::next(layer).base());

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
            }
            ImGui::PopID();
            --index;
        }

        if (ImGui::Button("Deselect layer", LargeButtonSize)) {
            m_activeLayer = nullptr;
            for (auto &tool : m_tools)
                tool->setActiveLayer(nullptr);
        }
        if (ImGui::Button("Squash and export", LargeButtonSize))
            squash().saveToFile("export.png");
    }
    ImGui::End();
}

auto SceneCanvas::swapLayers(decltype(m_layers)::reverse_iterator &current, int offset) -> void
{
    if (offset == -1) {
        // Move UP
        if (current == m_layers.rbegin())
            return;
    } else if (offset == 1) {
        // Move DOWN
        if (current + 1 == m_layers.rend())
            return;
    }
    std::iter_swap(current, current + offset);
}

auto SceneCanvas::getLargestImageSize() const -> sf::Vector2u
{
    sf::Vector2u max{};

    for (const auto &layer : m_layers) {
        max = std::max(max, layer.image.getSize(), [](const sf::Vector2u &a, const sf::Vector2u &b) {
            return a.x * a.y < b.x * b.y;
        });
    }
    return max;
}

auto SceneCanvas::squash() const -> sf::Image
{
    sf::Image image{};
    const auto largest = getLargestImageSize();
    image.create(largest.x, largest.y, sf::Color::Transparent);

    for (const auto &layer : m_layers) {
        const auto &pos = layer.sprite.getPosition();
        image.copy(layer.image, static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y), sf::IntRect{}, true);
    }
    return image;
}

void SceneCanvas::registerWindow(sf::RenderWindow &window)
{
    Scene::registerWindow(window);

    for (auto &tool : m_tools)
        tool->registerWindow(window);
}
