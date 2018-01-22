#include <algorithm>
#include <iostream>
#include "directory.h"
#include "globals.h"
#include "player.h"
#include "play_state.h"

namespace
{
const float CONVOY_X_OFFSET = 16.f;
const float NEW_LIFE_WAIT_TIME = 3.f;
const float NEW_LEVEL_WAIT_TIME = 4.f;
}

play_state::play_state()
{
  m_flag.load(get_data_dir() + "flag.png", the_global_palette);
  m_flag.set_num_cells(1, 1);
}

void play_state::player_dead()
{
  std::cout << "PLAYER DEAD!!!!!111\n";
  m_player->explode();
  baddie::set_can_charge(false);
  m_wait_time = NEW_LIFE_WAIT_TIME;
}

void play_state::update(float dt) 
{
  // Use elapsed time to calc sideways vel for baddies in convoy
  baddie::calc_convoy_x(dt);

  // Update baddies, and check if any baddies have hit the player
  for (auto& bad : m_baddies)
  {
    bad->update(dt);
 
    if (   !bad->is_exploding() 
        && !m_player->is_exploding()
        && sprite_collision(bad.get(), m_player.get()))
    {
      player_dead();
      bad->explode();
    }
  }

  // Remove dead baddies
  m_baddies.erase(
    std::remove_if(
      m_baddies.begin(), 
      m_baddies.end(), 
      [](const std::unique_ptr<baddie>& b) { return b->is_dead(); }), 
    m_baddies.end()
  );

  // Update bullets, check if any have hit player
  for (auto& b : m_bullets)
  {
    b->update(dt);
 
    if (   !m_player->is_exploding()
        && sprite_collision(b.get(), m_player.get()))
    {
      player_dead();
      b->dead();
    }
  }

  // Remove dead bullets
  m_bullets.erase(
    std::remove_if(
      m_bullets.begin(), 
      m_bullets.end(), 
      [](const std::unique_ptr<bullet>& b) { return b->is_dead(); }), 
    m_bullets.end()
  );

  // Check player bullets against baddies
  for (auto& b : m_player_bullets)
  {
    b->update(dt);

    // Check against baddies
    for (auto& bad : m_baddies)
    {
      if (   !bad->is_exploding()
          && sprite_collision(b.get(), bad.get()))
      {
        b->dead();
        bad->explode();
        m_score += bad->get_score();
  
        if (m_baddies.empty())
        {
          // Completed level!
          m_wait_time = NEW_LEVEL_WAIT_TIME; 
        }
      }
    }
  }

  // Remove dead bullets
  m_player_bullets.erase(
    std::remove_if(
      m_player_bullets.begin(), 
      m_player_bullets.end(), 
      [](const std::unique_ptr<player_bullet>& b) { return b->is_dead(); }), 
    m_player_bullets.end()
  );

  if (m_player->is_dead())
  {
    m_wait_time -= dt;
    if (m_wait_time < 0)
    {
      // Game over or new life?
      new_life();
    }
  }
  else
  {
    m_player->update(dt);
  }

  if (m_baddies.empty())
  {
    m_wait_time -= dt;
    if (m_wait_time < 0)
    {
      // Completed level!
      new_level();
    }
  }
}

void play_state::draw() 
{
  the_font.draw(the_screen, 2, 2, "SCORE: " + std::to_string(m_score));

  for (auto& b : m_player_bullets)
  {
    b->draw(the_screen);
  }

  for (auto& b : m_baddies)
  {
    b->draw(the_screen);
  }

  for (auto& b : m_bullets)
  {
    b->draw(the_screen);
  }

  if (!m_player->is_dead())
  {
    m_player->draw(the_screen);
  }

  for (int i = 0; i < m_level; i++)
  {
    m_flag.draw_cell(the_screen, 0, 
      screen::WIDTH - 20 - i * 10,
      screen::HEIGHT - 20);
  }
}

void play_state::on_input(int input) 
{
  m_player->on_input(input);
}

void play_state::add_player_bullet(player_bullet* b)
{
  m_player_bullets.push_back(std::unique_ptr<player_bullet>(b));
}

void play_state::add_bullet(bullet* b)
{
  m_bullets.push_back(std::unique_ptr<bullet>(b));
}

void play_state::new_level()
{
  m_level++;

  // TODO populate with a timer
  for (int i = 0; i < 7; i++)
  {
    const float W = 14.f;
    const float H = 9.f;
    const float BOTTOM = 60.f;

    // Cyan fodder baddies at bottom
    for (int j = 0; j < 3; j++)
    {
      baddie* b = new baddie;
      b->set_convoy_pos(vec2(i * W + CONVOY_X_OFFSET, BOTTOM - j * H));
      m_baddies.push_back(std::unique_ptr<baddie>(b));
    }
  }
}

void play_state::new_life()
{
  m_player.reset(new player);
  baddie::set_can_charge(true);
}

void play_state::on_active() 
{
  m_score = 0;
  m_lives = 3;
  m_level = 0;
  new_life();
  new_level();
}  

void play_state::on_deactive() 
{
}

