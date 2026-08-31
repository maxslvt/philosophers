
#include "../inc/philosopher.h"

void	ft_init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&data->tab_fork[i], NULL);
		pthread_mutex_init(&data->tab_philo[i].philo_m, NULL);
		i++;
	}
	pthread_mutex_init(&data->data_m, NULL);
	pthread_mutex_init(&data->print_m, NULL);
}

int	ft_init_values(t_data *data, int ac, char **av)
{
	if (ac == 6)
	{
		data->nb_eat_max = ft_atoi(av[5]);
		if ((!is_int(av[5], data->nb_eat_max) || data->nb_eat_max <= 0))
			return (0);
	}
	else
		data->nb_eat_max = -1;
	data->is_started = 0;
	data->simulation_ended = 0;
	data->nb_finished = 0;
	data->nb_philo = ft_atoi(av[1]);
	data->time_die = ft_atoi(av[2]);
	data->time_eat = ft_atoi(av[3]);
	data->time_sleep = ft_atoi(av[4]);
	return (ft_check_data(data, av));
}

void	ft_philo_init(t_data *data, int idx)
{
	t_philo	*philo;

	philo = &data->tab_philo[idx];
	philo->id = idx;
	philo->data = data;
	philo->is_alive = 1;
	philo->last_meal_ms = 0;
	philo->nb_meal = 0;
	philo->f_left = idx;
	philo->f_right = (idx + 1) % data->nb_philo;
	if (philo->id == data->nb_philo - 1 && data->nb_philo > 1)
	{
		philo->f_left = 0;
		philo->f_right = idx;
	}
}

int	ft_init(t_data *data, int ac, char **av)
{
	int	i;

	if (ac < 5 || ac > 6)
		return (0);
	memset(data, 0, sizeof(t_data));
	if (!ft_init_values(data, ac, av))
		return (0);
	data->tab_philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->tab_philo)
		return (0);
	memset(data->tab_philo, 0, sizeof(t_philo) * data->nb_philo);
	data->tab_fork = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->tab_fork)
		return (free(data->tab_philo), 0);
	i = 0;
	while (i < data->nb_philo)
	{
		ft_philo_init(data, i);
		i++;
	}
	ft_init_mutex(data);
	return (1);
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