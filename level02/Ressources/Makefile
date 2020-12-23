# EXECUTABLE
NAME = generator

# SOURCES
SRC = generator.cpp

# OBJETS
OBJ = $(SRC:.cpp=.o)

# COMPILATEUR
CC      = g++
CFLAGS  = -g3

# TEXT
RED = \033[31m
WHITE = \033[0;29m
YELLOW = \033[33m
PINK = \033[35m
GREEN = \033[32m
BLUE = \033[36m
DEFAULT = \033[0m

# REGLES
all: $(NAME)

$(NAME): $(OBJ)
	@printf "\033[2K\r$(BLUE)>>Linking...$(DEFAULT) "
	@$(CC) -o $@ $(OBJ)

	@printf "\033[2K\r$(NAME) has been created $(GREEN)[OK]$(DEFAULT)\n"

%.o: %.cpp
	@printf "\033[2K\r$(BLUE)>>Compiling $(DEFAULT)$< "
	@$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean fclean re

clean:
	@echo "$(YELLOW)$(NAME):\t$(RED)rm $(YELLOW)objects\t  $(GREEN)[OK]$(DEFAULT)"
	@rm -f $(OBJ)

fclean: clean
	@echo "$(YELLOW)$(NAME):\t$(RED)rm $(YELLOW)binaries\t  $(GREEN)[OK]$(DEFAULT)"
	@rm -f $(NAME)

re:		fclean all
