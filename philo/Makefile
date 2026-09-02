NAME = philo

SRCDIR = src
OBJDIR = objs
INCDIR = includes

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCDIR)

SRC = main.c \
	monitor.c \
	utils.c \
	philosophers.c \
	init.c \

RED     = \033[1;31m
GREEN   = \033[1;32m
YELLOW  = \033[1;33m
BLUE    = \033[1;34m
MAGENTA = \033[1;35m
CYAN    = \033[1;36m
WHITE   = \033[1;37m
RESET   = \033[0m

FILES = ${addprefix ${SRCDIR}/,${SRC}}

OBJS = ${FILES:${SRCDIR}/%.c=${OBJDIR}/%.o}

all: $(NAME) finish

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS) $(ADDFLAGS)

${OBJDIR}/%.o: ${SRCDIR}/%.c $(INCDIR)/philo.h
	@mkdir -p ${dir $@}
	@printf "🟡 $(MAGENTA)[COMPILING📦] $<$(RESET)\n"
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

finish:
	@printf '$(RESET)\n'
	@printf "\r\033[K✅ $(GREEN)[PHILO] Compiled !$(RESET)\n"

.PHONY : all clean fclean re finish