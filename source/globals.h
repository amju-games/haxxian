#pragma once

#include <memory>
#include "sound_player.h"
#include "font.h"
#include "game.h"
#include "palette.h"
#include "play_state.h"
#include "screen.h"
#include "timer.h"

extern std::unique_ptr<sound_player> the_sound_player;
extern font the_font;
extern game the_game;
extern timer the_timer;
extern font the_font;
extern palette the_global_palette;
extern std::unique_ptr<play_state> the_play_state;
extern screen the_screen;
extern timer the_timer;

