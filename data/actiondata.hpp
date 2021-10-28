#ifndef ACTIONDATA_HPP_
#define ACTIONTDATA_HPP_

#include <bitset>

enum class Action { Right, Left, Fire, MouseLeft, MouseRight };

struct ActionData {
  std::bitset<5> m_input;  
};

#endif