#pragma once

#include <memory.h>
#include <vector>
#include "baddie.h"
#include "bullet.h"
#include "game_state.h"
#include "player.h"

class play_state : public game_state
{
public:
  play_state();

  void update(float dt) override;
  void draw() override;
  void on_input(int input) override;
  void on_active() override;
  void on_deactive() override;

  void add_bullet(bullet* b);
  void add_player_bullet(player_bullet* b);
  
  void new_life();
  void new_level();

private:
  void player_dead();

private:
  // When waiting for new life or new level
  float m_wait_time = 0;

  std::vector<std::unique_ptr<baddie>> m_baddies;
  std::unique_ptr<player> m_player;
  std::vector<std::unique_ptr<bullet>> m_bullets;
  std::vector<std::unique_ptr<player_bullet>> m_player_bullets;

  int m_score = 0;
  int m_lives = 3;
  int m_level = 0;

  sprite_sheet m_flag;
};

