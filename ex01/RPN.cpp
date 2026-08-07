/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 22:29:22 by vinda-si          #+#    #+#             */
/*   Updated: 2026/08/06 23:04:58 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN() {}
RPN::RPN(const RPN& other) : _stack(other._stack) {}
RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
		_stack = other._stack;
	return *this;
}
RPN::~RPN() {}

void RPN::evaluate(const std::string& expression)
{
	std::istringstream iss(expression);
	std::string token;

	while (iss >> token)
	{
		if (token == "+" || token == "-" || token == "*" || token == "/")
		{
			if (_stack.size() < 2)
			{
				std::cerr << "Error" << std::endl;
				return;
			}
			int	val2 = _stack.top(); _stack.pop();
			int val1 = _stack.top(); _stack.pop();

			if (token == "+")
				_stack.push(val1 + val2);
			else if (token == "-")
				_stack.push(val1 - val2);
			else if (token == "*")
				_stack.push(val1 * val2);
			else if (token == "/")
			{
				if (val2 == 0)
				{
					std::cerr << "Error" << std::endl;
					return;
				}
				_stack.push(val1 / val2);
			}
		}	
		else
		{
			if (token.length() != 1 || !isdigit(token[0]))
			{
				std::cerr << "Error" << std::endl;
				return;
			}
			_stack.push(token[0] - '0');
		}
	}
	if (_stack.size() != 1)
	{
		std::cerr << "Error" << std::endl;
	}
	else
	{
		std::cout << _stack.top() << std::endl;
	}
}
