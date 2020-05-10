NAME = RM
EXE_DIR = ./
EXE = $(EXE_DIR)$(NAME)
FLAGS = -Wall -Wextra -Werror
SOURCE = main.c shader.c
HEADER = shader.h

INCLUDES = -Iincludes -I$(GLEW_DIR)/include -I$(GLFW_DIR)/include

GLEW_DIR = /usr/local/Cellar/glew/2.1.0_1/
GLFW_DIR = /usr/local/Cellar/glfw/3.3.2/

LIB = -framework OpenGL -lglew -lglfw

LIB_DIR = -L$(GLEW_DIR)lib -L$(GLFW_DIR)lib

VPATH = srcs includes

OBJ = $(SOURCE:.c=.o)
OBJ_DIR = obj/
OBJECT = $(patsubst %.o, $(OBJ_DIR)%.o, $(OBJ))

.PHONY : all clean fclean re

all : $(EXE)

$(EXE) : $(OBJECT)
	@echo "\033[1;3;34m"
	gcc $(FLAGS) $(OBJECT) -o $@ $(INCLUDES) $(LIB_DIR) $(LIB)
	@echo "\033[23;32m--[ "$(NAME) ]-- successfully compiled"\033[0m"

$(OBJ_DIR)%.o : %.c $(HEADER)
	@echo "\033[36;3m\c"
	gcc $(FLAGS) $(INCLUDES) -c $< -o $@
	@echo "\033[0m\c"

clean :
	@find $(OBJ_DIR) -type f -not -name '.gitkeep' -delete

fclean : clean
	@rm -f $(EXE)

re : fclean all
