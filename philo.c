/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney <rimney@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:29:56 by rimney            #+#    #+#             */
/*   Updated: 2022/03/03 04:49:11 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_assign(char **argv, t_philosophers *philo)
{
	philo->philosopher = malloc(sizeof(pthread_t) * ft_atoi(argv[1]));
	philo->forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(argv[1]));
	philo->number_of_philosophers = ft_atoi(argv[1]);
	philo->time_to_die = ft_atoi(argv[2]);
	philo->time_to_eat = ft_atoi(argv[3]);
	philo->time_to_sleep = ft_atoi(argv[4]);
	philo->each_time = ft_atoi(argv[5]);
}

// void	ft_eat(t_philosophers *philo)
// {
	
	
// }

void	*ft_routine(void *p)
{
	static int i = 0;
	t_philosophers *philo = (t_philosophers *)p;
	pthread_mutex_lock(&philo->forks[i]);
	printf("Philo %d has taken a fork\n", i + 1);
	pthread_mutex_lock(&philo->forks[(i + 1) % philo->number_of_philosophers]);
	printf("Philo %d is eating\n", i + 1);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->forks[i]);
	pthread_mutex_unlock(&philo->forks[(i + 1) % philo->number_of_philosophers]);
	printf("Philo %d is sleeping\n", i);
	usleep(philo->time_to_sleep * 1000);
	i++;
	return (0);
}

void	ft_create_threads(t_philosophers *philo)
{
	int i = 0;
	int j = 0;
	while(j < philo->number_of_philosophers)
	{
		pthread_mutex_init(&philo->forks[j], 0);
		usleep(50);
		j++;
	}
	while(i < philo->number_of_philosophers)
	{
		pthread_create(&philo->philosopher[i], NULL, ft_routine, philo);
		usleep(50);
		pthread_join(philo->philosopher[i], 0);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_philosophers philo;

	ft_assign(argv, &philo);
	ft_create_threads(&philo);
}