/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosohpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney <rimney@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 19:42:37 by rimney            #+#    #+#             */
/*   Updated: 2022/04/04 22:24:15 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"



int    ft_assign(int argc, char **argv, t_args *args) // tested and it works fine <3.
{
    args->philo_num = ft_atoi(argv[1]);
    args->time_to_die = ft_atoi(argv[2]);
    args->time_to_eat = ft_atoi(argv[3]);
    args->time_to_sleep = ft_atoi(argv[4]);
    args->each_time = 0;
    if (argc == 6)
        args->each_time = ft_atoi(argv[5]);
    return (0);
}

void    ft_assign_values(t_philo *philo, t_args *args)
{
    int i;

    i = 0;
    while (i < args->philo_num)
    {
        philo[i].id = i + 1;
        philo[i].args = args;
        if (i == args->philo_num - 1)
            philo[i].next_fork = &philo[0].fork;
        else
            philo[i].next_fork = &philo[i + 1].fork;
		i++;
    }
}

void	ft_philo_activity(t_philo *philo)
{
	ft_print_message("is thinkng", philo->id, philo->args);
	pthread_mutex_lock(&philo->fork);
	ft_print_message("has taken a fork", philo->id, philo->args);
	pthread_mutex_lock(philo->next_fork);
	ft_print_message("has taken a fork", philo->id, philo->args);
	philo->dead = ft_get_time() - philo->args->time_to_die;
	ft_print_message("is eating", philo->id, philo->args);
	usleep(philo->args->time_to_sleep * 1000);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(philo->next_fork);
	ft_print_message("is sleeping", philo->id, philo->args);
	usleep(philo->args->time_to_sleep * 1000);
}

void	*ft_health_check(void *value)
{
	t_philo *philo;
	philo = (t_philo *) value;
	while(1)
	{
		if(ft_get_time() >= philo->dead + 5)
		{
			ft_print_message("died", philo->id, philo->args);
			pthread_mutex_lock(&philo->args->printing);
			philo->args->status = 1;
		}
		else if(philo->max_eat == 1)
		{
			philo->args->beats += 1;
			break;
		}
	}
	return (NULL);
	
}

void	*ft_routine(void *value)
{
	int i;
	t_philo *philo;
	pthread_t thread_h;

	philo = (t_philo *)value;
	philo->max_eat = 0;
	i = 0;
	philo->dead = philo->args->time + philo->args->time_to_die;
	pthread_create(&thread_h, NULL, &ft_health_check, philo);
	pthread_detach(thread_h);
	while(i < philo->args->each_time || !(philo->args->each_time))
	{
		ft_philo_activity(philo);
		i++;
		if(i == philo->args->each_time)
			ft_print_message("is thinking", philo->id, philo->args);
	}
	philo->max_eat = 1;
	return (NULL);
}



t_philo *ft_create_threads(t_args *args)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = malloc(sizeof(t_philo) * args->philo_num);
	if (!philo)
    	return (printf("Threads allocation error\n"), NULL);
	while (i < args->philo_num)
	{
		pthread_mutex_init(&philo[i].fork, NULL);
		i++;
	}
	i = 0;
	ft_assign_values(philo, args);
	args->time = ft_get_time();
	while (i < args->philo_num)
	{
		if (pthread_create(&philo[i].thread_id, NULL, &ft_routine, &philo[i]))
			return (printf("An error has been occured when creating the thread\n"), NULL); // i should free here !!
		usleep(60);
		i++;
	}
	i = 0;
	while (i < args->philo_num)
	{
		pthread_detach(philo[i].thread_id);
		i++;
	}
	return (philo);
}

int	main(int argc, char **argv)
{
	t_args args;
	t_philo *philo;
    int i;
	args.status = 0;
	args.beats = 0;
	if (!ft_check_input(argc, argv))
		return (printf("Wrong input\n"), 0);
	ft_assign(argc, argv, &args);
	pthread_mutex_init(&args.printing, NULL);
	philo = ft_create_threads(&args);
	if(!philo)
		return (0);
	while(args.status == 0)
	{
		if(args.beats == args.philo_num)
			break;
		sleep(50);
	}
	i = 0;
	while(i < args.philo_num)
	{
		pthread_mutex_destroy(&philo[i].fork);
		i++;
	}
	pthread_mutex_destroy(&args.printing);
	//free(philo);
//	system("leaks a.out");
    return (0);
}