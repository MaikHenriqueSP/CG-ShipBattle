#include "ship.hpp"
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Ship::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_baseLifePoints = LIFE_POINTS;
  m_currentLifePoints = m_baseLifePoints;
  m_translation = glm::vec2(0, VERTICAL_POSITION);
  m_velocity = glm::vec2(0);
  
  std::array<glm::vec2, 13> positions = BODY_POINTS;

  for (auto &position : positions) {
    position /= glm::vec2{NORMALIZE_FACTOR, NORMALIZE_FACTOR};
  }

  std::array indices = BODY_POINTS_INDICES;

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),  GL_STATIC_DRAW);
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

void Ship::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);
  glBindVertexArray(m_vao);
  glUniform1f(m_scaleLoc, m_scale);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);
  
  BaseShip::setColor();
  generateThrusterBlink();

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 10 * 3, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  glUseProgram(0);
}

void Ship::terminateGL()  {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void Ship::update(float deltaTime) { 
  if (m_actionData.m_input[static_cast<size_t>(Action::Left)] && m_translation.x > -1 + m_scale) {
    m_translation -= glm::vec2(1, 0) * deltaTime;
  }
    
  if (m_actionData.m_input[static_cast<size_t>(Action::MouseLeft)] && m_translation.x > -1 + m_scale && !isWithinMouseInterval()) {
    m_translation -= glm::vec2(1, 0) * deltaTime;
  } 

  if (m_actionData.m_input[static_cast<size_t>(Action::Right)] && m_translation.x < 1 - m_scale) {
    m_translation += glm::vec2(1, 0)  * deltaTime;
  }

  if (m_actionData.m_input[static_cast<size_t>(Action::MouseRight)] && m_translation.x < 1 - m_scale && !isWithinMouseInterval()) {
    m_translation += glm::vec2(1, 0) * deltaTime;
  } 
}

void Ship::receiveDamage() {
  m_currentLifePoints--;
}

bool Ship::isWithinMouseInterval() {
  float interval = glm::abs(m_lastMousePosition - m_translation.x );
  return interval < m_mouseMovementDelta;
}

void Ship::generateThrusterBlink() {
  if (m_trailBlinkTimer.elapsed() > 100.0 / 1000.0) m_trailBlinkTimer.restart();

  if ((m_actionData.m_input[static_cast<size_t>(Action::Left)] || m_actionData.m_input[static_cast<size_t>(Action::MouseLeft)] ||
    m_actionData.m_input[static_cast<size_t>(Action::Right)] || m_actionData.m_input[static_cast<size_t>(Action::MouseRight)]) && !isWithinMouseInterval()) {
    if (m_trailBlinkTimer.elapsed() < 50.0 / 1000.0) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glUniform4f(m_colorLoc, 1, 1, 1, 0.5f);
      glDrawElements(GL_TRIANGLES, 11 * 3, GL_UNSIGNED_INT, nullptr);

      glDisable(GL_BLEND);
    }
  }

}
