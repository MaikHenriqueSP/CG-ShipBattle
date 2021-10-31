#include "util.hpp"

float Util::getRandomNumber(float start, float end, bool isNegativeAcceptable) {
  std::uniform_real_distribution<float> m_randomNumber{start, end};
  std::default_random_engine m_randomEngine;
  
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  float result = m_randomNumber(m_randomEngine);

  if (isNegativeAcceptable) {
    std::uniform_real_distribution<float> m_randomNegative{0, 1};
    float rndNumber = m_randomNegative(m_randomEngine);
    int multiplier = rndNumber > 0.5f ? 1 : -1;
    result *= multiplier; 
  }

  return result;
}
