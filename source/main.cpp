// Hack Day Game Prototype
// (c) Jason Colman Jan 2018

#include <cassert>
#include <iostream>
#include <GLUT/glut.h>
#include "colour.h"
#include "directory.h"
#include "font.h"
#include "globals.h"
#include "input.h"
#include "play_state.h"
#include "sprite.h"

// Size of window in actual device pixels
const int WINDOW_W = 500;
const int WINDOW_H = 750;

const int PRETEND_SCREEN_W = 128;
const int PRETEND_SCREEN_H = 192;

bool yes_full_screen = false;

const float BG_SCROLL_VEL = -5.f;

static sprite bg;
static float bg_y = 0;

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  bg.draw(the_screen, 0, bg_y);

  the_game.draw();

  the_screen.draw_on_gl_thread(the_global_palette);

  glutSwapBuffers();
  glutPostRedisplay();
}

void update()
{
  the_timer.update();
  float dt = the_timer.get_dt();

  bg_y += dt * BG_SCROLL_VEL;
  const float edge = -(640 - 192); // TODO
  if (bg_y < edge)
  {
    bg_y = 0;
  }

  the_game.update(dt);
}

// * draw_and_update *
// Called every frame.
void draw_and_update()
{
  draw();
  update();
}

static int move = 0;

static void set(int& i, int b)
{
  i |= b;
}

static void clear(int& i, int b)
{
  i &= ~b; 
}

void key_down(unsigned char c, int, int)
{
  // Space bar => button
  if (c == ' ')
  {
    game_state* gs = the_game.get_game_state();
    gs->on_input(BUTTON_A);
  }
}

void key_up(unsigned char c, int, int)
{
  // Escape -> quit immediately
  // OH NO IT CRASHES

  if (c == 27)
  {
    delete the_sound_player;

    if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
    {
      glutLeaveGameMode();
    }
    exit(0);
  }

}

void special_key_down(int c, int, int)
{
  switch (c)
  {
  case GLUT_KEY_UP:
    set(move, MOVE_UP);
    break;
  case GLUT_KEY_DOWN:
    set(move, MOVE_DOWN);
    break;
  case GLUT_KEY_LEFT:
    set(move, MOVE_LEFT);
    break;
  case GLUT_KEY_RIGHT:
    set(move, MOVE_RIGHT);
    break;
  }

  game_state* gs = the_game.get_game_state();
  gs->on_input(move);
}

void special_key_up(int c, int, int)
{
  switch (c)
  {
  case GLUT_KEY_UP:
    clear(move, MOVE_UP);
    break;
  case GLUT_KEY_DOWN:
    clear(move, MOVE_DOWN);
    break;
  case GLUT_KEY_LEFT:
    clear(move, MOVE_LEFT);
    break;
  case GLUT_KEY_RIGHT:
    clear(move, MOVE_RIGHT);
    break;
  }

  game_state* gs = the_game.get_game_state();
  gs->on_input(move);
}

void joystick(unsigned int, int x, int y, int z)
{
  std::cout << "Got js callback! x: " << x << " y:" << y << "\n";
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WINDOW_W, WINDOW_H);

  if (yes_full_screen)
  {
    glutGameModeString("640x480:32@60");
    glutEnterGameMode();
  }
  else 
  {
    glutCreateWindow("Hello");
  }

  // Set up glut callbacks
  glutDisplayFunc(draw_and_update);
  glutKeyboardFunc(key_down);
  glutKeyboardUpFunc(key_up);
  glutSpecialFunc(special_key_down);
  glutSpecialUpFunc(special_key_up);

  // Joystick - Windows only?
  int joy_poll_interval = 60; // ?
  glutJoystickFunc(joystick, joy_poll_interval);

  // Pretend screen size
  screen::WIDTH = PRETEND_SCREEN_W;
  screen::HEIGHT = PRETEND_SCREEN_H;
  gluOrtho2D(0, screen::WIDTH, 0, screen::HEIGHT);
  the_screen.set_size(screen::WIDTH, screen::HEIGHT);

  // Add black colour for space bg!
  // This will be index 1, because index 0 is for transparent colour.
  the_global_palette.add_colour(colour(0, 0, 0));

  // Init font
  the_font.load(get_data_dir() + "font1_magenta.png", the_global_palette);
  the_font.set_num_cells(16, 4);

  // Init game states
  the_play_state = new play_state;

  // Initial state
  the_game.set_game_state(the_play_state);

  // Bg - always there for all states
  bg.load(get_data_dir() + "bg.png", the_global_palette);
  bg.set_num_cells(1, 1);

  // Must update once before draw
  update();

  the_sound_player = new BassSoundPlayer;

  // Play background music
  const bool LOOP = true;
//  the_sound_player->PlayWav(get_data_dir() + "Visager_-_11_-_Eerie_Mausoleum.wav", LOOP);


  glutMainLoop();
}


