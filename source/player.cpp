#include <algorithm>
#include "directory.h"
#include "globals.h"
#include "input.h"
#include "player.h"
#include "screen.h"

namespace
{
const float BULLET_SPEED = -200.f;
const float PLAYER_SPEED = 30.f;
const float BULLET_WAIT_TIME = 0.2f;
const float MIN_X = -4.f;
const float MAX_X = 120.f;
const float EXPLOSION_ANIM_TIME = 0.15f;
const float EXPLODE_TIME = 2.f;
}

player::player()
{
  m_sprite = &m_ship_sprite;
  m_ship_sprite.load(get_data_dir() + "ship.png", the_global_palette);
  m_ship_sprite.set_num_cells(2, 1);
  m_ship_sprite.set_cell_range(0, 0);

  m_explosion_sprite.load(get_data_dir() + "player_explosion.png", the_global_palette);
  m_explosion_sprite.set_num_cells(4, 1);
  m_explosion_sprite.set_cell_range(0, 3);
  m_explosion_sprite.set_cell_time(EXPLOSION_ANIM_TIME);

  set_pos(vec2(60, screen::HEIGHT - 40));
}

bool player::is_exploding() const
{
  return m_mode == player_mode::EXPLODING;
}

void player::explode()
{
  set_vel(vec2(0, 0));
  m_mode = player_mode::EXPLODING;
  m_explosion_time = EXPLODE_TIME;
  m_sprite = &m_explosion_sprite;
}

void player::update(float dt)
{
  haxxian_game_object::update(dt);

  vec2 pos = get_pos();
  pos.x = std::min(pos.x, MAX_X);
  pos.x = std::max(pos.x, MIN_X);
  set_pos(pos);

  switch (m_mode)
  {
  case player_mode::OK:

    if (m_bullet_time > 0)
    {
      m_bullet_time -= dt;
      if (m_bullet_time <= 0)
      {
        m_bullet_time = 0;
        // Switch to sprite with bullet
        m_ship_sprite.set_cell_range(0, 0);
      }
    }
    break;

  case player_mode::EXPLODING:
    m_explosion_time -= dt;
    if (m_explosion_time < 0)
    {
      dead();
    }
  }
}

void player::on_input(int input)
{
  if (m_mode != player_mode::OK)
  {
    return;
  }

  if (input & BUTTON_A)
  {
    if (m_bullet_time <= 0)
    {
      // Can fire
      player_bullet* b = new player_bullet;
      b->set_pos(get_pos() + vec2(6, 0));
      b->set_vel(vec2(0, BULLET_SPEED));
      the_play_state->add_player_bullet(b);

      m_bullet_time = BULLET_WAIT_TIME;
      // Switch to sprite with no bullet
      m_ship_sprite.set_cell_range(1, 1);
    }
    return;
  } 

  float dir = 0;
  if (input & MOVE_LEFT)
  {
    dir -= 1.f;
  }
  if (input & MOVE_RIGHT)
  {
    dir += 1.f;
  }
  set_vel(vec2(PLAYER_SPEED * dir, 0));

}

