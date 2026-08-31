
#include "../inc/philosopher.h"

static int	ft_nbnb(char *str)
{
	int	nb;
	int	i;

	nb = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb++;
		i++;
	}
	return (nb);
}

int	is_int(char *str, int nb)
{
	if (ft_nbnb(str) > 10)
		return (0);
	if (ft_nbnb(str) == 10 && nb < 1000000000)
		return (0);
	if (ft_is_neg(str))
		return (0);
	return (1);
}

int	ft_check_data(t_data *data, char **av)
{
	if (is_int(av[1], data->nb_philo) && is_int(av[2], data->time_die)
		&& is_int(av[3], data->time_eat) && is_int(av[4], data->time_sleep)
		&& data->nb_philo > 0 && data->time_die > 0 && data->time_eat > 0
		&& data->time_sleep > 0)
		return (1);
	return (0);
}

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