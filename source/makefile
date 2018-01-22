all:haxxian_exe

clean:
	rm haxxian *.o ../../lo-res/*.o ../../microlib/*.o 

run:
	./haxxian


CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++

WARNINGS=-Wall -Wno-deprecated-declarations

INCLUDES=\
  -I.. \
  -I../../lo-res \
  -I../../microlib

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++11   -DMACOSX  -g -O0 -arch i386

LDFLAGS=-framework OpenGL -framework GLUT -framework Carbon ../../microlib/libbass.dylib -arch i386

SOURCES=\
baddie.cpp \
bullet.cpp \
globals.cpp \
haxxian_game_object.cpp \
main.cpp \
player.cpp \
play_state.cpp \
../../lo-res/font.cpp \
../../lo-res/image.cpp \
../../lo-res/lodepng.cpp \
../../lo-res/palette.cpp \
../../lo-res/screen.cpp \
../../lo-res/sprite.cpp \
../../lo-res/sprite_sheet.cpp \
../../microlib/BassSoundPlayer.cpp \
../../microlib/directory.cpp \
../../microlib/file_string_utils.cpp \
../../microlib/game.cpp \
../../microlib/game_object.cpp \
../../microlib/message_queue.cpp \
../../microlib/timer.cpp


OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=haxxian

haxxian_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

