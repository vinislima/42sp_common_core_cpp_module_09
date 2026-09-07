/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 21:53:55 by vinda-si          #+#    #+#             */
/*   Updated: 2026/09/07 20:21:45 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <cerrno>

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
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000.0) + time.tv_usec;
}

bool PmergeMe::parseArguments(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::istringstream iss(argv[i]);
		std::string tok;

		while (iss >> tok)
		{
			if (tok.find_first_not_of("0123456789") != std::string::npos)
				return false;

			errno = 0;
			char* end = 0;
			long value = std::strtol(tok.c_str(), &end, 10);
			if (*end != '\0' || errno == ERANGE || value < 0 || value > INT_MAX)
				return false;

			_vec.push_back(static_cast<int>(value));
			_deq.push_back(static_cast<int>(value));
		}
	}
	return !_vec.empty();
}

std::vector<size_t> PmergeMe::jacobsthalOrder(size_t n)
{
	std::vector<size_t> order;
	if (n == 0)
		return order;

	std::vector<size_t> jac;
	jac.push_back(1);
	jac.push_back(1);
	while (jac.back() - 1 < n)
		jac.push_back(jac[jac.size() - 1] + 2 * jac[jac.size() - 2]);

	for (size_t k = 2; k < jac.size(); ++k)
	{
		size_t hi = jac[k] - 1;
		size_t lo = jac[k - 1] - 1;
		if (hi > n)
			hi = n;
		for (size_t i = hi; i > lo; --i)
			order.push_back(i - 1);
		if (hi == n)
			break;
	}
	return order;
}

template <typename PairSeq>
void PmergeMe::sortPairsByWinner(PairSeq& pairs)
{
	if (pairs.size() < 2)
		return;

	bool hasStraggler = (pairs.size() % 2 != 0);
	PairSeq mainChain;
	PairSeq pend;

	for (size_t i = 0; i + 1 < pairs.size(); i += 2)
	{
		if (pairs[i].first > pairs[i + 1].first)
		{
			mainChain.push_back(pairs[i]);
			pend.push_back(pairs[i + 1]);
		}
		else
		{
			mainChain.push_back(pairs[i + 1]);
			pend.push_back(pairs[i]);
		}
	}

	sortPairsByWinner(mainChain);

	for (size_t i = 0; i < pend.size(); ++i)
	{
		size_t lo = 0;
		size_t hi = mainChain.size();
		while (lo < hi)
		{
			size_t mid = lo + (hi - lo) / 2;
			if (mainChain[mid].first < pend[i].first)
				lo = mid + 1;
			else
				hi = mid;
		}
		mainChain.insert(mainChain.begin() + lo, pend[i]);
	}

	if (hasStraggler)
	{
		typename PairSeq::value_type s = pairs[pairs.size() - 1];
		size_t lo = 0;
		size_t hi = mainChain.size();
		while (lo < hi)
		{
			size_t mid = lo + (hi - lo) / 2;
			if (mainChain[mid].first < s.first)
				lo = mid + 1;
			else
				hi = mid;
		}
		mainChain.insert(mainChain.begin() + lo, s);
	}

	pairs = mainChain;
}

void PmergeMe::sortVector()
{
	double start = getTime();

	if (_vec.size() > 1)
	{
		bool hasStraggler = (_vec.size() % 2 != 0);
		int straggler = hasStraggler ? _vec[_vec.size() - 1] : 0;

		std::vector<std::pair<int, int> > pairs;
		for (size_t i = 0; i + 1 < _vec.size(); i += 2)
		{
			int a = _vec[i];
			int b = _vec[i + 1];
			if (a < b)
				std::swap(a, b);
			pairs.push_back(std::make_pair(a, b));
		}

		sortPairsByWinner(pairs);

		std::vector<int> chain;
		std::vector<int> pend;
		chain.push_back(pairs[0].second);
		for (size_t i = 0; i < pairs.size(); ++i)
		{
			chain.push_back(pairs[i].first);
			if (i > 0)
				pend.push_back(pairs[i].second);
		}

		std::vector<size_t> order = jacobsthalOrder(pend.size());
		for (size_t k = 0; k < order.size(); ++k)
		{
			size_t j = order[k];
			int value = pend[j];
			std::vector<int>::iterator bound =
				std::upper_bound(chain.begin(), chain.end(), pairs[j + 1].first);
			std::vector<int>::iterator pos =
				std::lower_bound(chain.begin(), bound, value);
			chain.insert(pos, value);
		}

		if (hasStraggler)
		{
			std::vector<int>::iterator pos =
				std::lower_bound(chain.begin(), chain.end(), straggler);
			chain.insert(pos, straggler);
		}

		_vec = chain;
	}

	_timeVec = getTime() - start;
}

void PmergeMe::sortDeque()
{
	double start = getTime();

	if (_deq.size() > 1)
	{
		bool hasStraggler = (_deq.size() % 2 != 0);
		int straggler = hasStraggler ? _deq[_deq.size() - 1] : 0;

		std::deque<std::pair<int, int> > pairs;
		for (size_t i = 0; i + 1 < _deq.size(); i += 2)
		{
			int a = _deq[i];
			int b = _deq[i + 1];
			if (a < b)
				std::swap(a, b);
			pairs.push_back(std::make_pair(a, b));
		}

		sortPairsByWinner(pairs);

		std::deque<int> chain;
		std::deque<int> pend;
		chain.push_back(pairs[0].second);
		for (size_t i = 0; i < pairs.size(); ++i)
		{
			chain.push_back(pairs[i].first);
			if (i > 0)
				pend.push_back(pairs[i].second);
		}

		std::vector<size_t> order = jacobsthalOrder(pend.size());
		for (size_t k = 0; k < order.size(); ++k)
		{
			size_t j = order[k];
			int value = pend[j];
			std::deque<int>::iterator bound =
				std::upper_bound(chain.begin(), chain.end(), pairs[j + 1].first);
			std::deque<int>::iterator pos =
				std::lower_bound(chain.begin(), bound, value);
			chain.insert(pos, value);
		}

		if (hasStraggler)
		{
			std::deque<int>::iterator pos =
				std::lower_bound(chain.begin(), chain.end(), straggler);
			chain.insert(pos, straggler);
		}

		_deq = chain;
	}

	_timeDeq = getTime() - start;
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
	sortVector();
	sortDeque();
}

void PmergeMe::displayResults() const
{
	printContainer(_vec, "After:  ");
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Time to process a range of " << _vec.size()
		<< " elements with std::vector : " << _timeVec << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size()
		<< " elements with std::deque  : " << _timeDeq << " us" << std::endl;
}
