#pragma once

#include "game_object.h"
#include "screen.h"
#include "sprite.h"

class haxxian_game_object : public game_object
{
public:
  virtual void draw(screen& scr);
  void update(float dt) override; 

  void dead() { m_is_alive = false; }
  bool is_dead() const { return !m_is_alive; }

  friend bool sprite_collision(
    haxxian_game_object* jgo1, haxxian_game_object* jgo2); 

protected:
  sprite* m_sprite = nullptr;
  bool m_is_alive = true; // ?
};

