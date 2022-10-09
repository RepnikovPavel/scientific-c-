#pragma once
#include <string>
#include <vector>
#include <queue>
#include <atomic>
#include <mutex>

struct DoubleLine
{
	DoubleLine(std::vector<double> _upper_line, std::vector<double> _lower_line)
	{
		upper_line = _upper_line;
		lower_line = _lower_line;
	}
	std::vector<double> upper_line;
	std::vector<double> lower_line;
};

struct DrawParamsForOneLine 
{	
	std::vector<double> x;
	std::vector<double> y;
	std::string mode = "lines+markers";
	std::string line_name = "";
	std::string line_color = "#000000";
	std::string line_style = "solid";
	std::string line_width = "3";
	std::string marker_color = "#162CF3";
	std::string marker_size = "8";

	std::vector<
		DoubleLine
	> errors_lines_data;
	std::vector<std::string> error_lines_names;
	std::vector<std::string> error_lines_colors;
	std::vector<std::string> error_lines_style;
	std::vector<std::string> error_lines_width;
};

struct DrawRequestParams_2D_line_plot_with_errors
{
	std::string board_id="";
	std::string upper_title = "";
	std::string x_label = "";
	std::string y_label = "";
	std::vector<DrawParamsForOneLine> lines;
};


class ConnectionsManagerInTheCurrentThread final
{
public:		ConnectionsManagerInTheCurrentThread();
public:		~ConnectionsManagerInTheCurrentThread();
public:		void StartListenServer();

public:		void PushDrawRequest_2D_line_plot_with_errors(
				DrawRequestParams_2D_line_plot_with_errors&& DrawRequestParams);
public:		void WaitUntilAllRenderingRequestsAreSent();

private:	std::atomic<int>			_connections_count;
private:	bool						_need_stop_server;
private:	bool						_user_try_make_connection;
private:	bool						_is_it_possible_to_add_to_the_queue;
private:	bool						_everything_that_needed_to_be_written_to_the_socket_is_written;

private:	std::queue<std::string>		_queue_for_rendering;
private:	std::mutex					_queue_mutex;
};