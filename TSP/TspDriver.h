#pragma once

#include <Siv3D.hpp>
#include "DataSet.h"

class TspDriver
{
public:
	TspDriver();
	~TspDriver();

	void SetTspDataSet(const Array<Point>& points);
	void Draw(const Font& font);
	void ExecuteOneStep();

private:
	Array<Point> points_;
	Point max_point_;
	Array<int> points_order_;
	Array<int> updated_points_;
	bool stil_exec_;
};

