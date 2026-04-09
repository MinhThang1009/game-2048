# Mục đích: Build game 2048
# Chức năng: Tự động biên dịch source code và chạy game

# Trình biên dịch
CXX = g++

# File source
SOURCES = src/main.cpp src/logic.cpp
TEST_SOURCES = tests/test_logic.cpp src/logic.cpp

# File output
OUTPUT = build/Game2048.exe
TEST_OUTPUT = build/run_test.exe

# Tự tìm đường dẫn SDL2 theo hệ điều hành
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2/UCRT64)
    SDL_INCLUDE = -IC:/msys64/ucrt64/include/SDL2
    SDL_LIB = -LC:/msys64/ucrt64/lib
    SDL_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_gfx -lSDL2_ttf
    RUN = ./$(OUTPUT)
else
    # Linux / macOS
    SDL_INCLUDE = $(shell pkg-config --cflags sdl2 SDL2_gfx SDL2_ttf)
    SDL_LIB =
    SDL_FLAGS = $(shell pkg-config --libs sdl2 SDL2_gfx SDL2_ttf)
    OUTPUT = build/Game2048
    RUN = ./$(OUTPUT)
endif

# Build game
all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CXX) $(SOURCES) -o $(OUTPUT) $(SDL_INCLUDE) $(SDL_LIB) $(SDL_FLAGS)

# Build va chay game
run: $(OUTPUT)
	$(RUN)

# Build va chay test
test: $(TEST_OUTPUT)
	./$(TEST_OUTPUT)

$(TEST_OUTPUT): $(TEST_SOURCES)
	$(CXX) $(TEST_SOURCES) -o $(TEST_OUTPUT)

# Xoa file exe
clean:
	rm -f $(OUTPUT) $(TEST_OUTPUT)
