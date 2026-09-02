
#include "philo.h"

int	ft_init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->tab_fork[i], NULL) != 0)
			return (0);
		if (pthread_mutex_init(&data->tab_philo[i].philo_m, NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->data_m, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->print_m, NULL) != 0)
		return (0);
	return (1);
}

int	ft_init_values(t_data *data, int ac, char **av)
{
	data->nb_philo = ft_parse_arg(av[1]);
	data->time_die = ft_parse_arg(av[2]);
	data->time_eat = ft_parse_arg(av[3]);
	data->time_sleep = ft_parse_arg(av[4]);
	if (data->nb_philo == -1 || data->time_die == -1 
		|| data->time_eat == -1 || data->time_sleep == -1)
		return (0);
	if (ac == 6)
	{
		data->nb_eat_max = ft_parse_arg(av[5]);
		if (data->nb_eat_max == -1)
			return (0);
	}
	else
		data->nb_eat_max = -1;
	data->is_started = false;
	data->is_ended = false;
	data->nb_finished = 0;
	return (1);
}

void	ft_philo_init(t_data *data, int idx)
{
	t_philo	*philo;

	philo = &data->tab_philo[idx];
	philo->id = idx;
	philo->data = data;
	philo->is_alive = true;
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

static int	ft_alloc_data(t_data *data)
{
	data->tab_philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->tab_philo)
		return (0);
	memset(data->tab_philo, 0, sizeof(t_philo) * data->nb_philo);
	data->tab_fork = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->tab_fork)
	{
		free(data->tab_philo);
		return (0);
	}
	return (1);
}

int	ft_init(t_data *data, int ac, char **av)
{
	int	i;

	if (ac < 5 || ac > 6)
		return (0);
	memset(data, 0, sizeof(t_data));
	if (!ft_init_values(data, ac, av))
		return (0);
	if (!ft_alloc_data(data))
		return (0);
	i = 0;
	while (i < data->nb_philo)
	{
		ft_philo_init(data, i);
		i++;
	}
	if (!ft_init_mutex(data))
	{
		free(data->tab_fork);
		free(data->tab_philo);
		return (0);
	}
	return (1);
}
