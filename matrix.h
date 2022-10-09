#pragma once
#include <array>
#include <vector>
#include <iostream>
#include <format>
#include <string>
#include <chrono>
#include <random>



class timer
{
	using clock_t = std::chrono::high_resolution_clock;
	using microseconds = std::chrono::microseconds;
public:
	timer();
	double stop();
	~timer();
private:
	const clock_t::time_point start_;
};

struct InitValue
{
	double init_value;
};

struct NormalDistributionParams
{
	double mu;
	double sigma;
};

template<size_t M, size_t N>
	requires (M > 1 && N == 1) || (M == 1 && N > 1) || (M > 1 && N > 1)
class matrix
{
public:
	matrix() = delete;

	matrix(const matrix<M, N>& other) :_container(other._container){}
	matrix(matrix<M, N>&& other)
	{
		_container = std::move(other._container);
	}
	matrix(InitValue init_value)
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] = init_value.init_value;
			}
		}
	}
	matrix(NormalDistributionParams normal_distribution_params)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::normal_distribution<double> d(normal_distribution_params.mu,normal_distribution_params.sigma);
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] = d(gen);
			}
		}
	}

	auto& operator[](size_t i)&
	{
		return _container[i];
	}
	auto& operator=(matrix<M, N>&& other)&
	{
		_container = std::move(other._container);
		return *this;
	}
	auto T()
	{
		matrix<N, M> new_matrix({ .init_value = 0.0 });
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				new_matrix._container[j][i] = _container[i][j];
			}
		}
		return new_matrix;
	}

	auto&& operator+(matrix<M, N>&& other) &&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j]+= other._container[i][j];
			}
		}
		return std::move(*this);
	}
	auto&& operator+(matrix<M, N>&& other) &
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				other._container[i][j] += _container[i][j];
			}
		}
		return std::move(other);
	}
	auto&& operator+(matrix<M, N>& other)&&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] += other._container[i][j];
			}
		}
		return std::move(*this);
	}
	auto operator+(matrix<M, N>& other)&
	{
		matrix<M, N> new_matrix({ .init_value = 0.0 });
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				new_matrix._container[i][j] = _container[i][j] + other._container[i][j];
			}
		}
		return new_matrix;
	}

	auto&& operator-(matrix<M, N>&& other)&&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] -= other._container[i][j];
			}
		}
		return std::move(*this);
	}
	auto&& operator-(matrix<M, N>&& other)&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				other._container[i][j] = _container[i][j] - other._container[i][j];
			}
		}
		return std::move(other);
	}
	auto&& operator-(matrix<M, N>& other)&&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] -= other._container[i][j];
			}
		}
		return std::move(*this);
	}
	auto operator-(matrix<M, N>& other)&
	{
		matrix<M, N> new_matrix({ .init_value = 0.0 });
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				new_matrix._container[i][j] = _container[i][j] - other._container[i][j];
			}
		}
		return new_matrix;
	}

	void operator+=(matrix<M, N>&& other)&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] += other._container[i][j];
			}
		}
	}
	void operator+=(matrix<M, N>& other)&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] += other._container[i][j];
			}
		}
	}
	void operator*=(double alpha)
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] *= alpha;
			}
		}
	}

	void PrintSelf() const 
	{
		std::cout << "\t|";
		for (size_t j = 0; j < N; j++)
		{
			if (j == N - 1)
			{
				std::cout << std::format("{:.5f}", _container[0][j]);
			}
			else {
				std::cout << std::format("{:.5f} ", _container[0][j]);
			}
		}
		std::cout << std::format("|{}x{}",M,N) << std::endl;

		for (size_t i = 1; i < M; i++)
		{
			std::cout << "\t|";
			for (size_t j = 0; j < N; j++)
			{
				if (j == N - 1)
				{
					std::cout << std::format("{:.5f}", _container[i][j]);
				}
				else {
					std::cout << std::format("{:.5f} ", _container[i][j]);
				}

			}
			if (i==M-1)
			{
				std::cout << "|";
			}
			else {
				std::cout << "|" << std::endl;
			}
			

		}
	}

	std::array<std::array<double,N>,M> _container;
};

template<size_t M, size_t N1, size_t N2>
auto operator*(matrix<M, N1>&& A1, matrix<N1, N2>&& A2)
{
	matrix<M, N2> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N2; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < N1; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}
template<size_t N>
double operator*(matrix<1, N>&& A1, matrix<N,1>&& A2)
{
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
	{
		sum += A1._container[0][i] * A2._container[i][0];
	}
	return sum;
}
template<size_t M>
auto&& operator*(matrix<M, M>&& A1, matrix<M, M>&& A2)
{
	for (size_t i = 0; i < M; i++)
	{	
		std::array<double,M> buffer;
		for (size_t j = 0; j < M; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < M; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			buffer[j] = sum;
		}
		A1._container[i] = buffer;
	}
	return std::move(A1);
}

template<size_t M, size_t N1, size_t N2>
auto operator*(matrix<M, N1>& A1, matrix<N1, N2>& A2)
{
	matrix<M, N2> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N2; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < N1; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}
template<size_t N>
double operator*(matrix<1, N>& A1, matrix<N, 1>& A2)
{
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
	{
		sum += A1._container[0][i] * A2._container[i][0];
	}
	return sum;
}
template<size_t M>
auto operator*(matrix<M, M>& A1, matrix<M, M>& A2)
{
	matrix<M, M> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < M; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < M; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}

