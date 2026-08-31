
#include "../inc/philosopher.h"

long int	gettime(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
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

	start = gettime();
	while ((gettime() - start) < time)
	{
		if (ft_has_simulation_stopped(data))
			break ;
		usleep(500);
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int		result;
	int		sign;
	size_t	i;

	sign = 1;
	result = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + str[i++] - '0';
	return (result * sign);
}