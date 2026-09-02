#include "philo.h"

/* Initializes all required mutexes for forks, data, and writing */
static int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (0);
		if (pthread_mutex_init(&data->philos[i].state_mutex, NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->data_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (0);
	return (1);
}

/* Parses arguments and initializes the simulation parameters */
static int	init_values(t_data *data, int ac, char **av)
{
	data->philo_count = parse_argument(av[1]);
	data->time_to_die = parse_argument(av[2]);
	data->time_to_eat = parse_argument(av[3]);
	data->time_to_sleep = parse_argument(av[4]);
	if (data->philo_count == -1 || data->time_to_die == -1 
		|| data->time_to_eat == -1 || data->time_to_sleep == -1)
		return (0);
	if (ac == 6)
	{
		data->meals_limit = parse_argument(av[5]);
		if (data->meals_limit == -1)
			return (0);
	}
	else
		data->meals_limit = -1;
	data->is_started = false;
	data->is_ended = false;
	data->finished_count = 0;
	return (1);
}

/* Initializes individual philosopher state and assigns their forks */
static void	init_philosophers(t_data *data, int idx)
{
	t_philo	*philo;

	philo = &data->philos[idx];
	philo->id = idx;
	philo->data = data;
	philo->is_alive = true;
	philo->meals_eaten = 0;
	philo->left_fork_id = idx;
	philo->right_fork_id = (idx + 1) % data->philo_count;
	if (philo->id == data->philo_count - 1 && data->philo_count > 1)
	{
		philo->left_fork_id = 0;
		philo->right_fork_id = idx;
	}
}

/* Allocates memory for the philosophers array and the forks array */
static int	alloc_data(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philos)
		return (0);
	memset(data->philos, 0, sizeof(t_philo) * data->philo_count);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->forks)
	{
		free(data->philos);
		return (0);
	}
	return (1);
}

/* Main initialization function that sets up all data structures */
int	init_data(t_data *data, int ac, char **av)
{
	int	i;

	if (ac != 5 && ac != 6)
		return (0);
	if (!init_values(data, ac, av))
		return (0);
	if (!alloc_data(data))
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		init_philosophers(data, i);
		i++;
	}
	if (!init_mutexes(data))
	{
		free(data->forks);
		free(data->philos);
		return (0);
	}
	return (1);
}
