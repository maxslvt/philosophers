NAME 		= philo

SRC_DIR		=	src

SRC			=	main.c \
				waiting.c \
				utils.c \
				philosophers.c \
				init.c \

FILES		=	${addprefix ${SRC_DIR}/,${SRC}}

OBJ_DIR		=	objs

OBJETS		=	${FILES:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

INC_DIR		=	./inc

CFLAGS		=	-Wall -Wextra -Werror

all : $(NAME)

$(NAME): $(OBJETS)
	cc $(OBJETS) -o $(NAME) $(CFLAGS) $(ADDFALGS)

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c $(INC_DIR)/philosopher.h
	@mkdir -p ${dir $@}
	cc $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re