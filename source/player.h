#pragma once

#include "haxxian_game_object.h"

class player : public haxxian_game_object
{
public:
  player();
  void update(float dt) override;
  
  void on_input(int input);

  void explode();
  bool is_exploding() const;

private:
  sprite m_ship_sprite;
  sprite m_explosion_sprite;

  // If > 0, this is the time until a bullet is ready at the tip of the
  //  ship.
  float m_bullet_time = 0;

  // If exploding, this is the time until we are dead.
  float m_explosion_time = 0;

  enum class player_mode
  {
    OK,
    EXPLODING
  };

  player_mode m_mode = player_mode::OK;
};


