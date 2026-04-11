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

# Tự tìm môi trường và thiết lập lệnh phù hợp
ifdef MSYSTEM
    # Chạy từ terminal MSYS2 (có sẵn rm, cp)
    SDL_INCLUDE = -IC:/msys64/ucrt64/include/SDL2
    SDL_LIB = -LC:/msys64/ucrt64/lib
    SDL_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_gfx -lSDL2_ttf
    RUN = ./$(OUTPUT)
    COPY = cp assets/font.ttf build/font.ttf
    CLEAN = rm -f $(OUTPUT) $(TEST_OUTPUT) build/font.ttf
else ifeq ($(OS),Windows_NT)
    # Chạy từ CMD hoặc PowerShell (dùng lệnh PowerShell)
    SDL_INCLUDE = -IC:/msys64/ucrt64/include/SDL2
    SDL_LIB = -LC:/msys64/ucrt64/lib
    SDL_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_gfx -lSDL2_ttf
    RUN = $(OUTPUT)
    COPY = powershell -Command "Copy-Item -Force assets/font.ttf build/font.ttf"
    CLEAN = powershell -Command "Remove-Item -Force -ErrorAction SilentlyContinue build/Game2048.exe; Remove-Item -Force -ErrorAction SilentlyContinue build/run_test.exe; Remove-Item -Force -ErrorAction SilentlyContinue build/font.ttf"
else
    # Linux / macOS
    SDL_INCLUDE = $(shell pkg-config --cflags sdl2 SDL2_gfx SDL2_ttf)
    SDL_LIB =
    SDL_FLAGS = $(shell pkg-config --libs sdl2 SDL2_gfx SDL2_ttf)
    OUTPUT = build/Game2048
    RUN = ./$(OUTPUT)
    COPY = cp assets/font.ttf build/font.ttf
    CLEAN = rm -f $(OUTPUT) $(TEST_OUTPUT) build/font.ttf
endif

# Build game
all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CXX) $(SOURCES) -o $(OUTPUT) $(SDL_INCLUDE) $(SDL_LIB) $(SDL_FLAGS)
	$(COPY)

# Build và chạy game
run: $(OUTPUT)
	$(RUN)

# Build và chạy test
test: $(TEST_OUTPUT)
	./$(TEST_OUTPUT)

$(TEST_OUTPUT): $(TEST_SOURCES)
	$(CXX) $(TEST_SOURCES) -o $(TEST_OUTPUT)

# Xóa file exe và font
clean:
	$(CLEAN)
