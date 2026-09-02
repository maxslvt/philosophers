
#include "philo.h"

long int	gettime(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}

static long int	gettime_micro(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval.tv_sec * 1000000 + timeval.tv_usec);
}

long int	get_timestamp(long start_time)
{
	return (gettime() - start_time);
}

void	ft_print_routine(t_data *data, int id, const char *routine)
{
	pthread_mutex_lock(&data->print_m);
	if (!ft_has_simulation_stopped(data))
		printf("%ld %d %s", get_timestamp(data->start_time), id + 1, routine);
	pthread_mutex_unlock(&data->print_m);
}

int	ft_usleep(unsigned int time, t_data *data)
{
	long	start;
	long	remains;
	long	time_micro;

	time_micro = (long)time * 1000;
	start = gettime_micro();
	while (1)
	{
		if (ft_has_simulation_stopped(data))
			break ;
		remains = time_micro - (gettime_micro() - start);
		if (remains <= 0)
			break ;
		remains /= 1000;
		if (remains > 50)
			usleep(40000);
		else if (remains > 5)
			usleep((remains - 2) * 1000);
		else
			usleep(100);
	}
	return (0);
}
