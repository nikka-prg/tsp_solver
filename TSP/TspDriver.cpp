#include "TspDriver.h"

namespace {
	const Circle kCircle(5.0);
	const Vec2 kDrawMergin(80.0, 60.0);
	const Vec2 kDrawArea(640.0, 480.0);
}


TspDriver::TspDriver()
{
	SetTspDataSet(DataSet::kUnitedStates);
	stil_exec_ = true;
}


TspDriver::~TspDriver()
{
}

void TspDriver::SetTspDataSet(const Array<Point>& points)
{
	points_.clear();
	points_order_.clear();

	points_ = points;
	std::vector<int> point_x;
	std::vector<int> point_y;
	int index = 0;
	for (auto p : points) {
		point_x.push_back(p.x);
		point_y.push_back(p.y);
		points_order_.push_back(index++);
	}
	// max_num (to draw)
	max_point_.x = *std::max_element(point_x.begin(), point_x.end());
	max_point_.y = *std::max_element(point_y.begin(), point_y.end());
}

void TspDriver::Draw(const Font& font)
{
	// points
	Array<Vec2> drawing_points;
	for (auto p : points_) {
		const Vec2 vec = (kDrawArea * p / max_point_) + kDrawMergin;
		drawing_points.push_back(vec);
		kCircle.movedBy(vec).draw(Palette::Orange);
	}

	// line
	double distance = 0.0;
	for (auto i : step(points_.size())) {
		const int temp = (i + 1) % points_.size();
		const int base_point = points_order_[i];
		const int next_point = points_order_[temp];
		const Line line(drawing_points[base_point], drawing_points[next_point]);
		Color color = Palette::Black;
		if (updated_points_.includes(base_point) && updated_points_.includes(next_point)) {
			color = Palette::Red;
		}
		line.draw(1.5, color);
		distance += points_[base_point].distanceFrom(points_[next_point]);
	}

	// distance
	font(distance).draw(20, 500, ColorF(0.6));
}

void TspDriver::ExecuteOneStep()
{
	updated_points_.clear();

	for (auto i : step(points_.size() - 2)) {
		for (auto j = i + 2; j < points_.size() - 1; ++j) {
			const int j_next = (j + 1) % points_.size();
			double before = points_[points_order_[i]].distanceFrom(points_[points_order_[i + 1]])
				+ points_[points_order_[j]].distanceFrom(points_[points_order_[j_next]]);
			double after = points_[points_order_[i]].distanceFrom(points_[points_order_[j]])
				+ points_[points_order_[i + 1]].distanceFrom(points_[points_order_[j_next]]);
			if (after < before) {
				// new order
				Array<int> new_order;
				for (int k = 0; k <= i; ++k) {
					new_order.push_back(points_order_[k]);
				}
				for (int k = i + 1; k <= j; ++k) {
					const int t = j - k + i + 1;
					new_order.push_back(points_order_[t]);
				}
				for (int k = j + 1; k < points_order_.size(); ++k) {
					new_order.push_back(points_order_[k]);
				}
				points_order_ = new_order;

				// update points
				updated_points_.push_back(points_order_[i]);
				updated_points_.push_back(points_order_[i + 1]);
				updated_points_.push_back(points_order_[j]);
				updated_points_.push_back(points_order_[j_next]);
				return;
			}
		}
	}
	stil_exec_ = false;
	return;
}
