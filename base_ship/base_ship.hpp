#ifndef BASESHIP_HPP_
#define BASESHIP_HPP_

#include "abcg.hpp"
#include "./../data/gamedata.hpp"
#include "./../data/actiondata.hpp"
#include "./../data/typedata.hpp"
#include "./../base_constants.hpp"

class Bullets;
class OpenGLWindow;
class StarLayers;

class BaseShip {
 public:
  virtual void paintGL(const GameData &gameData) = 0;
  virtual void terminateGL() = 0;
  virtual void receiveDamage() = 0;
  void setColor ();
  
 protected:  
  friend Bullets;
  friend OpenGLWindow;
  friend StarLayers;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{1};
  float m_scale{0.150f};
  int m_baseLifePoints{};
  int m_currentLifePoints{};
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(0)};
  glm::vec2 m_forward{0.0f, 1.0f};

  abcg::ElapsedTimer m_bulletCoolDownTimer;
  ActionData m_actionData;
  TypeData m_typeData;
};

#endif