
#include "philo.h"

static void	ft_eat(t_data *data, int idx)
{
	t_philo	*philo;
	long	t;

	philo = &data->tab_philo[idx];
	pthread_mutex_lock(&data->tab_fork[philo->f_left]);
	ft_print_routine(data, idx, "has taken a fork\n");
	pthread_mutex_lock(&data->tab_fork[philo->f_right]);
	pthread_mutex_lock(&philo->philo_m);
	philo->last_meal_ms = gettime();
	philo->nb_meal++;
	pthread_mutex_unlock(&philo->philo_m);
	pthread_mutex_lock(&data->print_m);
	if (!ft_has_simulation_stopped(data))
	{
		t = get_timestamp(data->start_time);
		printf("%ld %d has taken a fork\n%ld %d is eating\n", t, idx + 1, t, idx + 1);
	}
	pthread_mutex_unlock(&data->print_m);
	ft_usleep(data->time_eat, data);
	pthread_mutex_unlock(&data->tab_fork[philo->f_left]);
	pthread_mutex_unlock(&data->tab_fork[philo->f_right]);
}

static void	ft_think(t_data *data, t_philo *philo)
{
	long	time_to_think;

	ft_print_routine(data, philo->id, "is thinking\n");
	if (data->nb_philo % 2 == 0)
	{
		if (data->time_eat > data->time_sleep)
			time_to_think = data->time_eat - data->time_sleep;
		else
			time_to_think = 0;
	}
	else
	{
		time_to_think = (data->time_eat * 2) - data->time_sleep;
		if (time_to_think < 0)
			time_to_think = 0;
	}
	if (time_to_think > 0)
		ft_usleep(time_to_think * 0.4, data);
}

static void	ft_sleep(t_data *data, t_philo *philo)
{
	ft_print_routine(data, philo->id, "is sleeping\n");
	ft_usleep(data->time_sleep, data);
}

static int	ft_check_meal(t_data *data, t_philo *philo)
{
	if (data->nb_eat_max != -1 && philo->nb_meal >= data->nb_eat_max)
	{
		pthread_mutex_lock(&data->data_m);
		data->nb_finished++;
		pthread_mutex_unlock(&data->data_m);
		pthread_mutex_lock(&philo->philo_m); //added
		philo->is_alive = false; //added
		pthread_mutex_unlock(&philo->philo_m); //added
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
	ft_print_routine(data, philo->id, "is thinking\n");
	if (data->nb_philo == 1)
		return (ft_philo_one(data, philo));
	if (philo->id % 2 == 1)
		ft_usleep(data->time_eat / 2, data);
	while (!ft_has_simulation_stopped(data))
	{
		ft_eat(data, philo->id);
		if (!ft_check_meal(data, philo))
			break ;
		ft_sleep(data, philo);
		ft_think(data, philo);
	}
	return (NULL);
}
