#include "base_ship.hpp"

void BaseShip::setColor () {
    float lifePercentage =  1.0f * m_currentLifePoints / m_baseLifePoints;

    if (lifePercentage > 0.75f) {
      m_color = BaseConstants::GREEN_COLOR;
    } else if (lifePercentage > 0.35f) {
      m_color = BaseConstants::YELLOW_COLOR;
    } else {
      m_color = BaseConstants::RED_COLOR;
    }
}