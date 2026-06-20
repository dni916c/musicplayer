#------------------------------------------------------------------------------
#  Makefile for TECHNOSOUND Music Player
#
#  make                 makes practice (miniaudio playback test)
#  make test_playlist   makes the Playlist unit test
#  make all             makes all executables and library objects
#  make clean           removes all binaries and object files
#  make checkPlaylist   runs test_playlist under valgrind for memory leaks
#------------------------------------------------------------------------------

AUDIO_LIBS     = -lpthread -ldl -lm
COMPILE        = g++ -std=c++17 -Wall -c
LINK           = g++ -std=c++17 -Wall -o
REMOVE         = rm -f
MEMCHECK       = valgrind --leak-check=full

# ----- executables ----------------------------------------------------------

practice : practice_player.o
	$(LINK) practice practice_player.o $(AUDIO_LIBS)

test_playlist : test_playlist.o playlist.o
	$(LINK) test_playlist test_playlist.o playlist.o

all : practice test_playlist audio_engine.o web_server.o

# ----- object files ---------------------------------------------------------

practice_player.o : practice_player.cpp miniaudio.h
	$(COMPILE) practice_player.cpp

test_playlist.o : test_playlist.cpp playlist.h
	$(COMPILE) test_playlist.cpp

playlist.o : playlist.cpp playlist.h
	$(COMPILE) playlist.cpp

audio_engine.o : audio_engine.cpp audio_engine.h miniaudio.h
	$(COMPILE) audio_engine.cpp

web_server.o : web_server.cpp web_server.h
	$(COMPILE) web_server.cpp

# ----- utilities ------------------------------------------------------------

clean :
	$(REMOVE) practice test_playlist practice_player.o test_playlist.o playlist.o audio_engine.o web_server.o

checkPlaylist : test_playlist
	$(MEMCHECK) test_playlist
