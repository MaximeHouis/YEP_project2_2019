/*
** EpiGimp Copyright (C) 2020 Maxime Houis
** This program comes with ABSOLUTELY NO WARRANTY.
** This is free software, and you are welcome to redistribute it
** under certain conditions; see LICENSE for details.
*/

#pragma once

#include <Engine/Application.hpp>

namespace usa {

class EpiGimpApp final : public Engine::Application {
public:
    EpiGimpApp(int ac, char **av);
    virtual ~EpiGimpApp() = default;

    auto init() -> void;
    auto deinit() -> void;
    auto tick(double deltaTime) -> void;
    auto draw() -> void;
};

} // namespace usa