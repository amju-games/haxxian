#include "haxxian_game_object.h"

bool sprite_collision(
  haxxian_game_object* jgo1, haxxian_game_object* jgo2)
{
  pix_int_result r = pixel_intersect(
    *jgo1->m_sprite, jgo1->m_sprite->get_cell(), jgo1->m_pos.x, jgo1->m_pos.y,
    *jgo2->m_sprite, jgo2->m_sprite->get_cell(), jgo2->m_pos.x, jgo2->m_pos.y);

  return (r == pix_int_result::YES_COLLIDE);
}

void haxxian_game_object::draw(screen& dest)
{
  m_sprite->draw(dest, m_pos.x, m_pos.y);
}

void haxxian_game_object::update(float dt) 
{
  game_object::update(dt);

  m_sprite->update(dt);
}

