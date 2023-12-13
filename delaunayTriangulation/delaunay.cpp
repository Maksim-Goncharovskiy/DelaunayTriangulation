#include <delaunay/delaunay.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> 
delaunay::Point::Point(double x, double y) {
	x_ = x;
	y_ = y;
}

delaunay::Edge::Edge(Point* A, Point* B) {
	A_ = A;
	B_ = B;
}

bool delaunay::Edge::operator==(const Edge& other) {
	if ((A_ == other.A_ && B_ == other.B_) || (A_ == other.B_ && B_ == other.A_)) {
		return true;
	}
	return false;
}

delaunay::Triangle::Triangle(Point* A, Point* B, Point* C) {
	A_ = A;
	B_ = B;
	C_ = C;
	AB_ = Edge(A, B);
	BC_ = Edge(B, C);
	AC_ = Edge(A, C);
}


bool delaunay::InCircle(const Triangle& tr, const Point& p) {
	double A_x = tr.A_->x_;
	double A_y = tr.A_->y_;
	double B_x = tr.B_->x_;
	double B_y = tr.B_->y_;
	double C_x = tr.C_->x_;
	double C_y = tr.C_->y_;

	double d = (A_x * (B_y - C_y) + B_x * (C_y - A_y) + C_x * (A_y - B_y));
	double x = ((A_x * A_x + A_y * A_y) * (B_y - C_y) + (B_x * B_x + B_y * B_y) * (C_y - A_y) + (C_x * C_x + C_y * C_y) * (A_y - B_y)) / (2 * d);
	double y = ((A_x * A_x + A_y * A_y) * (C_x - B_x) + (B_x * B_x + B_y * B_y) * (A_x - C_x) + (C_x * C_x + C_y * C_y) * (B_x - A_x)) / (2 * d);
	double r = sqrt((x - A_x) * (x - A_x) + (y - A_y) * (y - A_y));
	double dist = sqrt((p.x_ - x) * (p.x_ - x) + (p.y_ - y) * (p.y_ - y));

	return dist <= r;
}

bool delaunay::areNeighbors(const Triangle& lhs, const Triangle& rhs) {
	Point* A = lhs.A_;
	Point* B = lhs.B_;
	Point* C = lhs.C_;

	Point* D = rhs.A_;
	Point* E = rhs.B_;
	Point* F = rhs.C_;

	if (((A == D) || (A == E) || (A == F)) && ((B == D) || (B == E) || (B == F))) {
		return true;
	}
	else if (((A == D) || (A == E) || (A == F)) && ((C == D) || (C == E) || (C == F))) {
		return true;
	}
	else if (((B == D) || (B == E) || (B == F)) && ((C == D) || (C == E) || (C == F))) {
		return true;
	}
	else {
		return false;
	}
}

void delaunay::makePreamble(std::ofstream& fout) {
	fout << R"(\documentclass[a4paper]{article})" << std::endl;
	fout << R"(\usepackage{pgfplots})" << std::endl;
	fout << R"(\usepackage{tikz})" << std::endl;
	fout << R"(\usepackage{tkz-euclide})" << std::endl;
	fout << R"(\usepackage[english, russian]{babel})" << std::endl;
	fout << R"(\usepackage[T2A]{fontenc})" << std::endl;
	fout << R"(\usepackage[utf8]{inputenc})" << std::endl;

	fout << R"(\usepackage{geometry})" << std::endl;
	fout << R"(\geometry{top = 20mm})" << std::endl;
	fout << R"(\geometry{bottom = 25mm})" << std::endl;
	fout << R"(\geometry{left = 25mm})" << std::endl;
	fout << R"(\geometry{right = 25mm})" << std::endl;

	fout << R"(\title{Visualization of the Bowyer-Watson triangulation algorithm.})" << std::endl;
}



