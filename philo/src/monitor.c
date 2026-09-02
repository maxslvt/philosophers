
#include "philo.h"

int	ft_has_simulation_stopped(t_data *data)
{
	int	stopped;

	pthread_mutex_lock(&data->data_m);
	stopped = data->simulation_ended;
	pthread_mutex_unlock(&data->data_m);
	return (stopped);
}

int	ft_wait_start(t_data *data)
{
	while (1)
	{
		pthread_mutex_lock(&data->data_m);
		if (data->is_started)
		{
			pthread_mutex_unlock(&data->data_m);
			return (1);
		}
		if (data->simulation_ended)
		{
			pthread_mutex_unlock(&data->data_m);
			return (0);
		}
		pthread_mutex_unlock(&data->data_m);
		usleep(200);
	}
}

static int	ft_check_philo_death(t_data *data, int i)
{
	pthread_mutex_lock(&data->tab_philo[i].philo_m);
	if (gettime() - data->tab_philo[i].last_meal_ms >= data->time_die
		&& data->tab_philo[i].is_alive)
	{
		pthread_mutex_lock(&data->print_m);
		pthread_mutex_lock(&data->data_m);
		data->simulation_ended = 1;
		pthread_mutex_unlock(&data->data_m);
		printf("%ld %d %s", get_timestamp(data->start_time),
			data->tab_philo[i].id + 1, "died\n");
		pthread_mutex_unlock(&data->print_m);
		pthread_mutex_unlock(&data->tab_philo[i].philo_m);
		return (1);
	}
	pthread_mutex_unlock(&data->tab_philo[i].philo_m);
	return (0);
}

void	ft_wait_death(t_data *data)
{
	int	i;

	i = 0;
	while (!ft_has_simulation_stopped(data))
	{
		pthread_mutex_lock(&data->data_m);
		if (data->nb_finished >= data->nb_philo)
		{
			data->simulation_ended = 1;
			pthread_mutex_unlock(&data->data_m);
			break ;
		}
		pthread_mutex_unlock(&data->data_m);
		if (ft_check_philo_death(data, i))
			break ;
		i = (i + 1) % data->nb_philo;
		if (i == 0)
			usleep(1000);
	}
}

void	ft_wait_threads(t_data *data, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		pthread_join(data->tab_philo[i].thid, NULL);
		i++;
	}
}
