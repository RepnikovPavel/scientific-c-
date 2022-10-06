#include "matrix.h"

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
