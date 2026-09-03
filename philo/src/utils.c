/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masolet- <masolet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 14:32:54 by masolet-          #+#    #+#             */
/*   Updated: 2026/09/03 14:58:46 by masolet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
// Parses a string argument into a strictly positive integer
*/
long	parse_argument(const char *str)
{
	long	res;
	int		i;

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
		i++;
	}
	if (res <= 0)
		return (-1);
	return (res);
}

/*
// Safely prints a philosopher's action with a timestamp
*/
void	print_action(t_data *data, int id, const char *routine)
{
	pthread_mutex_lock(&data->write_mutex);
	if (!is_simulation_stopped(data))
		printf("%ld %d %s", get_elapsed_time(data->start_time),
			id + 1, routine);
	pthread_mutex_unlock(&data->write_mutex);
}

/*
// Destroys all mutexes and frees allocated memory before exit
*/
void	cleanup_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (data->forks)
			pthread_mutex_destroy(&data->forks[i]);
		if (data->philos)
			pthread_mutex_destroy(&data->philos[i].state_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->data_mutex);
	pthread_mutex_destroy(&data->write_mutex);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}
