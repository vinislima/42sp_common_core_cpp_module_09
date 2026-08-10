/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 21:46:09 by vinda-si          #+#    #+#             */
/*   Updated: 2026/08/09 21:53:33 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <cstdlib>
#include <sys/time.h>
#include <algorithm>

class PmergeMe
{
	private:
		std::vector<int> _vec;
		std::deque<int> _deq;
		double _timeVec;
		double _timeDeq;

		template <typename Container>
		void mergeInsertSort(Container& c);

		template <typename Container>
		void printContainer(const Container& c, const std::string& prefix) const;

		double getTime() const;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();

		bool parseArguments(int argc, char** argv);
		void sortAndMeasure();
		void displayResults() const;
};

#endif
