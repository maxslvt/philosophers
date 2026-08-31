
#include "../inc/philosopher.h"

static int	ft_eat(t_data *data, int idx)
{
	t_philo	*philo;

	philo = &data->tab_philo[idx];
	pthread_mutex_lock(&data->tab_fork[philo->f_left]);
	ft_print_routine(data, idx, "has taken a fork\n");
	if (data->nb_philo == 1)
	{
		ft_usleep(data->time_die, data);
		pthread_mutex_unlock(&data->tab_fork[philo->f_left]);
		return (1);
	}
	pthread_mutex_lock(&data->tab_fork[philo->f_right]);
	ft_print_routine(data, idx, "has taken a fork\n");
	pthread_mutex_lock(&philo->philo_m);
	philo->last_meal_ms = gettime();
	philo->nb_meal++;
	pthread_mutex_unlock(&philo->philo_m);
	ft_print_routine(data, idx, "is eating\n");
	ft_usleep(data->time_eat, data);
	pthread_mutex_unlock(&data->tab_fork[philo->f_left]);
	pthread_mutex_unlock(&data->tab_fork[philo->f_right]);
	return (1);
}

static void	ft_sleep(t_data *data, t_philo *philo)
{
	ft_print_routine(data, philo->id, "is sleeping\n");
	ft_usleep(data->time_sleep, data);
	if (data->nb_philo % 2 == 1)
	{
		if (data->time_eat >= data->time_sleep)
			ft_usleep(data->time_eat - data->time_sleep + 1, data);
	}
}

static int	ft_check_meal(t_data *data, t_philo *philo)
{
	if (data->nb_eat_max != -1 && philo->nb_meal >= data->nb_eat_max)
	{
		pthread_mutex_lock(&data->data_m);
		data->nb_finished++;
		pthread_mutex_unlock(&data->data_m);
		return (0);
	}
	return (1);
}

void	*ft_philo(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (!ft_wait_start(data))
		return (NULL);
	if (philo->id % 2 == 1)
		ft_usleep(data->time_eat / 2, data);
	while (!ft_has_simulation_stopped(data))
	{
		ft_print_routine(data, philo->id, "is thinking\n");
		ft_eat(data, philo->id);
		if (!ft_check_meal(data, philo))
			break ;
		ft_sleep(data, philo);
	}
	return (NULL);
}