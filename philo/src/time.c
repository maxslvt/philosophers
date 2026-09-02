#include "philo.h"

/* Returns the current time in milliseconds */
long	get_current_time(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}

/* Returns the current time in microseconds for precise calculations */
static long	get_current_time_micro(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval.tv_sec * 1000000 + timeval.tv_usec);
}

/* Calculates the elapsed time in milliseconds since the start of the simulation */
long	get_elapsed_time(long start_time)
{
	return (get_current_time() - start_time);
}

/* A more precise alternative to usleep using smaller intervals */
int	precise_usleep(unsigned int time, t_data *data)
{
	long	start;
	long	remains;
	long	time_micro;

	time_micro = (long)time * 1000;
	start = get_current_time_micro();
	while (1)
	{
		if (is_simulation_stopped(data))
			break ;
		remains = time_micro - (get_current_time_micro() - start);
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
