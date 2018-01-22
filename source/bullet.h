#pragma once

#include "haxxian_game_object.h"

class bullet : public haxxian_game_object
{
public:
  bullet();
  void update(float dt) override;

protected:
  sprite m_sp;
};

class player_bullet : public bullet
{
public:
  player_bullet();
};

