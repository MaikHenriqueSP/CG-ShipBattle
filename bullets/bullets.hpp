#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <list>

#include "abcg.hpp"
#include "./../data/gamedata.hpp"
#include "./../ship/ship.hpp"
#include "./../base_ship/base_ship.hpp"
#include "./../data/typedata.hpp"

class OpenGLWindow;

class Bullets {
  public:
    void initializeGL(GLuint program);
    void paintGL();
    void terminateGL();
    void update(BaseShip &ship, const GameData &gameData, float deltaTime);

  private:
    friend OpenGLWindow;

    GLuint m_program{};
    GLint m_colorLoc{};
    GLint m_rotationLoc{};
    GLint m_translationLoc{};
    GLint m_scaleLoc{};

    GLuint m_vao{};
    GLuint m_vbo{};

    struct Bullet {
      bool m_dead{false};
      glm::vec2 m_translation{glm::vec2(0)};
      glm::vec2 m_velocity{glm::vec2(0)};
      TypeData m_typeData{};
    };

    float m_scale{0.015f};
    
    std::list<Bullet> m_bullets;

    void verifyBulletOffScreen(Bullet &bullet);
    void removeDeadBullets();
    void createBullet(BaseShip &ship);
};

#endif