std::vector<delaunay::Triangle> delaunay::getTriangulation(std::vector<delaunay::Point>& points, std::ofstream& fout) {
	delaunay::makePreamble(fout);

	fout << R"(\begin{document})" << std::endl;
	fout << R"(\maketitle)" << std::endl;
	points.insert(points.begin(), delaunay::Point(710, 10));
	points.insert(points.begin() + 1, delaunay::Point(10, 750));
	points.insert(points.begin() + 2, delaunay::Point(1400, 790));

	std::vector<delaunay::Triangle> result;
	result.push_back(delaunay::Triangle(&points[0], &points[1], &points[2]));

	cv::Scalar colorLine(0, 255, 0);
	int thicknessLine = 2;

	fout << R"(\section{Creating a super-triangle.})" << std::endl;
	fout << R"(\begin{tikzpicture})" << std::endl;
	fout << R"(\tkzDefPoints{7.10/0.10/A,0.10/7.50/B,14.00/7.9/C})" << std::endl;
	fout << R"(\tkzDrawPolygon[blue](A,B,C))" << std::endl;
	fout << R"(\tkzDrawPoints(A,B,C))" << std::endl;
	for (int i = 0; i < points.size(); i++) {
		fout << R"(\tkzDefPoints{)" << points[i].x_ / 100 << "/" << points[i].y_ / 100 << "/A}" << std::endl;
		fout << R"(\tkzDrawPoints(A))" << std::endl;
	}
	fout << R"(\end{tikzpicture})" << std::endl;


	fout << R"(\section{Adding points.})" << std::endl;


	for (int i = 3; i < points.size(); i++) {
		fout << R"(\subsection{Point number )" << i - 2 << "}" << std::endl;

		delaunay::Point& P = points[i];


		std::vector<int> bad_triangles; // indexes of bad triangles in result
		std::vector<delaunay::Triangle> new_result; // valid triangles during adding the new point
		for (int j = 0; j < result.size(); j++) {
			if (delaunay::InCircle(result[j], P)) {
				bad_triangles.push_back(j);
			}
			else {
				new_result.push_back(result[j]);
			}
		}

		// Finding bad edges
		for (int j = 0; j < bad_triangles.size() - 1; j++) {
			for (int k = j + 1; k < bad_triangles.size(); k++) {
				delaunay::Triangle& first = result[bad_triangles[j]];
				delaunay::Triangle& second = result[bad_triangles[k]];
				if (delaunay::areNeighbors(first, second)) {
					if (first.AB_ == second.AB_) {
						first.AB_.good = false;
						second.AB_.good = false;
					}
					else if (first.AB_ == second.BC_) {
						first.AB_.good = false;
						second.BC_.good = false;
					}
					else if (first.AB_ == second.AC_) {
						first.AB_.good = false;
						second.AC_.good = false;
					}
					else if (first.BC_ == second.AB_) {
						first.BC_.good = false;
						second.AB_.good = false;
					}
					else if (first.BC_ == second.BC_) {
						first.BC_.good = false;
						second.BC_.good = false;
					}
					else if (first.BC_ == second.AC_) {
						first.BC_.good = false;
						second.AC_.good = false;
					}
					if (first.AC_ == second.AB_) {
						first.AC_.good = false;
						second.AB_.good = false;
					}
					else if (first.AC_ == second.BC_) {
						first.AC_.good = false;
						second.BC_.good = false;
					}
					else if (first.AC_ == second.AC_) {
						first.AC_.good = false;
						second.AC_.good = false;
					}
				}
			}
		}

		// Visualisation before deleting bad edges
		fout << R"(\Large{Adding a point and finding bad edges:})" << std::endl;
		fout << std::endl;
		fout << std::endl;
		fout << R"(\begin{tikzpicture})" << std::endl;

		for (int j = 0; j < new_result.size(); j++) {
			fout << R"(\draw[ultra thick, green]()" << new_result[j].A_->x_ / 100 << ", " << new_result[j].A_->y_ / 100 << ")--"
				<< "(" << new_result[j].B_->x_ / 100 << ", " << new_result[j].B_->y_ / 100 << ")--"
				<< "(" << new_result[j].C_->x_ / 100 << ", " << new_result[j].C_->y_ / 100 << ")--"
				<< "(" << new_result[j].A_->x_ / 100 << ", " << new_result[j].A_->y_ / 100 << ");" << std::endl;

			fout << R"(\tkzDefPoints{)" << new_result[j].A_->x_ / 100 << "/" << new_result[j].A_->y_ / 100 << "/A, "
				<< new_result[j].B_->x_ / 100 << "/" << new_result[j].B_->y_ / 100 << "/B, "
				<< new_result[j].C_->x_ / 100 << "/" << new_result[j].C_->y_ / 100 << "/C}" << std::endl;

			fout << R"(\tkzDrawPoints(A, B, C))" << std::endl;
		}
		for (int j = 0; j < bad_triangles.size(); j++) {
			delaunay::Triangle& bad = result[bad_triangles[j]];

			// Визуализая определения плохих ребер, рисование окружностей. Выбрал самый лучший шаг, только для теста № 1.
			if (i - 2 == 6) {
				fout << R"(\tkzDefPoints{)" << bad.A_->x_ / 100 << "/" << bad.A_->y_ / 100 << "/A, "
					<< bad.B_->x_ / 100 << "/" << bad.B_->y_ / 100 << "/B, "
					<< bad.C_->x_ / 100 << "/" << bad.C_->y_ / 100 << "/C}" << std::endl;
				fout << R"(\tkzDefTriangleCenter[circum](A,B,C))" << std::endl;
				fout << R"(\tkzGetPoint{O})" << std::endl;
				fout << R"(\tkzDrawPoints(A, B, C))" << std::endl;
				fout << R"(\tkzDrawCircle[dashed, thin, black](O,A))" << std::endl;
			}

			if (bad.AB_.good == false) {
				fout << R"(\draw[ultra thick, red]()" << bad.AB_.A_->x_ / 100 << ", " << bad.AB_.A_->y_ / 100 << ")--"
					<< "(" << bad.AB_.B_->x_ / 100 << ", " << bad.AB_.B_->y_ / 100 << ");" << std::endl;

				fout << R"(\tkzDefPoints{)" << bad.AB_.A_->x_ / 100 << "/" << bad.AB_.A_->y_ / 100 << "/A, "
					<< bad.AB_.B_->x_ / 100 << "/" << bad.AB_.B_->y_ / 100 << "/B}" << std::endl;
				fout << R"(\tkzDrawPoints(A, B, C))" << std::endl;
			}
			else {
				fout << R"(\draw[ultra thick, green]()" << bad.AB_.A_->x_ / 100 << ", " << bad.AB_.A_->y_ / 100 << ")--"
					<< "(" << bad.AB_.B_->x_ / 100 << ", " << bad.AB_.B_->y_ / 100 << ");" << std::endl;

				fout << R"(\tkzDefPoints{)" << bad.AB_.A_->x_ / 100 << "/" << bad.AB_.A_->y_ / 100 << "/A, "
					<< bad.AB_.B_->x_ / 100 << "/" << bad.AB_.B_->y_ / 100 << "/B}" << std::endl;
				fout << R"(\tkzDrawPoints(A, B, C))" << std::endl;
			}
			if (bad.BC_.good == false) {
				fout << R"(\draw[ultra thick, red]()" << bad.BC_.A_->x_ / 100 << ", " << bad.BC_.A_->y_ / 100 << ")--"
					<< "(" << bad.BC_.B_->x_ / 100 << ", " << bad.BC_.B_->y_ / 100 << ");" << std::endl;

				fout << R"(\tkzDefPoints{)" << bad.BC_.A_->x_ / 100 << "/" << bad.BC_.A_->y_ / 100 << "/A, "
					<< bad.BC_.B_->x_ / 100 << "/" << bad.BC_.B_->y_ / 100 << "/B}" << std::endl;
				fout << R"(\tkzDrawPoints(A, B, C))" << std::endl;
			}
			else {
				fout << R"(\draw[ultra thick, green]()" << bad.BC_.A_->x_ / 100 << ", " << bad.BC_.A_->y_ / 100 << ")--"
					<< "(" << bad.BC_.B_->x_ / 100 << ", " << bad.BC_.B_->y_ / 100 << ");" << std::endl;

				fout << R"(\tkzDefPoints{)" << bad.BC_.A_->x_ / 100 << "/" << bad.BC_.A_->y_ / 100 << "/A, "
					<< bad.BC_.B_->x_ / 100 << "/" << bad.BC_.B_->y_ / 100 << "/B}" << std::endl;
				fout << R"(\tkzDrawPoints(A, B, C))" << std::endl;
			}
			if (bad.AC_.good == false) {
				fout << R"(\draw[ultra thick, red]()" << bad.AC_.A_->x_ / 100 << ", " << bad.AC_.A_->y_ / 100 << ")--"
					<< "(" << bad.AC_.B_->x_ / 100 << ", " << bad.AC_.B_->y_ / 100 << ");" << std::endl;

				fout << R"(\tkzDefPoints{)" << bad.AC_.A_->x_ / 100 << "/" << bad.AC_.A_->y_ / 100 << "/A, "
					<< bad.AC_.B_->x_ / 100 << "/" << bad.AC_.B_->y_ / 100 << "/B}" << std::endl;
				fout << R"(\tkzDrawPoints(A, B, C))" << std::endl;
			}
			else {
				fout << R"(\draw[ultra thick, green]()" << bad.AC_.A_->x_ / 100 << ", " << bad.AC_.A_->y_ / 100 << ")--"
					<< "(" << bad.AC_.B_->x_ / 100 << ", " << bad.AC_.B_->y_ / 100 << ");" << std::endl;

				fout << R"(\tkzDefPoints{)" << bad.AC_.A_->x_ / 100 << "/" << bad.AC_.A_->y_ / 100 << "/A, "
					<< bad.AC_.B_->x_ / 100 << "/" << bad.AC_.B_->y_ / 100 << "/B}" << std::endl;
				fout << R"(\tkzDrawPoints(A, B, C))" << std::endl;
			}
		}

		fout << R"(\draw[fill = yellow]()" << P.x_ / 100 << ", " << P.y_ / 100 << ") " << "circle[radius=3pt];" << std::endl;
		fout << R"(\draw[ultra thick, blue](7.10, 0.10)--(0.10, 7.50)--(14.00, 7.90)--(7.10, 0.10);)" << std::endl;
		fout << R"(\end{tikzpicture})" << std::endl;
		fout << std::endl;
		fout << std::endl;


		// Creating new triangles with good bad_triangles's edges
		for (int j = 0; j < bad_triangles.size(); j++) {
			delaunay::Triangle& bad = result[bad_triangles[j]];
			if (bad.AB_.good == true) {
				new_result.push_back(delaunay::Triangle(&P, bad.AB_.A_, bad.AB_.B_));
			}
			if (bad.BC_.good == true) {
				new_result.push_back(delaunay::Triangle(&P, bad.BC_.A_, bad.BC_.B_));
			}
			if (bad.AC_.good == true) {
				new_result.push_back(delaunay::Triangle(&P, bad.AC_.A_, bad.AC_.B_));
			}
		}
		result = new_result;

		// Visualisation of triangulation after adding a new point
		fout << "Creating new triangles:" << std::endl;
		fout << std::endl;
		fout << std::endl;
		fout << R"(\begin{tikzpicture})" << std::endl;
		for (int j = 0; j < result.size(); j++) {
			fout << R"(\tkzDefPoints{)" << result[j].A_->x_ / 100 << "/" << result[j].A_->y_ / 100 << "/A,"
				<< result[j].B_->x_ / 100 << "/" << result[j].B_->y_ / 100 << "/B,"
				<< result[j].C_->x_ / 100 << "/" << result[j].C_->y_ / 100 << "/C}" << std::endl;
			fout << R"(\tkzDrawPolygon[green](A,B,C))" << std::endl;
			fout << R"(\tkzDrawPoints(A,B,C))" << std::endl;
		}
		fout << R"(\draw[ultra thick, blue](7.10, 0.10)--(0.10, 7.50)--(14.00, 7.90)--(7.10, 0.10);)" << std::endl;
		fout << R"(\end{tikzpicture})" << std::endl;
	}

	// Deleting super triangle
	int g = 0;
	while (g < result.size()) {
		if (result[g].A_ == &points[0] || result[g].A_ == &points[1] || result[g].A_ == &points[2]) {
			result.erase(result.begin() + g);
		}
		else if (result[g].B_ == &points[0] || result[g].B_ == &points[1] || result[g].B_ == &points[2]) {
			result.erase(result.begin() + g);
		}
		else if (result[g].C_ == &points[0] || result[g].C_ == &points[1] || result[g].C_ == &points[2]) {
			result.erase(result.begin() + g);
		}
		else {
			g = g + 1;
		}
	}

	// Visualisation of final triangulation 
	fout << R"(\section{Deleting super triangle.})" << std::endl;
	fout << R"(\begin{tikzpicture})" << std::endl;
	cv::Mat triangulation(800, 1450, CV_8UC3, cv::Scalar(0, 0, 0));
	for (int i = 0; i < result.size(); i++) {
		cv::Point p1(result[i].A_->x_, result[i].A_->y_), p2(result[i].B_->x_, result[i].B_->y_), p3(result[i].C_->x_, result[i].C_->y_);
		cv::line(triangulation, p1, p2, colorLine, thicknessLine);
		cv::line(triangulation, p2, p3, colorLine, thicknessLine);
		cv::line(triangulation, p3, p1, colorLine, thicknessLine);

		fout << R"(\tkzDefPoints{)" << result[i].A_->x_ / 100 << "/" << result[i].A_->y_ / 100 << "/A,"
			<< result[i].B_->x_ / 100 << "/" << result[i].B_->y_ / 100 << "/B,"
			<< result[i].C_->x_ / 100 << "/" << result[i].C_->y_ / 100 << "/C}" << std::endl;
		fout << R"(\tkzDrawPolygon[green](A,B,C))" << std::endl;
		fout << R"(\tkzDrawPoints(A,B,C))" << std::endl;
	}
	fout << R"(\draw[dashed, thin, black](7.10, 0.10)--(0.10, 7.50)--(14.00, 7.90)--(7.10, 0.10);)" << std::endl;
	fout << R"(\end{tikzpicture})" << std::endl;


	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display window", triangulation);
	cv::waitKey(0);


	fout << R"(\end{document})";
	return result;
}
