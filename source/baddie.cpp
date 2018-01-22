#include "baddie.h"
#include "directory.h"
#include "globals.h"
#include "play_state.h"
#include "screen.h"

const vec2 BULLET_VEL(0, 10.f);
const float BULLET_PROBABILITY = 0.01f;
const float CHARGE_DOWN_SPEED = 40.f;
const float MIN_CHARGE_TIME = 2.f;
const float CHARGE_PROBABILITY = 0.001f;
const float CHARGE_ACC_MULT = 40.f;
const float CHARGE_PERIOD = 2.f;
const float CONVOY_ANIM_TIME = 0.6f;
const float CHARGE_ANIM_TIME = 0.4f;
const float EXPLOSION_ANIM_TIME = 0.15f;
const float OFF_BOTTOM_OF_SCREEN = 10.f; // added to screen::H
const float OFF_TOP_OF_SCREEN = -20.f;
const float CONVOY_DRIFT_SPEED = 3.f;
const float REJOIN_TIME = 2.f;
const float EXPLODE_TIME = 0.8f;

float baddie::s_convoy_x = 0;

bool baddie::s_can_charge = true;

void baddie::set_can_charge(bool can_charge)
{
  s_can_charge = can_charge;
}

void baddie::calc_convoy_x(float dt)
{
  static float t = 0;
  t += dt;
  // Calc vel
  float convoy_vel_x = sin(t * 0.5f);

  // Apply vel to position
  static float convoy_x = 0;
  convoy_x += dt * convoy_vel_x * CONVOY_DRIFT_SPEED;

  s_convoy_x = convoy_x;
}

void baddie::set_convoy_pos(const vec2 cp)
{
  m_convoy_pos = cp;
}

baddie::baddie()
{
  m_sprite = &m_convoy_sprite;

  m_convoy_sprite.load(get_data_dir() + "green_convoy.png", the_global_palette);
  m_convoy_sprite.set_num_cells(3, 1);
  m_convoy_sprite.set_cell_range(0, 2);
  m_convoy_sprite.set_cell_time(CONVOY_ANIM_TIME);

  m_charge_sprite.load(get_data_dir() + "green_charge.png", the_global_palette);
  m_charge_sprite.set_num_cells(8, 2);
  m_charge_sprite.set_cell_range(0, 7);
  m_charge_sprite.set_cell_time(CHARGE_ANIM_TIME);

  m_explosion_sprite.load(get_data_dir() + "explosion.png", the_global_palette);
  m_explosion_sprite.set_num_cells(4, 1);
  m_explosion_sprite.set_cell_range(0, 3);
  m_explosion_sprite.set_cell_time(EXPLOSION_ANIM_TIME);
}

float rnd(float f)
{
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * f;
}

float rnd(float rmin, float rmax)
{
  return rmin + rnd(rmax - rmin);
}

int baddie::get_score() const
{
  int sc = 30;
  if (m_mode == mode::CHARGING)
  {
    sc *= 2;
  }
  return sc;
}

bool baddie::is_exploding() const
{
  return m_mode == mode::EXPLODING;
}

void baddie::explode()
{
  set_vel(vec2(0, 0));
  m_mode = mode::EXPLODING;
  m_explode_time = EXPLODE_TIME;
  m_sprite = &m_explosion_sprite;
}

void baddie::set_up_charge()
{
  m_convoy_time = 0;
  m_mode = mode::CHARGING;
  m_charge_down_speed = CHARGE_DOWN_SPEED * rnd(.5f, 2.f);
  m_charge_side_mult = CHARGE_ACC_MULT * rnd(.5f, 2.f);
  m_charge_period = CHARGE_PERIOD * rnd(.5f, 2.f);
  if (rnd(-1, 1) < 0) 
  {
    m_charge_period *= -1.f;
  }
 
  m_sprite = &m_charge_sprite;
}

void baddie::set_up_rejoin_convoy()
{
  // Adjust vel so we head towards our convoy pos
  m_mode = mode::REJOINING_CONVOY;

  m_pos.y = OFF_TOP_OF_SCREEN;
  float dy = m_convoy_pos.y - m_pos.y;
  float dx = m_convoy_pos.x + s_convoy_x - m_pos.x;

  // Time to reach convoy pos
  m_rejoin_time = REJOIN_TIME;
  
  // Set the rough velocity - we will need to adjust though 
  m_acc = vec2(0, 0);
  m_vel = vec2(dx / m_rejoin_time, dy / m_rejoin_time); 
}

void baddie::update(float dt)
{
  switch (m_mode)
  {
  case mode::IN_CONVOY:
  
    m_pos = m_convoy_pos + vec2(s_convoy_x, 0);

    // Time to charge?
    m_convoy_time += dt;
    if (   s_can_charge
        && m_convoy_time > MIN_CHARGE_TIME 
        && rnd(1.f) < CHARGE_PROBABILITY)
    {
      set_up_charge();
    }
    break;

  case mode::CHARGING: 
  
    // Go down. Also curve left/right
    m_charge_time += dt;
    m_vel = vec2(cos(m_charge_time * m_charge_period) * m_charge_side_mult, m_charge_down_speed);

    // Off bottom of screen? If so, go to off top of screen and head towards convoy pos
    if (m_pos.y > (screen::HEIGHT + OFF_BOTTOM_OF_SCREEN))
    {
      set_up_rejoin_convoy();
    }
    else
    {
      // Within some y range?
      if (rnd(1.f) < BULLET_PROBABILITY)
      {
        bullet* b = new bullet;
        b->set_pos(get_pos());
        b->set_vel(BULLET_VEL + vec2(0, m_vel.y));
        the_play_state->add_bullet(b);
      }
    }
    break;
  
  case mode::REJOINING_CONVOY:
    // Move closer to convoy pos 
    m_rejoin_time -= dt;
    if (m_rejoin_time <= 0) 
    {
      m_vel = vec2(0, 0);
      m_mode = mode::IN_CONVOY;
      m_sprite = &m_convoy_sprite;
    }
    else
    {
      float dy = m_convoy_pos.y - m_pos.y;
      float dx = m_convoy_pos.x + s_convoy_x - m_pos.x;
      m_vel = vec2(dx / m_rejoin_time, dy / m_rejoin_time);
    }
    break;

  case mode::EXPLODING:
    m_explode_time -= dt;
    if (m_explode_time < 0)
    {
      dead();
    }
  }

  haxxian_game_object::update(dt);
}

