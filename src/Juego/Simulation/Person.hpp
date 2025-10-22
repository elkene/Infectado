#pragma once
#include <SFML/Graphics.hpp>
#include <random>


enum class PState { NORMAL, ENFERMO, RECUPERAR, INMUNE, MUERTO};


struct Person {
    PState state = PState::NORMAL;
    float stateTimer = 0.f; // tiempo dentro del estado
    float partialImmunityTimer = 0.f; // tiempo de inmunidad parcial

    float pInfect = 0.20f; // probabilidad individual de infectar (0.15-0.30)
    float pImmune = 0.03f; // probabilidad de inmunidad permanente (0.01-0.10)
    float pDeath = 0.10f; // probabilidad de morir al terminar recuperacion


    bool permanentImmunity = false;


    // utilities
    inline bool isSusceptible() const {
        return state == PState::NORMAL || (state == PState::NORMAL && partialImmunityTimer>0.f);
    }
    inline bool isInfectious() const { return state == PState::ENFERMO; }
};