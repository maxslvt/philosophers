NAME = philo

SRCDIR = src
OBJDIR = objs
INCDIR = includes

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCDIR)

SRC = main.c \
	monitor.c \
	utils.c \
	time.c \
	routine.c \
	init.c

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
	@printf "$(BLUE)[PHILO] Compiling: $(CYAN)$<$(RESET)\n"
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@printf "$(MAGENTA)[PHILO] Cleaning object files...$(RESET)\n"
	@rm -rf $(OBJDIR)
	@printf "$(GREEN)[PHILO] Cleaned !$(RESET)\n"

fclean: clean
	@printf "$(MAGENTA)[PHILO] Cleaning executable...$(RESET)\n"
	@rm -rf $(NAME)
	@printf "$(GREEN)[PHILO] Cleaned !$(RESET)\n"

re: fclean all

finish:
	@printf '$(RESET)\n'
	@printf "\r\033[K✅ $(GREEN)[PHILO] Compiled !$(RESET)\n"

.PHONY : all clean fclean re finish