#include "bullet.h"
#include "directory.h"
#include "globals.h"
#include "screen.h"

const float OFF_TOP_OF_SCREEN = -10.f;

bullet::bullet()
{
  m_sp.load(get_data_dir() + "bullet.png", the_global_palette);
  m_sp.set_num_cells(2, 1);
  m_sp.set_cell_range(0, 0);
  m_sprite = &m_sp;
}

void bullet::update(float dt)
{
  haxxian_game_object::update(dt);

  if (m_pos.y > screen::HEIGHT || m_pos.y < OFF_TOP_OF_SCREEN)
  {
    dead();
  }
}

player_bullet::player_bullet()
{
  m_sprite->set_cell_range(1, 1);
}


