/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:50 by grohr             #+#    #+#             */
/*   Updated: 2025/04/18 22:24:46 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//clean_up:
//- Détruit tous les mutexes des fourchettes avec pthread_mutex_destroy.
//- Détruit les mutexes pour les logs et la vérification des morts.
//- Libère la mémoire allouée pour les fourchettes et les philosophes.
//- pthread_mutex_destroy libère les ressources des mutexes après utilisation.
//
static void	clean_up(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	free(data->forks);
	free(data->philos);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	supervisor;

	//easy
	if (!init_parse_args(argc, argv, &data))
		return (printf("Invalid arguments\n"), 1);
	data.simulation_stop = 0;
	//ok->repasser vite fait sur les notes
	data.start_time = get_time();
	//ok-> essayer d'approfondir init_mutexes
	if (!init_mutexes(&data) || !init_philos(&data))
		return (printf("Initialization failed\n"), 1);
	if (!create_threads(&data, &supervisor))
		return (printf("Thread creation failed\n"), 1);
	join_threads(&data, supervisor);
	clean_up(&data);
	return (0);
}
