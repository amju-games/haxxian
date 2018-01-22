#pragma once

#include "haxxian_game_object.h"

class baddie : public haxxian_game_object
{
public:
  baddie();  

  void update(float dt) override;

  // Call to calc sideways drift for the whole convoy
  static void calc_convoy_x(float dt);

  // Set original position in convoy assuming no offset
  void set_convoy_pos(const vec2 cp);

  void explode();
  bool is_exploding() const;

  // Get score for killing this baddie
  int get_score() const;

  // So we can turn off charging when player killed
  static void set_can_charge(bool can_charge);

protected:
  void set_up_charge();
  void set_up_rejoin_convoy();

protected:

  enum class mode
  {
    IN_CONVOY,
    CHARGING,
    REJOINING_CONVOY,
    EXPLODING
  };

  mode m_mode = mode::IN_CONVOY;

  // Time waiting in convoy
  float m_convoy_time = 0.f;
  float m_charge_time = 0.f;
  float m_explode_time = 0.f;

  // When rejoining, time until we are back in convoy
  float m_rejoin_time = 0.f;

  float m_charge_down_speed = 0;
  float m_charge_side_mult = 0;
  float m_charge_period = 0;

  // Position in convoy, ignoring drift, to which we return after a charge.
  vec2 m_convoy_pos;

  sprite m_convoy_sprite;
  sprite m_charge_sprite;
  sprite m_explosion_sprite;

  // X-offset for whole convoy (it drifts left-right)
  static float s_convoy_x;

  // Can we charge?
  static bool s_can_charge;
};

