# Проект - "Построение триангуляции Делоне"
## Цели работы:
1. Создание библиотеки delaunayTriangulation, которая:
   - Строит триангуляцию, удовлетворяющую условию Делоне, по заданному множеству точек.
   - Визуализирует построенную триангуляцию при помощи OpenCV, а также генерирует tex-файл, который содержит в себе необходимую информацию для визуализации основных шагов алгоритма.
2. Создание тестового консольного приложения для тестирования и демонстрации работы библиотеки.

## Реализуемый алгоритм: алгоритм Боуэра-Ватсона https://ru.wikipedia.org/wiki/Алгоритм_Боуэра_—_Ватсона

---
## Описание библиотеки delaunayTriangulation.
1. Для чего: Библиотека delaunayTriangulation содержит набор функций и структур, необходимых для реализации алгоритма Боуэра-Ватсона построения триангуляции Делоне. Соответственно она может быть использована в сторонних библиотеках для решения задач, требующих построения триангуляции.
2. Использование функционала: все функции и структуры библиотеки находятся внутри пространства имён delaunay.
Алгоритм Боуэра-Ватсона реализован в функции getTriangulation():
```c++
/**
	@brief Реализация алгоритма Боуэра-Ватсона.

	@param points Входной набор точек
	@param fout Поток для вывода в tex-файл информации для визуализации шагов алгоритма
	@return Вектор треугольников, входящих в триангуляцию
	*/
std::vector<Triangle> getTriangulation(std::vector<Point>& points, std::ofstream& fout);
```
## Описание тестового приложения 
Представляет собой консольное приложение, принимающее параметры командной строки: 
- путь до тестового файла
- путь к tex-файлу, куда будет записана информация для визуализации шагов алгоритма в LaTeX \
Если не передать второй параметр, запись будет производиться в файл по умолчанию, который находится в папке results. 
В папке проекта в разделе tests можно найти примеры тестовых файлов, на которых есть возможность протестировать работу библиотеки. Формат тестовых файлов выглядит следующим образом: в первой строке одно натуральное число N - количество точек входного набора. Далее идут N строк,
в каждой из которых пара положительных чисел, записанных через пробел
- координаты точки. __При несоответствии тестового файла описанному выше
формату корректная работа тестового приложения не гарантирована__!
