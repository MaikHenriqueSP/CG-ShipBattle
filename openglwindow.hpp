#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "./bullets/bullets.hpp"
#include "./ship/ship.hpp"
#include "./enemy/enemy_ship.hpp"

const float ENEMY_SHIP_START_POSITION = 0.90f;
const float ENEMY_SHIP_VERTICAL_DELTA = 0.20f;
const int ENEMY_SHIP_QUANTITY = 3;

class OpenGLWindow : public abcg::OpenGLWindow {

 protected:
  void resizeGL(int width, int height) override;
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void terminateGL() override;
  void paintGL() override;
  void paintUI() override;

 private:
  GLuint m_objectsProgram{};
  int m_lastmousepos{300};
  int m_viewportHeight{};
  int m_viewportWidth{};

  abcg::ElapsedTimer m_restartWaitTimer;
  std::list<EnemyShip> m_enemies;
  GameData m_gameData;
  Bullets m_bullets;
  ImFont* m_font{};
  Ship m_ship;


  void handleMouseButtonDown(SDL_Keycode keyCode);
  void handleMouseButtonUp(SDL_Keycode keyCode);
  void handleKeyDown(SDL_Keycode keyCode);
  void handleKeyUp(SDL_Keycode keyCode);
  void handleMouseMotion();
  void verifyEnemyLife();
  void verifyGameOver();
  void verifyShot();
  void verifyWin();
  void restart();
  void update();
};

#endif