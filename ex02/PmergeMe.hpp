/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 21:46:09 by vinda-si          #+#    #+#             */
/*   Updated: 2026/09/07 20:21:55 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <utility>
#include <string>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <sys/time.h>
#include <algorithm>

class PmergeMe
{
	private:
		std::vector<int> _vec;
		std::deque<int> _deq;
		double _timeVec;
		double _timeDeq;

		// Recursive merge step (Ford-Johnson): sorts a sequence of
		// (winner, loser) pairs by their winner. Instantiated once per
		// container type through sortVector() / sortDeque().
		template <typename PairSeq>
		void sortPairsByWinner(PairSeq& pairs);

		template <typename Container>
		void printContainer(const Container& c, const std::string& prefix) const;

		static std::vector<size_t> jacobsthalOrder(size_t n);

		double getTime() const;

		void sortVector();
		void sortDeque();

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
