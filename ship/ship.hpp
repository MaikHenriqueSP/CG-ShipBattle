#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "./../data/gamedata.hpp"
#include "./../base_ship/base_ship.hpp"
#include "./../base_constants.hpp"


class Ship : public BaseShip {
  const int LIFE_POINTS = 3;
  const float NORMALIZE_FACTOR = 15.5f;
  const float VERTICAL_POSITION = -0.82f;
  const std::array<glm::vec2, 13> BODY_POINTS {
    glm::vec2{-15.5f, -03.5f},
    glm::vec2{-15.5f, +02.0f},
    glm::vec2{-12.0f, -02.0f},
    glm::vec2{-04.0f, +00.0f},
    glm::vec2{-00.0f, +15.5f}, 
    glm::vec2{+04.0f, +00.0f},
    glm::vec2{+12.0f, -02.0f}, 
    glm::vec2{+15.5f, +02.0f},
    glm::vec2{+15.5f, -03.5f}, 
    glm::vec2{+0.00f, -03.5f},
    glm::vec2{-03.0f, -03.5f}, 
    glm::vec2{+3.00f, -03.5f},
    glm::vec2{+00.0f, -08.0f}
  };
  const std::array<int, 33> BODY_POINTS_INDICES{0, 1, 2,
                    0, 2, 8,
                    2, 3, 6,
                    3, 4, 5,
                    5, 6, 2,
                    6, 7, 8,
                    6, 8, 0,
                    3, 5, 9,
                    9, 2, 5,
                    9, 6, 3,
                    // Thruster trail
                    10, 11, 12
  };

  public:
    void initializeGL(GLuint program);
    void paintGL(const GameData &gameData) override;
    void terminateGL() override;
    void update(float deltaTime);
    void receiveDamage() override;

    float m_lastMousePosition{0};
    float m_mouseMovementDelta{0.01f};
    abcg::ElapsedTimer m_trailBlinkTimer;

  private:
    bool isWithinMouseInterval();
    void generateThrusterBlink();    
};

#endif