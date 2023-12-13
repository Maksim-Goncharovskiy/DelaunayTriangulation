#pragma once
#ifndef DELAUNAY_TRIANGULATION_LIB_2373
#define DELAUNAY_TRIANGULATION_LIB_2373


#include <iostream>
#include <fstream>
#include <vector>
/**
namespace delaunay
@brief ������������ ��� delaunay, c������� ��� ����������� ��������� � ������� ��� ���������� ����������� ������.
*/
namespace delaunay {
	struct Point {
		double x_ = 0;
		double y_ = 0;

		Point() = default;
		~Point() = default;

		Point(const Point& other) = default;
		Point& operator=(const Point& other) = default;

		Point(Point&& other) = default;
		Point& operator=(Point&& other) = default;

		/**
		@brief ����������� � �����������

		@param x - ���������� x �����
		@param y - ���������� y �����
		*/
		Point(double x, double y);
	};


	struct Edge {
		Point* A_ = nullptr; //!< ��������� �� ������ ������� �����.
		Point* B_ = nullptr; //!< ��������� �� ������ ������� �����.
		/**
		@brief ���� � ����������� �������� �� ������ ����� ������.

		True - ����� �������, false - ����� ������(����� �������).
		*/
		bool good = true;

		Edge() = default;
		~Edge() = default;

		Edge(const Edge& other) = default;
		Edge& operator=(const Edge& other) = default;

		Edge(Edge&& other) = default;
		Edge& operator=(Edge&& other) = default;

		/**
		@brief ����������� � �����������

		@param A ������ ����� �������
		@param B ������ ����� �������
		*/
		Edge(Point* A, Point* B);
		/**
		@brief �������� ����������� ���� ����

		@param other �����, � ������� ���������� ���������.
		@return true, ���� ����� ���������, � ��������� ������ - false.
		*/
		bool operator==(const Edge& other);
	};


	struct Triangle {
		Point* A_ = nullptr; //!< ��������� �� ������ ������� ������������.
		Point* B_ = nullptr; //!< ��������� �� ������ ������� ������������.
		Point* C_ = nullptr; //!< ��������� �� ������ ������� ������������.

		Edge AB_; //!< ������ �����, �� ����� A_ � B_
		Edge BC_; //!< ������ �����, �� ����� B_ � C_
		Edge AC_; //!< ������ �����, �� ����� A_ � C_

		Triangle() = default;
		~Triangle() = default;

		Triangle(const Triangle& other) = default;
		Triangle& operator=(const Triangle& other) = default;

		Triangle(Triangle&& other) = default;
		Triangle& operator=(Triangle&& other) = default;

		/**
		@brief ����������� � �����������

		@param A ��������� �� ������ ������� ������������.
		@param B ��������� �� ������ ������� ������������.
		@param � ��������� �� ������ ������� ������������.
		*/
		Triangle(Point* A, Point* B, Point* C);
	};

	/**
	@brief ��������� ��������� �� ����� ������ ����������, ��������� ������ ������������.

	@param tr �����������, ��� �������� ����������� �������.
	@param p �����, ��������� ������� ������������ ��������� ���������� ������������ �����������.
	@return true, ���� ����� ����� ������ ��� �� ����������, ��������� ������ ������������. false - ���� ����� ����� ��� ����������, ��������� ������ ������������.

	*/
	bool InCircle(const Triangle& tr, const Point& p);

	/**
	@brief ��������� �������� �� ��� ������������ ��������.

	@param lhs Cc���� �� ������ �����������.
	@param rhs Cc���� �� ������ �����������.
	@return true, ���� � ������������� ���� ����� �����, false - ���� ����� ���� ���.
	*/
	bool areNeighbors(const Triangle& lhs, const Triangle& rhs);

	void makePreamble(std::ofstream& fout);

	/**
	@brief ���������� ��������� ������-�������.

	@param points ������� ����� �����
	@param fout ����� ��� ������ � tex-���� ���������� ��� ������������ ����� ���������
	@return ������ �������������, �������� � ������������
	*/
	std::vector<Triangle> getTriangulation(std::vector<Point>& points, std::ofstream& fout);
}
#endif 
