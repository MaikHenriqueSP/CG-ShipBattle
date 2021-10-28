#include "enemy_ship.hpp"
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void EnemyShip::initializeGL(GLuint program, float verticalPosition) {
  terminateGL();
  
  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_forward = glm::vec2(0.0f, -1.0f);
  m_scale = 0.10f;

  float horizontalPosition = Util::getRandomNumber(BaseConstants::SCREEN_LEFT_LIMIT + m_scale, BaseConstants::SCREEN_RIGHT_LIMIT - m_scale, true);
  m_translation = glm::vec2{horizontalPosition, verticalPosition}; 

  float horizontalVelocity = Util::getRandomNumber(MIN_VELOCITY, MAX_VELOCITY, true);
  m_velocity = glm::vec2{horizontalVelocity, 0.0f};

  m_baseLifePoints = LIFE_POINTS;
  m_currentLifePoints = m_baseLifePoints;
  m_typeData.m_type = SHIP_TYPE;

  std::array<glm::vec2, 7> positions = BODY_POINTS;

  for (auto &position :positions) {
    position /= glm::vec2{-NORMALIZE_FACTOR, -NORMALIZE_FACTOR};
  }

  std::array indices = BODY_POINTS_INDICES;

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  glBindVertexArray(0);
}

void EnemyShip::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  BaseShip::setColor();

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);

  glUseProgram(0);
}

void EnemyShip::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void EnemyShip::update(float deltaTime) {
    updatePosition(deltaTime);
    updateShooting();
}

void EnemyShip::updatePosition(float deltaTime) {
  bool isInInvertRegion = 1 - glm::abs(m_translation.x) < m_invertDelta;

  if (!m_inInvertRegionFlag && isInInvertRegion) {
    m_velocity *= -1;
    m_inInvertRegionFlag = true;
  } else if (!isInInvertRegion) {
    m_inInvertRegionFlag = false;
  }

  m_translation += m_velocity * deltaTime;
}

void EnemyShip::updateShooting() {
  if (m_bulletCoolDownTimer.elapsed() > 200.0 / 1000.0) {
    std::uniform_real_distribution<float> m_randomShoot{0.0f, 1.0f};    
    if (Util::getRandomNumber(0.0f, 1.0f, false) > 0.85f) {
      m_actionData.m_input.set(static_cast<size_t>(Action::Fire));
    } else {
      m_bulletCoolDownTimer.restart();
      m_actionData.m_input.reset(static_cast<size_t>(Action::Fire));
    }
  }
}

void EnemyShip::receiveDamage() {
  m_currentLifePoints--;
}