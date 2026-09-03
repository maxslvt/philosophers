/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masolet- <masolet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 14:32:31 by masolet-          #+#    #+#             */
/*   Updated: 2026/09/03 14:32:34 by masolet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	pthread_mutex_t	state_mutex;
	pthread_t		thread_id;
	long			last_meal_ms;
	struct s_data	*data;
	int				id;
	int				meals_eaten;
	int				left_fork_id;
	int				right_fork_id;
	bool			is_alive;
}					t_philo;

typedef struct s_data
{
	pthread_mutex_t	data_mutex;
	pthread_mutex_t	write_mutex;
	long			start_time;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				philo_count;
	int				finished_count;
	int				meals_limit;
	bool			is_started;
	bool			is_ended;
}					t_data;

/* init.c */
int					init_data(t_data *data, int ac, char **av);

/* time.c */
long				get_current_time(void);
long				get_elapsed_time(long start_time);
int					precise_usleep(unsigned int time, t_data *data);

/* routine.c */
void				*philosopher_routine(void *arg);

/* monitor.c */
bool				is_simulation_stopped(t_data *data);
bool				wait_for_start(t_data *data);
void				monitor_routine(t_data *data);

/* utils.c */
long				parse_argument(const char *str);
void				print_action(t_data *data, int id, const char *routine);
void				cleanup_data(t_data *data);

#endif