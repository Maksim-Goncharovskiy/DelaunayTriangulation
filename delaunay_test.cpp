#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <delaunay/delaunay.hpp>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Insufficient number of arguments" << std::endl;
	}
	if (argc == 2) {
		std::ifstream input_file;
		std::string input_file_name = argv[1];
		input_file.open(input_file_name);

		std::ofstream output_file;
		output_file.open(R"(..\results\visualisation.tex)", std::ofstream::out | std::ofstream::trunc);
		int N = 0;
		input_file >> N;
		std::vector<delaunay::Point> input;
		for (int i = 0; i < N; i++) {
			double x = 0;
			double y = 0;
			input_file >> x >> y;
			input.push_back(delaunay::Point(x, y));
		}
		std::vector<delaunay::Triangle> result = delaunay::getTriangulation(input, output_file);
	}
	if (argc >= 2) {
		std::ifstream input_file;
		std::string input_file_name = argv[1];
		input_file.open(input_file_name);

		std::ofstream output_file;
		std::string output_file_name = argv[2];
		output_file.open(output_file_name, std::ofstream::out | std::ofstream::trunc);
		int N = 0;
		input_file >> N;
		std::vector<delaunay::Point> input;
		for (int i = 0; i < N; i++) {
			double x = 0;
			double y = 0;
			input_file >> x >> y;
			input.push_back(delaunay::Point(x, y));
		}
		std::vector<delaunay::Triangle> result = delaunay::getTriangulation(input, output_file);
	}
}