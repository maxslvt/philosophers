#include "philo.h"

/* Special routine for a single philosopher to avoid instant deadlock */
static void	*single_philo_routine(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->forks[philo->left_fork_id]);
	print_action(data, philo->id, "has taken a fork\n");
	precise_usleep(data->time_to_die, data);
	pthread_mutex_unlock(&data->forks[philo->left_fork_id]);
	return (NULL);
}

/* Calculates and executes the optimal thinking time to avoid desynchronization */
static void	philo_think(t_data *data, t_philo *philo)
{
	long	time_to_think;

	print_action(data, philo->id, "is thinking\n");
	if (data->philo_count % 2 == 0)
	{
		if (data->time_to_eat > data->time_to_sleep)
			time_to_think = data->time_to_eat - data->time_to_sleep;
		else
			time_to_think = 0;
	}
	else
	{
		time_to_think = (data->time_to_eat * 2) - data->time_to_sleep;
		if (time_to_think < 0)
			time_to_think = 0;
	}
	if (time_to_think > 0)
		precise_usleep(time_to_think * 0.4, data);
}

/* Checks if the philosopher has reached the maximum number of meals */
static int	check_meals_limit(t_data *data, t_philo *philo)
{
	if (data->meals_limit != -1 && philo->meals_eaten >= data->meals_limit)
	{
		pthread_mutex_lock(&data->data_mutex);
		data->finished_count++;
		pthread_mutex_unlock(&data->data_mutex);
		pthread_mutex_lock(&philo->state_mutex);
		philo->is_alive = false;
		pthread_mutex_unlock(&philo->state_mutex);
		return (0);
	}
	return (1);
}

/* Handles the eating process: locking forks, updating stats, and eating */
static void	philo_eat(t_data *data, int idx)
{
	t_philo	*philo;
	long	t;

	philo = &data->philos[idx];
	pthread_mutex_lock(&data->forks[philo->left_fork_id]);
	print_action(data, idx, "has taken a fork\n");
	pthread_mutex_lock(&data->forks[philo->right_fork_id]);
	pthread_mutex_lock(&philo->state_mutex);
	philo->last_meal_ms = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->state_mutex);
	pthread_mutex_lock(&data->write_mutex);
	if (!is_simulation_stopped(data))
	{
		t = get_elapsed_time(data->start_time);
		printf("%ld %d has taken a fork\n%ld %d is eating\n", t, idx + 1, t, idx + 1);
	}
	pthread_mutex_unlock(&data->write_mutex);
	precise_usleep(data->time_to_eat, data);
	pthread_mutex_unlock(&data->forks[philo->left_fork_id]);
	pthread_mutex_unlock(&data->forks[philo->right_fork_id]);
}

/* The main lifecycle of a philosopher: eat, sleep, and think */
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (!wait_for_start(data))
		return (NULL);
	print_action(data, philo->id, "is thinking\n");
	if (data->philo_count == 1)
		return (single_philo_routine(data, philo));
	if (philo->id % 2 == 1)
		precise_usleep(data->time_to_eat / 2, data);
	while (!is_simulation_stopped(data))
	{
		philo_eat(data, philo->id);
		if (!check_meals_limit(data, philo))
			break ;
		print_action(data, philo->id, "is sleeping\n");
		precise_usleep(data->time_to_sleep, data);
		philo_think(data, philo);
	}
	return (NULL);
}
