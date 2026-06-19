CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Ivendor -Isrc -MMD -MP
LDFLAGS  := -lpthread -ldl -lm

BIN      := player
SRC      := src/main.cpp src/audio_engine.cpp src/miniaudio_impl.cpp
OBJ      := $(SRC:.cpp=.o)
DEP      := $(OBJ:.o=.d)

.PHONY: all clean run

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile each .cpp to its own .o. Because miniaudio_impl.o is separate, the
# slow ~95k-line miniaudio build only reruns when miniaudio.h changes — your
# own edits recompile in a fraction of a second.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pull in the auto-generated header dependencies (-MMD -MP above), so touching
# audio_engine.h rebuilds the right objects and nothing stale slips through.
-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(BIN)

# Usage: make run FILE=path/to/track.flac
run: $(BIN)
	./$(BIN) $(FILE)
