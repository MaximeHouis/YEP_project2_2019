#pragma once

#include "Engine/UI/AWidget.hpp"
#include <functional>
#include <string_view>

namespace usa
{

    namespace Engine
    {

        class WMenuItem : public AWidget
        {
        private:
            std::string_view m_shortcut{};
            std::function<void()> f;
        public:
            WMenuItem(const std::string_view &name, const std::string_view &shortcut = "") : AWidget(name, ImVec2()), m_shortcut(shortcut) {}
            virtual ~WMenuItem() = default;

            auto bindAction(std::function<void()> func) -> void override;
            auto render() -> void override;
        };

    } // namespace Engine

} // namespace usa