template<size_t M, size_t N1, size_t N2>
auto operator*(matrix<M, N1>& A1, matrix<N1, N2>&& A2)
{
	matrix<M, N2> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N2; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < N1; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}
template<size_t N>
double operator*(matrix<1, N>& A1, matrix<N, 1>&& A2)
{
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
	{
		sum += A1._container[0][i] * A2._container[i][0];
	}
	return sum;
}
template<size_t M>
auto operator*(matrix<M, M>& A1, matrix<M, M>&& A2)
{
	matrix<M, M> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < M; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < M; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}

template<size_t M, size_t N1, size_t N2>
auto operator*(matrix<M, N1>&& A1, matrix<N1, N2>& A2)
{
	matrix<M, N2> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N2; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < N1; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}
template<size_t N>
double operator*(matrix<1, N>&& A1, matrix<N, 1>& A2)
{
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
	{
		sum += A1._container[0][i] * A2._container[i][0];
	}
	return sum;
}
template<size_t M>
auto&& operator*(matrix<M, M>&& A1, matrix<M, M>& A2)
{
	for (size_t i = 0; i < M; i++)
	{
		std::array<double,M> buffer;
		for (size_t j = 0; j < M; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < M; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			buffer[j] = sum;
		}
		A1._container[i] = buffer;
	}
	return std::move(A1);
}

template<size_t M,size_t N>
auto&& operator*(double alpha, matrix<M, N>&& A)
{
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			A._container[i][j] *= alpha;
		}
	}
	return std::move(A);
}
template< size_t M, size_t N>
auto operator*(double alpha, matrix<M, N>& A)
{
	matrix<M, N> new_matrix({.init_value=0.0});
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			new_matrix._container[i][j] = alpha*A._container[i][j];
		}
	}
	return new_matrix;
}


template<typename Last>
void print_wstrings(Last last)
{
	std::wcout << last;
}
template<typename First, typename ...Rest>
void print_wstrings(First first, Rest... rest)
{
	std::wcout << first << " ";
	print_wstrings(rest ...);
}
template<typename ...PrintableData>
void print(PrintableData... data)
{
	std::wcout << "\t";
	print_wstrings(data ...);
	std::wcout << std::endl;
}
template<size_t M, size_t N>
void print(const matrix<M, N>& matrix)
{
	matrix.PrintSelf();
	std::cout << '\n' << std::endl;
}

template<size_t M, size_t N>
	requires (M == N)
auto inv(const matrix<M, N>& A)
{
	matrix<M, M> L(A);
	matrix<M, M> R({.init_value=0.0});
	for (size_t i = 0; i < M; i++)
	{
		R[i][i] = 1.0;
	}
	for (size_t k = 0; k < M; k++)
	{
		double first_value = L[k][k];
		L[k][k] = 1.0;
		for (size_t j = k+1; j < M; j++)
		{
			L[k][j] /= first_value;
		}
		for (size_t j = 0; j <= k; j++)
		{
			R[k][j] /= first_value;
		}
		for (size_t i = k+1; i < M; i++)
		{
			double left_value_i = L[i][k];
			L[i][k] = 0.0;
			for (size_t j = k+1; j < M; j++)
			{
				L[i][j] -= L[k][j]*left_value_i;
			}
			for (size_t j = 0; j <=k; j++)
			{
				R[i][j] -= R[k][j] * left_value_i;
			}
		}
		for (int i = (int)k-1; i>=0; i--)
		{
			double left_value_i = L[i][k];
			L[i][k] = 0.0;
			for (size_t j = k+1; j < M ; j++)
			{
				L[i][j] -= L[k][j] * left_value_i;
			}
			for (size_t j = 0; j <= k; j++)
			{
				R[i][j] -= R[k][j] * left_value_i;
			}
		}
	}
	return R;
}

template<size_t M, size_t N>
	requires ((N == 1) && (M > 1)) || ((M == 1) && (N > 1))
auto linspace(double a, double b)
{
	if constexpr ((N ==1) && (M > 1)) {
		matrix<M, 1> _matrix({ .init_value = 0.0 });
		double h = (b - a) / ((double)M - 1.0);
		for (size_t i = 0; i < M; i++)
		{
			_matrix[i][0] = a + (double)i * h;
		}
		return _matrix;
	}
	else if ((M == 1) && (N > 1)) 
	{
		matrix<1, N> _matrix({ .init_value = 0.0 });
		double h = (b - a) / ((double)N - 1.0);
		for (size_t i = 0; i < N; i++)
		{
			_matrix[0][i] = a + (double)i * h;
		}
		return _matrix;
	}

}

template<size_t M, size_t N>
	requires ((N == 1) && (M > 1)) || ((M == 1) && (N > 1))
double sum(matrix<M,N>& _vec) 
{
	if constexpr ((N == 1) && (M > 1)) {
		double sum = 0.0;
		for (size_t i = 0; i < M; i++)
		{
			sum += _vec[i][0];
		}
		return sum;
	}
	else if ((M == 1) && (N > 1))
	{
		double sum = 0.0;
		for (size_t i = 0; i < N; i++)
		{
			sum += _vec[0][i];
		}
		return sum;
	}
}

std::vector<double> pointwise_abs(const std::vector<double>& input);
std::vector<double> pointwise_minus(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> pointwise_plus(const std::vector<double>& v1, const std::vector<double>& v2);