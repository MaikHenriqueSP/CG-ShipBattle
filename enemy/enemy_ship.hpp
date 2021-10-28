#ifndef ENEMY_HPP_
#define ENEMY_HPP_

#include <random>
#include "abcg.hpp"
#include "./../data/gamedata.hpp"
#include "./../base_ship/base_ship.hpp"
#include "./../util/util.hpp"
#include "./../base_constants.hpp"

class EnemyShip : public BaseShip {
  const int LIFE_POINTS = 3;
  const float MIN_VELOCITY = 0.5f;
  const float MAX_VELOCITY = 1.0f;
  const float NORMALIZE_FACTOR = 10.0f;
  const Type SHIP_TYPE = Type::Enemy;
  const std::array<glm::vec2, 7> BODY_POINTS {
    glm::vec2{-10.0f, -04.0f}, 
    glm::vec2{+10.0f, +02.0f},
    glm::vec2{-04.0f, +08.0f}, 
    glm::vec2{+04.0f, +08.0f},
    glm::vec2{-10.0f, +02.0f}, 
    glm::vec2{+10.0f, -04.0f},
    glm::vec2{+00.0f, -04.0f} 
  };
  const std::array<int, 18> BODY_POINTS_INDICES {
    0, 1, 5,
    5, 4, 0,
    1, 2, 4,
    4, 3, 1,
    2, 3, 6,
    1, 4, 5  
  };

  public:
    void paintGL(const GameData &gameData) override;
    void terminateGL() override;
    void receiveDamage() override;

    void initializeGL(GLuint program, float verticalPosition);
    void update(float deltaTime);

  private:
    abcg::ElapsedTimer m_bulletCoolDownTimer;
    std::array<glm::vec2, 7> getBodyPositions();
    std::array<int, 18> getBodyIndices();
    float m_invertDelta{0.10f};
    bool m_inInvertRegionFlag{false};

    void updatePosition(float deltaTime);
    void updateShooting();
    float getRandomNumber(float start, float end, bool isNegativeAcceptable);
};

#endif