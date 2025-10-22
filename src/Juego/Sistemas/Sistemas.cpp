#include "Sistemas.hpp"
#include "../../Motor/Componentes/IComponentes.hpp"
#include "Juego/Figuras/Figuras.hpp"
#include <cmath>

namespace IVJ {

    void SistemaRectangulo(
        const std::vector<std::shared_ptr<CE::Objeto>>& entes,
        float dt,
        float altoVentana)
    {
        for (auto& e : entes)
        {
            if (auto rect = std::dynamic_pointer_cast<IVJ::Rectangulo>(e))
            {
                auto trans = e->getTransformada();
                trans->posicion.y += trans->velocidad.y * dt;

                if (trans->posicion.y < 0 || trans->posicion.y > altoVentana)
                {
                    trans->velocidad.y = -trans->velocidad.y;
                }
            }
        }
    }

    void SistemaTriangulo(
        const std::vector<std::shared_ptr<CE::Objeto>>& entes,
        float dt,
        float anchoVentana,
        float altoVentana)
    {
        for (auto& e : entes)
        {
            if (auto tri = std::dynamic_pointer_cast<IVJ::Triangulo>(e))
            {
                auto trans = e->getTransformada();

                // Movimiento horizontal constante
                trans->posicion.x += trans->velocidad.x * dt;

                // Movimiento vertical en forma de onda senoidal
                float amplitud = 100.f;        // Altura de la onda
                float frecuencia = 10.f;       // Número de oscilaciones
                trans->posicion.y = (altoVentana / 2.f) + amplitud * std::sin(trans->posicion.x * frecuencia * 0.01f);

                // Rebote en los bordes de la ventana
                if (trans->posicion.x < 0.f || trans->posicion.x > anchoVentana)
                {
                    trans->velocidad.x = -trans->velocidad.x; // invertir dirección
                }

            }
        }
    }

    void SistemaCirculo(
        const std::vector<std::shared_ptr<CE::Objeto>>& entes,
        float dt,
        float anchoVentana,
        float altoVentana)
    {
        for (auto& e : entes)
        {
            if (auto circ = std::dynamic_pointer_cast<IVJ::Circulo>(e))
            {
                auto trans = e->getTransformada();

                // Si aún no se inicializó el centro y radio, lo configuramos por defecto
                if (!trans->inicializado) {
                    trans->centroX = trans->posicion.x;
                    trans->centroY = trans->posicion.y;
                    trans->radio   = 50.f;
                    trans->angulo = 0.f;
                    trans->inicializado = true;
                }
                trans->angulo += trans->velocidad.x + dt;

                trans->posicion.x = trans->centroX + trans->radio * std::cos(trans->angulo);
                trans->posicion.y = trans->centroY + trans->radio * std::sin(trans->angulo);

                trans->centroX += trans->velocidad.y * dt;

                if (trans->centroX - trans->radio < 0.f || trans->centroX + trans->radio > anchoVentana) {
                    trans->velocidad.y = -trans->velocidad.y;
                }
            }
        }
    }


}
