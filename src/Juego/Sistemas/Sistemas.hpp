#pragma once
#include "../../Motor/Primitivos/Objetos.hpp"
#include "../Figuras/Figuras.hpp"
#include "../../Motor/Componentes/IComponentes.hpp"
#include <vector>
#include <memory>

namespace IVJ
{



    [[maybe_unused]] void SistemaRectangulo(
        const std::vector<std::shared_ptr<CE::Objeto>>& entes,
        float dt,
        float altoVentana);

    [[maybe_unused]] void SistemaTriangulo(
        const std::vector<std::shared_ptr<CE::Objeto>>& entes,
        float dt,
        float anchoVentana,
        float altoVentana);

    [[maybe_unused]] void SistemaCirculo(
        const std::vector<std::shared_ptr<CE::Objeto>>& entes,
        float dt,
        float anchoVentana,
        float altoVentana);


}
