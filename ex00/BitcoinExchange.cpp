/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 21:58:58 by vinda-si          #+#    #+#             */
/*   Updated: 2026/09/07 20:16:27 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

static std::string trim(const std::string& s)
{
	size_t start = s.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return "";
	size_t end = s.find_last_not_of(" \t\r\n");
	return s.substr(start, end - start + 1);
}

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _db(other._db) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
		_db = other._db;
	return *this;
}
BitcoinExchange::~BitcoinExchange() {}

static bool isAllDigits(const std::string& s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.length(); ++i)
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return false;
	return true;
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	std::string yearStr = date.substr(0, 4);
	std::string monthStr = date.substr(5, 2);
	std::string dayStr = date.substr(8, 2);
	if (!isAllDigits(yearStr) || !isAllDigits(monthStr) || !isAllDigits(dayStr))
		return false;

	int	year = std::atoi(yearStr.c_str());
	int	month = std::atoi(monthStr.c_str());
	int	day = std::atoi(dayStr.c_str());

	if (year < 1 || month < 1 || month > 12 || day < 1)
		return false;

	int	daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	bool	leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (month == 2 && leap)
		daysInMonth[1] = 29;
	if (day > daysInMonth[month - 1])
		return false;
	return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, float& value) const
{
	if (valueStr.empty())
		return false;

	char	*end;
	value = static_cast<float>(std::strtod(valueStr.c_str(), &end));

	if (end == valueStr.c_str() || *end != '\0')
		return false;
	if (value == 0.0f)
		value = 0.0f;
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
			std::string date = trim(line.substr(0, comma));
			float rate = static_cast<float>(std::strtod(line.substr(comma + 1).c_str(), NULL));
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
		if (trim(line).empty())
			continue;

		size_t	pipe = line.find('|');
		if (pipe == std::string::npos)
		{
			std::cerr << "Error: bad input => " << trim(line) << std::endl;
			continue;
		}
		std::string date = trim(line.substr(0, pipe));
		std::string valStr = trim(line.substr(pipe + 1));

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
