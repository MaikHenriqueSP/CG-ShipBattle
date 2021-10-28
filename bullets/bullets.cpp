#include "bullets.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Bullets::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_bullets.clear();

  auto sides{10};
  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  auto step{M_PI * 2 / sides};
  for (auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));


  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
               positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};


  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void Bullets::paintGL() {
  glUseProgram(m_program);

  glBindVertexArray(m_vao);
  glUniform4f(m_colorLoc, 1, 1, 1, 1);
  glUniform1f(m_scaleLoc, m_scale);

  for (auto &bullet : m_bullets) {
    glUniform2f(m_translationLoc, bullet.m_translation.x, bullet.m_translation.y);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
  }

  glBindVertexArray(0);
  glUseProgram(0);
}

void Bullets::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

void Bullets::update(BaseShip &ship, const GameData &gameData, float deltaTime) {
  if (ship.m_actionData.m_input[static_cast<size_t>(Action::Fire)] && gameData.m_state == State::Playing && ship.m_bulletCoolDownTimer.elapsed() > 250.0 / 1000.0) { 
    createBullet(ship); 
  }

  for (auto &bullet : m_bullets) {
    bullet.m_translation += bullet.m_velocity * deltaTime;
    verifyBulletOffScreen(bullet);
  }

  removeDeadBullets();
}

void Bullets::verifyBulletOffScreen(Bullet &bullet) { 
  if (bullet.m_translation.x < -1.1f) bullet.m_dead = true;
  if (bullet.m_translation.x > +1.1f) bullet.m_dead = true;
  if (bullet.m_translation.y < -1.1f) bullet.m_dead = true;
  if (bullet.m_translation.y > +1.1f) bullet.m_dead = true; 
}

void Bullets::removeDeadBullets() {
  m_bullets.remove_if([](const Bullet &p) { return p.m_dead; });
}

void Bullets::createBullet(BaseShip &ship) {
  ship.m_bulletCoolDownTimer.restart();

  auto bulletSpeed{2.0f};

  Bullet bullet{.m_dead = false,
                .m_translation = ship.m_translation,
                .m_velocity = ship.m_forward * bulletSpeed,
                .m_typeData = ship.m_typeData};
                
  m_bullets.push_back(bullet);
}