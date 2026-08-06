/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 21:58:58 by vinda-si          #+#    #+#             */
/*   Updated: 2026/08/05 22:43:02 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _db(other._db) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
		_db = other._db;
	return *this;
}
BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
	if (date.length() != 10 || date[4] != '_' || date[7] != '-')
		return false;

	int	year = std::atoi(date.substr(0, 4).c_str());
	int	month = std::atoi(date.substr(5, 2).c_str());
	int	day = std::atoi(date.substr(8, 2).c_str());

	if (year < 2009 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;
	return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, float& value) const
{
	char	*end;
	value = std::strtof(valueStr.c_str(), &end);

	if (*end != '\0' && *end != 'f')
		return false;
	return true;
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
	std::ifstream file(filename.c_str());

	if (!file.is_open())
	{
		std::cerr << "Error: could not open database file." << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line);
	
	while (std::getline(file, line))
	{
		size_t comma = line.find(',');
		if (comma != std::string::npos)
		{
			std::string date = line.substr(0, comma);
			float rate = std::strtof(line.substr(comma + 1).c_str(), NULL);
			_db[date] = rate;
		}
	}
}

void BitcoinExchange::processInput(const std::string& filename) const
{
	std::ifstream file(filename.c_str());

	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		size_t	pipe = line.find('|');
		if (pipe == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		std::string date = line.substr(0, pipe - 1);
		std::string valStr = line.substr(pipe + 2);

		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}
		float	val;

		if (!isValidValue(valStr, val) || val < 0)
		{
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}

		if (val > 1000)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}

		std::map<std::string, float>::const_iterator it = _db.lower_bound(date);
		if (it != _db.end() && it->first == date)
		{
			std::cout << date << " => " << val << " = " << val * it->second << std::endl;
		}
		else if (it != _db.begin())
		{
			--it;
			std::cout << date << " => " << val << " = " << val * it->second << std::endl;
		}
		else
		{
			std::cerr << "Error: date not found in database." << std::endl;
		}
	}
}
