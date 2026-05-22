# ============================================================
# Makefile - PDS2 2026 - Grupo 12 (Ricettario)
# Targets principais:
#   make            -> compila o executavel principal (build/app)
#   make test       -> compila e roda os testes + gera cobertura (gcovr)
#   make clean      -> remove artefatos de build
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -Itests
COVFLAGS := --coverage -O0 -g          # flags para gerar dados de cobertura (gcov/gcovr)

SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build
COV_DIR   := $(BUILD_DIR)/coverage

# Fontes da aplicacao EXCETO o main.cpp (o main dos testes vem do doctest)
APP_SRCS  := $(filter-out $(SRC_DIR)/main.cpp,$(wildcard $(SRC_DIR)/*.cpp))
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)

TEST_BIN  := $(BUILD_DIR)/run_tests
APP_BIN   := $(BUILD_DIR)/app

# ------------------------------------------------------------
# Build padrao: executavel principal
# ------------------------------------------------------------
all: $(APP_BIN)

$(APP_BIN): $(wildcard $(SRC_DIR)/*.cpp) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(wildcard $(SRC_DIR)/*.cpp) -o $@

# ------------------------------------------------------------
# make test : compila os testes COM cobertura, executa e gera relatorio
# ------------------------------------------------------------
test: $(BUILD_DIR) $(COV_DIR)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $(TEST_SRCS) $(APP_SRCS) -o $(TEST_BIN)
	./$(TEST_BIN)
	@echo "=================== COBERTURA ==================="
	gcovr -r . --filter src/ --filter include/ --exclude '.*doctest\.h' --html --html-details -o $(COV_DIR)/coverage.html ; \
	gcovr -r . --filter src/ --filter include/ --exclude '.*doctest\.h' ; \
	

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(COV_DIR): | $(BUILD_DIR)
	mkdir -p $(COV_DIR)

# ------------------------------------------------------------
clean:
	rm -rf $(BUILD_DIR) *.gcda *.gcno *.gcov

.PHONY: all test clean
