#include "Simulation.hpp"
#include <imgui.h>
#include <Motor/Render/Render.hpp>
#include <Motor/GUI/GLogger.hpp>
#include <iostream>

Simulation::Simulation(unsigned gridW, unsigned gridH)
: w(gridW), h(gridH), grid(w*h)
{
    std::random_device rd; rng.seed(rd());
}

void Simulation::init() {
    // inicializar personas con probabilidades random
    for (auto &p: grid) {
        p.state = PState::NORMAL;
        p.stateTimer = 0.f;
        p.partialImmunityTimer = 0.f;
        // randomize per-person probabilities
        p.pInfect = 0.15f + uni01(rng) * (0.30f - 0.15f);
        p.pImmune = 0.01f + uni01(rng) * (0.10f - 0.01f);
        p.pDeath  = 0.10f;
        p.permanentImmunity = false;
    }

    // Infectados ~10% inicial
    unsigned total = w * h;
    unsigned initial = std::max(1u, (unsigned)(total * 0.10f));
    for (unsigned k=0;k<initial;k++){
        unsigned idx = (unsigned)(uni01(rng) * total) % total;
        grid[idx].state = PState::ENFERMO;
        grid[idx].stateTimer = 0.f;
    }
}

void Simulation::update(float dt) {
    // primer pase: timer de progeso, se aplican infectados
    struct ToInfect { int i,j; };
    std::vector<ToInfect> willInfect;

    for (unsigned y=0;y<h;y++){
        for (unsigned x=0;x<w;x++){
            Person &p = at(x,y);

            // contador de inmunidad parcial
            if (p.partialImmunityTimer > 0.f) {
                p.partialImmunityTimer -= dt;
                if (p.partialImmunityTimer <= 0.f) {
                    p.partialImmunityTimer = 0.f;
                    // returns a estado normal
                    if (p.state == PState::NORMAL)
                        ;
                }
            }

            // logica de estado
            switch (p.state) {
                case PState::ENFERMO:
                {
                    // cada segundo de los 3 que tiene, probabilididad de contagiar a un vecino

                    p.stateTimer += dt;

                    // attempt contagion to 4-neighbors
                    const int dx[4] = {1,-1,0,0};
                    const int dy[4] = {0,0,1,-1};
                    for (int k=0;k<4;k++){
                        int nx = (int)x + dx[k];
                        int ny = (int)y + dy[k];
                        if (nx<0||nx>=(int)w||ny<0||ny>=(int)h) continue;
                        Person &other = at(nx,ny);
                        if (other.state==PState::NORMAL && other.partialImmunityTimer<=0.f && !other.permanentImmunity) {
                            // probabilidad: p.pInfect cada segundo -> escala por dt
                            float p_chance = p.pInfect * dt;
                            if (uni01(rng) < p_chance) {
                                willInfect.push_back({nx,ny});
                            }
                        }
                    }

                    if (p.stateTimer >= T_ENFERMO) {
                        p.state = PState::RECUPERAR;
                        p.stateTimer = 0.f;
                    }
                    break;
                }
                case PState::RECUPERAR:
                    p.stateTimer += dt;
                    if (p.stateTimer >= T_RECUP) {
                        // decide death or recovery
                        if (uni01(rng) < p.pDeath) {
                            p.state = PState::MUERTO;
                        } else {
                            // recuperado-inmunidad
                            if (uni01(rng) < p.pImmune) {
                                p.state = PState::INMUNE;
                                p.permanentImmunity = true;
                            } else {
                                // partial immunity for 2 seconds then NORMAL
                                p.state = PState::NORMAL;
                                p.partialImmunityTimer = T_PARTIAL_IMM;
                            }
                        }
                        p.stateTimer = 0.f;
                    }
                    break;
                case PState::INMUNE:
                    // permanente; nada que hacer
                    break;
                case PState::NORMAL:
                case PState::MUERTO:

                    break;
            }
        }
    }

    // se aplica infeccion (los hace enefermos y se les reinicia el timer)
    for (auto &t : willInfect) {
        Person &q = at(t.i,t.j);
        if (q.state==PState::NORMAL && q.partialImmunityTimer<=0.f && !q.permanentImmunity) {
            q.state = PState::ENFERMO;
            q.stateTimer = 0.f;
        }
    }
}

void Simulation::render(float x0, float y0, float cellSize) {
    // draw each cell as rectangle
    for (unsigned y=0;y<h;y++){
        for (unsigned x=0;x<w;x++){
            const Person &p = atc(x,y);
            sf::RectangleShape rect(sf::Vector2f(cellSize-2.f, cellSize-2.f));
            rect.setPosition(sf::Vector2f(x0 + x * cellSize + 1.f, y0 + y * cellSize + 1.f));
            sf::Color fill = sf::Color::Transparent;
            sf::Color outline = sf::Color(30,30,30);
            switch (p.state) {
                case PState::NORMAL:
                    if (p.partialImmunityTimer>0.f)
                        fill = sf::Color(150,200,255); //azul-parcialmente inmune
                    else
                        fill = sf::Color::Transparent;
                    outline = sf::Color(20,20,20);
                    break;
                case PState::ENFERMO:
                    fill = sf::Color(220,40,40);
                    outline = sf::Color(180,20,20);
                    break;
                case PState::RECUPERAR:
                    fill = sf::Color(100,100,220);
                    outline = sf::Color(60,60,160);
                    break;
                case PState::INMUNE:
                    fill = sf::Color(80,200,120);
                    outline = sf::Color(40,160,60);
                    break;
                case PState::MUERTO:
                    fill = sf::Color(40,40,40);
                    outline = sf::Color(10,10,10);
                    break;
            }
            rect.setFillColor(fill);
            rect.setOutlineColor(outline);
            rect.setOutlineThickness(1.f);
            CE::Render::Get().AddToDraw(rect);
        }
    }
}

bool Simulation::hasInfectious() const {
    for (auto &p: grid) if (p.state==PState::ENFERMO) return true;
    return false;
}

void Simulation::countStates(int &normal, int &enfermo, int &recuperar, int &inmune, int &muerto) const {
    normal = enfermo = recuperar = inmune = muerto = 0;
    for (auto &p: grid) {
        switch (p.state) {
            case PState::NORMAL: normal++; break;
            case PState::ENFERMO: enfermo++; break;
            case PState::RECUPERAR: recuperar++; break;
            case PState::INMUNE: inmune++; break;
            case PState::MUERTO: muerto++; break;
        }
    }
}

void Simulation::tryToggleAt(int mx, int my, float x0, float y0, float cellSize) {
    int i = (int)std::floor((mx - x0) / cellSize);
    int j = (int)std::floor((my - y0) / cellSize);
    if (i<0||i>=(int)w||j<0||j>=(int)h) return;
    Person &p = at(i,j);
    // toggle through states for debugging
    if (p.state==PState::NORMAL) p.state = PState::ENFERMO;
    else if (p.state==PState::ENFERMO) p.state = PState::RECUPERAR;
    else if (p.state==PState::RECUPERAR) p.state = PState::INMUNE;
    else if (p.state==PState::INMUNE) p.state = PState::MUERTO;
    else p.state = PState::NORMAL;
}

