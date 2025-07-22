/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:50 by grohr             #+#    #+#             */
/*   Updated: 2025/07/22 18:16:32 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//print_meal_stats:
//- Affiche le nombre de repas mangés par chaque philosophe à la fin
//- Permet de vérifier l'équité de la distribution des repas
//- Affiche aussi le total des repas et la moyenne
//
static void	print_meal_stats(t_data *data)
{
	int	i;
	int	total_meals;

	total_meals = 0;
	printf("\n=== MEAL STATISTICS ===\n");
	i = 0;
	while (i < data->num_philos)
	{
		printf("Philosopher %d ate %d times\n", 
			data->philos[i].id, data->philos[i].meals_eaten);
		total_meals += data->philos[i].meals_eaten;
		i++;
	}
	printf("Total meals: %d\n", total_meals);
	printf("Average meals per philosopher: %.2f\n", 
		(float)total_meals / data->num_philos);
	if (data->max_meals > 0)
		printf("Target meals per philosopher: %d\n", data->max_meals);
	printf("=======================\n");
}

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
	{
		return (printf("Thread creation failed\n"), 1);
	}
	join_threads(&data, supervisor);
	
	// Afficher les statistiques des repas avant le nettoyage
	print_meal_stats(&data);
	
	clean_up(&data);
	return (0);
}
