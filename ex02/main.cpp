/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 22:57:55 by vinda-si          #+#    #+#             */
/*   Updated: 2026/08/09 23:00:27 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}

	PmergeMe sorter;
	if (!sorter.parseArguments(argc, argv))
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}

	sorter.sortAndMeasure();
	sorter.displayResults();

	return (0);
}
