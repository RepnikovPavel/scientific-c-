#include "matrix.h"
#include <cmath>

timer::timer() : start_(clock_t::now()) {};
double timer::stop()
{
	const auto finish = clock_t::now();
	const auto us =
		std::chrono::duration_cast<microseconds>
		(finish - start_).count();
	return us / 1000000.0;
}
timer::~timer() {};

std::vector<double> pointwise_abs(const std::vector<double>& input)
{
	std::cout << "in poiuntwise abs" << std::endl;

	if (input.size() == 0)
	{
		throw std::logic_error("argument of abs function is empty");
	}
	std::vector<double> output;
	output.reserve(input.size());
	for (size_t i = 0; i < input.size(); i++)
	{
		output.push_back(std::fabs(input[i]));
	}
	return output;
}

std::vector<double> pointwise_minus(const std::vector<double>& v1, const std::vector<double>& v2)
{

	std::cout << "in poiuntwise minus" << std::endl;
	if (v1.size() != v2.size())
	{
		throw std::logic_error("arguments of operator- has different shapes");
	}
	if (v1.size()==0)
	{
		throw std::logic_error("left argument of operator- is empty");
	}
	if (v2.size() == 0)
	{
		throw std::logic_error("right argument of operator- is empty");
	}
	std::vector<double> output;
	output.reserve(v1.size());
	for (size_t i = 0; i < v1.size(); i++)
	{
		output.push_back(v1[i] - v2[i]);
	}
	return output;
}
std::vector<double> pointwise_plus(const std::vector<double>& v1, const std::vector<double>&v2)
{

	std::cout << "in poiuntwise plus" << std::endl;

	if (v1.size() != v2.size())
	{
		throw std::logic_error("arguments of operator- has different shapes");
	}
	if (v1.size() == 0)
	{
		throw std::logic_error("left argument of operator- is empty");
	}
	if (v2.size() == 0)
	{
		throw std::logic_error("right argument of operator- is empty");
	}
	std::vector<double> output;
	output.reserve(v1.size());
	for (size_t i = 0; i < v1.size(); i++)
	{
		output.push_back(v1[i] + v2[i]);
	}
	return output;
}