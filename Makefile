PROG = main

# Compilador C++
CXX = g++

# Opções do compilador
CXXFLAGS = -std=c++11 -Wall -lsfml-graphics -lsfml-window -lsfml-system -pthread# -fsanitize=address,undefined -g

# Diretório onde os arquivos .hpp e .cpp estão localizados
SRC_DIR = src

# Diretório onde os arquivos de objetos serão gerados
OBJ_DIR = obj

# Lista de arquivos .cpp
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Lista de arquivos de objeto gerados a partir dos arquivos .cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Regra para criar o executável
$(PROG): $(OBJS)
	$(CXX) main.cpp $(CXXFLAGS) -o $(PROG) $(OBJS)

# Regra para criar arquivos de objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove todos os arquivos de objeto e o executável
clean:
	rm -f $(PROG) $(OBJS)

# Define o alvo padrão (o que será construído quando você apenas chamar `make`)
all: $(PROG)

.PHONY: all clean
