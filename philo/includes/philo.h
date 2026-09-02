
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_data;

typedef struct s_philo
{
	int				id;
	bool			is_alive;
	pthread_t		thid;
	long			last_meal_ms;
	int				nb_meal;
	int				f_left;
	int				f_right;
	pthread_mutex_t	philo_m;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	bool			is_started;
	bool			is_ended;
	int				nb_philo;
	int				nb_finished;
	int				nb_eat_max;
	long			start_time;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	t_philo			*tab_philo;
	pthread_mutex_t	*tab_fork;
	pthread_mutex_t	data_m;
	pthread_mutex_t	print_m;
}					t_data;

void				ft_wait_setup(t_data *data);
bool				ft_wait_start(t_data *data);
void				ft_wait_death(t_data *data);
void				*ft_philo(void *arg);
void				*ft_philo_one(t_data *data, t_philo *philo);
void				ft_wait_threads(t_data *data, int nb);
int					ft_create_threads(t_data *data);
void				ft_print_routine(t_data *data, int id, const char *routine);
int					ft_usleep(unsigned int time, t_data *data);
long int			gettime(void);
long int			get_timestamp(long start_time);
bool				ft_has_simulation_stopped(t_data *data);
int					ft_init(t_data *data, int ac, char **av);
void				ft_philo_init(t_data *data, int idx);
long				ft_parse_arg(const char *str);
void				ft_free_data(t_data *data);

#endif