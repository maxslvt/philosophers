
#include "philo.h"


void	ft_free_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (data->tab_fork)
			pthread_mutex_destroy(&data->tab_fork[i]);
		if (data->tab_philo)
			pthread_mutex_destroy(&data->tab_philo[i].philo_m);
		i++;
	}
	pthread_mutex_destroy(&data->data_m);
	pthread_mutex_destroy(&data->print_m);
	if (data->tab_fork)
		free(data->tab_fork);
	if (data->tab_philo)
		free(data->tab_philo);
}

int	ft_parse_arg(const char *str)
{
	long int	res;
	int			i;

	res = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	if (res <= 0)
		return (-1);
	return ((int)res);
}

int	ft_create_threads(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = gettime();
	while (i < data->nb_philo)
	{
		data->tab_philo[i].last_meal_ms = data->start_time;
		if (pthread_create(&data->tab_philo[i].thid, NULL, ft_philo,
				&data->tab_philo[i]) != 0)
			return (i);
		i++;
	}
	pthread_mutex_lock(&data->data_m);
	data->is_started = 1;
	pthread_mutex_unlock(&data->data_m);
	return (-1);
}

void	*ft_philo_one(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->tab_fork[philo->f_left]);
	ft_print_routine(data, philo->id, "has taken a fork\n");
	ft_usleep(data->time_die, data);
	pthread_mutex_unlock(&data->tab_fork[philo->f_left]);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	if (!ft_init(&data, ac, av))
		return (write(2, "Error: problem during init\n", 27), 1);
	i = ft_create_threads(&data);
	if (i != -1)
	{
		pthread_mutex_lock(&data.data_m);
		data.simulation_ended = 1;
		pthread_mutex_unlock(&data.data_m);
		ft_wait_threads(&data, i);
		ft_free_data(&data);
		return (1);
	}
	ft_wait_death(&data);
	ft_wait_threads(&data, data.nb_philo);
	ft_free_data(&data);
	return (0);
}
