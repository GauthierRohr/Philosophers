NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRC_DIR = src/
OBJ_DIR = obj/
INC_DIR = inc/

SRCS =	0_main.c \
		1_init.c \
		2_threads.c \
		3_routine.c \
		4_supervisor.c \
		5_utils.c
SRC = $(addprefix $(SRC_DIR),$(SRCS))
OBJ = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC))

HEADER = $(INC_DIR)philo.h

# Couleurs
GREEN   = \033[32m
CYAN    = \033[36m
RESET   = \033[0m

all: $(NAME)
	@echo "$(CYAN)[philo]$(RESET):$(GREEN) ./philo <nb_of_philo> <time_to_die> <time_to_eat> <time_to_sleep> [nb_of_times_philo_eat]!$(RESET)"

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(CYAN)[philo]$(RESET):$(GREEN) Compilation réussie !$(RESET)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(CYAN)[philo]$(RESET):$(GREEN) Fichiers objets nettoyés$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(CYAN)[philo]$(RESET):$(GREEN) Exécutable nettoyé$(RESET)"

re: fclean all

.PHONY: all clean fclean re