/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 21:53:55 by vinda-si          #+#    #+#             */
/*   Updated: 2026/08/09 22:57:33 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : _timeVec(0), _timeDeq(0) {}
PmergeMe::PmergeMe(const PmergeMe& other) : _vec(other._vec), _deq(other._deq),
		_timeVec(other._timeVec), _timeDeq(other._timeDeq) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
		_timeVec = other._timeVec;
		_timeDeq = other._timeDeq;
	}
	return *this;
}
PmergeMe::~PmergeMe() {}

double PmergeMe::getTime() const
{
	struct timeval time;
	gettimeofday (&time, NULL);
	return (time.tv_sec * 1000000.0) + time.tv_usec;
}

template <typename Container>
void PmergeMe::mergeInsertSort(Container& c)
{
	if (c.size() <= 1)
		return;

	Container mainChain;
	Container pend;

	for (size_t i = 0; i < c.size(); i += 2)
	{
		if (i + 1 < c.size())
		{
			if (c[i] > c[i + 1])
			{
				mainChain.push_back(c[i]);
				pend.push_back(c[i + 1]);
			}
			else
			{
				mainChain.push_back(c[i + 1]);
				pend.push_back(c[i]);
			}
		}
		else
		{
			pend.push_back(c[i]);
		}
	}
	mergeInsertSort(mainChain);

	for (size_t i = 0; i < pend.size(); ++i)
	{
		typename Container::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), pend[i]);
		mainChain.insert(it, pend[i]);
	}
	c = mainChain;
}

bool PmergeMe::parseArguments(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg.empty() || arg.find_first_not_of("0123456789 ") != std::string::npos)
			return false;

		std::istringstream iss(arg);
		int val;
		while(iss >> val)
		{
			if (val < 0)
				return false;
			_vec.push_back(val);
			_deq.push_back(val);
		}
	}
	return true;
}

template <typename Container>
void PmergeMe::printContainer(const Container& c, const std::string& prefix) const
{
	std::cout << prefix;
	size_t limit = c.size() > 5 ? 5 : c.size();
	for (size_t i = 0; i < limit; ++i)
		std::cout << c[i] << " ";
	if (c.size() > 5)
		std::cout << "[...]";
	std::cout << std::endl;
}

void PmergeMe::sortAndMeasure()
{
	printContainer(_vec, "Before: ");

	double start = getTime();
	mergeInsertSort(_vec);
	_timeVec = getTime() - start;

	start = getTime();
	mergeInsertSort(_deq);
	_timeDeq = getTime() - start;
}

void PmergeMe::displayResults() const
{
	printContainer(_vec, "After: ");
	std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector : " << _timeVec << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size() << " elements with std::deque : " << _timeDeq << " us" << std::endl;
}
