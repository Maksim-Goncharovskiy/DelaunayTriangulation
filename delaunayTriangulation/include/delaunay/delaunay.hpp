#pragma once
#ifndef DELAUNAY_TRIANGULATION_LIB_2373
#define DELAUNAY_TRIANGULATION_LIB_2373


#include <iostream>
#include <fstream>
#include <vector>
/**
namespace delaunay
@brief Пространство имён delaunay, cодержит все необходимые структуры и функции для построения триангуляии Делоне.
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
		@brief Конструктор с параметрами

		@param x - координата x точки
		@param y - координата y точки
		*/
		Point(double x, double y);
	};


	struct Edge {
		Point* A_ = nullptr; //!< Указатель на первую вершину ребра.
		Point* B_ = nullptr; //!< Указатель на вторую вершину ребра.
		/**
		@brief Поле с информацией является ли данной ребро плохим.

		True - ребро хорошее, false - ребро плохое(будет удалено).
		*/
		bool good = true;

		Edge() = default;
		~Edge() = default;

		Edge(const Edge& other) = default;
		Edge& operator=(const Edge& other) = default;

		Edge(Edge&& other) = default;
		Edge& operator=(Edge&& other) = default;

		/**
		@brief Конструктор с параметрами

		@param A Первая точка отрезка
		@param B Вторая точка отрезка
		*/
		Edge(Point* A, Point* B);
		/**
		@brief Оператор сравнивания двух рёбер

		@param other Ребро, с которым происходит сравнение.
		@return true, если ребра совпадают, в противном случае - false.
		*/
		bool operator==(const Edge& other);
	};


	struct Triangle {
		Point* A_ = nullptr; //!< Указатель на первую вершину треугольника.
		Point* B_ = nullptr; //!< Указатель на вторую вершину треугольника.
		Point* C_ = nullptr; //!< Указатель на третью вершину треугольника.

		Edge AB_; //!< Первое ребро, из точек A_ и B_
		Edge BC_; //!< Второе ребро, из точек B_ и C_
		Edge AC_; //!< Третье ребро, из точек A_ и C_

		Triangle() = default;
		~Triangle() = default;

		Triangle(const Triangle& other) = default;
		Triangle& operator=(const Triangle& other) = default;

		Triangle(Triangle&& other) = default;
		Triangle& operator=(Triangle&& other) = default;

		/**
		@brief Конструктор с параметрами

		@param A Указатель на первую вершину треугольника.
		@param B Указатель на вторую вершину треугольника.
		@param С Указатель на третью вершину треугольника.
		*/
		Triangle(Point* A, Point* B, Point* C);
	};

	/**
	@brief Проверяет находится ли точка внутри окружности, описанной вокруг треугольника.

	@param tr Треугольник, для которого проверяется условие.
	@param p Точка, положение которой относительно описанной окружности треугольника проверяется.
	@return true, если точка лежит внутри или на окружности, описанной вокруг треугольника. false - если точка лежит вне окружности, описанной вокруг треугольника.

	*/
	bool InCircle(const Triangle& tr, const Point& p);

	/**
	@brief Проверяет являются ли два треугольника смежными.

	@param lhs Ccылка на первый треугольник.
	@param rhs Ccылка на второй треугольник.
	@return true, если у треугольников есть общее ребро, false - если общих рёбер нет.
	*/
	bool areNeighbors(const Triangle& lhs, const Triangle& rhs);

	void makePreamble(std::ofstream& fout);

	/**
	@brief Реализация алгоритма Боуэра-Ватсона.

	@param points Входной набор точек
	@param fout Поток для вывода в tex-файл информации для визуализации шагов алгоритма
	@return Вектор треугольников, входящих в триангуляцию
	*/
	std::vector<Triangle> getTriangulation(std::vector<Point>& points, std::ofstream& fout);
}
#endif 
