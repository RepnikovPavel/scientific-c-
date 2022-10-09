// c++20 requires
#include <vector>
#include <cmath>
#include "matrix.h"
#include "Connection.h"

template<size_t M ,size_t N>
requires (N==1)
class func_vec
{
public: 
	func_vec() :_matrix({.init_value=0.0})
	{}
	auto& operator()(double x)
	{
		for (size_t i = 0; i < M; i++)
		{
			_matrix[i][0] = std::pow(x, i);
		}
		return _matrix;
	}
private:	matrix<M, 1> _matrix;
};

#define x_start -1.8
#define x_stop 0.8
#define m 5
#define sample_size 20
#define _sigma 1.0

int main()
{
	try {
		/////////////////////////////data////////////////////////
		func_vec<m, 1> F;
		matrix<m, 1> a({ .init_value = 0.0 });
		a[0][0] = 1.0;
		a[1][0] = 1.0;
		a[2][0] = -3.0;
		a[3][0] = -10.0;
		a[4][0] = -5.0;
		matrix<sample_size, 1> X = linspace<sample_size, 1>(x_start, x_stop);
		matrix<sample_size, 1> true_Y({ .init_value = 0.0 });
		matrix<sample_size, 1> Y({ .init_value = 0.0 });
		matrix<sample_size, 1> epsilon({ .mu = 0.0,.sigma = _sigma });
		for (size_t i = 0; i < sample_size; i++)
		{
			true_Y[i][0] = a.T() * F(X[i][0]);
			Y[i][0] = (a.T() * F(X[i][0])) + epsilon[i][0];
		}
		////////////////////////////buffers//////////////////////
		matrix<m, 1> v({ .init_value = 0.0 });
		matrix<m, m> T({ .init_value = 0.0 });
		double V = 0.0;
		matrix<sample_size, m> F_x({ .init_value = 0.0 });
		///////////////////////////est///////////////////////////
		for (size_t i = 0; i < sample_size; i++)
		{
			auto tmp = F(X[i][0]);
			v += Y[i][0] * tmp;
			T += F(X[i][0]) * tmp.T();
			V += std::pow(Y[i][0], 2);
			for (size_t j = 0; j < m; j++)
			{
				F_x[i][j] = tmp[j][0];
			}

		}
		auto Y_est = F_x * inv(T) * v;
		/////////////////////////drawing/////////////////////////
		std::vector<double> x;
		std::vector<double> y;
		std::vector<double> y_true;
		std::vector<double> y_est;
		std::vector<double> true_epsilon;
		std::vector<double> est_epsilon;

		for (size_t i = 0; i < sample_size; i++)
		{
			x.push_back(X[i][0]);
			y.push_back(Y[i][0]);
			y_true.push_back(true_Y[i][0]);
			y_est.push_back(Y_est[i][0]);
			true_epsilon.push_back(epsilon[i][0]);
			est_epsilon.push_back(2 * epsilon[i][0]);
		}
		std::cout << "create class listener" << std::endl;
		ConnectionsManagerInTheCurrentThread magic;
		std::cout << "start  listener" << std::endl;
		magic.StartListenServer();
		auto line1 = DrawParamsForOneLine
		{
			.x = x,
			.y = y_est,
			.mode = "lines",
			.line_name = "$est. f$",
			.line_color = "#000000",
			.line_style = "solid",
			.line_width = "5",
			.errors_lines_data =
			{
				DoubleLine(pointwise_plus(y,pointwise_abs(true_epsilon)),pointwise_minus(y,pointwise_abs(true_epsilon))),
				DoubleLine(pointwise_plus(y,pointwise_abs(est_epsilon)),pointwise_minus(y,pointwise_abs(est_epsilon)))
			},
			.error_lines_names = { "$Error$","$est. Error$" },
			.error_lines_colors = { "#006600","#006600" },
			.error_lines_style = { "solid","dashdot" },
			.error_lines_width = {"4","4"}
		};

		auto line2 = DrawParamsForOneLine
		{
			.x = x,
			.y = y,
			.mode = "markers",
			.line_name = "$Data$",
			.marker_color = "#0000ff",
			.marker_size ="10",

		};

		auto line3 = DrawParamsForOneLine
		{
			.x = x,
			.y = y_true,
			.mode = "lines",
			.line_name = "$f$",
			.line_color = "#cc3300",
			.line_style = "solid",
			.line_width = "5",
		};


		std::cout << "push draw request" << std::endl;
		magic.PushDrawRequest_2D_line_plot_with_errors(
			{
				.board_id = "some_name",
				.upper_title ="$upper title$",
				.x_label = "$s_{o_{m_{e}}} cheats$",
				.y_label = "$y_label$",
				.lines = {line1,line2,line3}
			});

		magic.WaitUntilAllRenderingRequestsAreSent();
	}
	catch (std::logic_error err)
	{
		std::cout << std::format("\tlogic error:\t{}", err.what()) << std::endl;
	}
	catch (...)
	{
		std::cout << "\tunexpected error" << std::endl;
	}
}