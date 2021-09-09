#include "GeometryController.h"
#include <vector>




std::shared_ptr<Point> GeometryController::getSelectedPoint()
{
	std::shared_ptr<Point> selected_point = nullptr;
	for (auto iter = context.points.begin(); iter != context.points.end(); ++iter)
	{
		if ((*iter)->Selected(mouse_pos))
			return static_cast<std::shared_ptr<Point>>(*iter);
	}
	return nullptr;
}

void GeometryController::checkSelectedPoint()
{
	std::shared_ptr<Point> selected_point = nullptr;
	for (auto iter = context.points.begin(); iter != context.points.end(); ++iter)
	{
		(*iter)->Selected(mouse_pos);
	}
}



void GeometryController::MousePosChanged(std::pair<int, int> pos)
{
	checkSelectedPoint();
	mouse_pos = pos;
}


std::shared_ptr<Point> GeometryController::addPoint(int x, int y)
{
	auto new_point = std::make_shared<Point>(x, y);
	context.points.emplace_back(new_point);
	return new_point;
}
void GeometryController::addPoint()
{
	auto new_point = std::make_shared<Point>(mouse_pos.first, mouse_pos.second);
	context.points.emplace_back(new_point);
}
void GeometryController::addPoint(std::pair<int, int> pos)
{
	auto new_point = std::make_shared<Point>(pos.first, pos.second);
	context.points.emplace_back(new_point);
}
void GeometryController::addPoint(std::shared_ptr<Point> new_point)
{
	context.points.emplace_back(new_point);
}


void GeometryController::addLine(std::shared_ptr<Point> begin_point, std::shared_ptr<Point> end_point)
{
	auto new_line = std::make_shared<Line>(begin_point, end_point);
	context.lines.emplace_back(new_line);
}

void GeometryController::addLine(int x1, int y1, int x2, int y2)
{
	auto begin_point = std::make_shared<Point>(x1, y1);
	auto end_point = std::make_shared<Point>(x2, y2);
	auto new_line = std::make_shared<Line>(begin_point, end_point);
	context.lines.emplace_back(new_line);
}

void GeometryController::StartMakingLine()
{
	std::shared_ptr<Point> begin_point;
	if (context.points.empty())
	{
		begin_point = std::make_shared<Point>(mouse_pos.first, mouse_pos.second);
		addPoint(begin_point);
	}
	else
		begin_point = context.points.back();

	auto end_point = std::make_shared<Point>(mouse_pos.first, mouse_pos.second);

	addPoint(end_point);
	addLine(begin_point, end_point);

	lineIsDrawing = true;
}

void GeometryController::ResizeLine()
{

	auto point = context.points.back();
	point->SetPos(mouse_pos);
	auto line = context.lines.back();
	line->SetEnd(point);
}

void GeometryController::EndMakingLine()
{
	lineIsDrawing = false;
}

void GeometryController::StopDraggingPoint()
{
	pointIsDragging = false;
}

void GeometryController::StartDraggingPoint()
{
	last_selected = getSelectedPoint();
	if (last_selected)
	{
		pointIsDragging = true;
	}
}

bool GeometryController::OnDrawingLine()
{
	return lineIsDrawing;
}

bool GeometryController::OnDraggingPoint()
{
	return pointIsDragging;
}

void GeometryController::dragPoint()
{
	if (last_selected)
	{
		last_selected->SetPos(mouse_pos);
	}
}

void GeometryController::MakeSpline()
{
	std::shared_ptr<Point>* ps = new std::shared_ptr<Point>[4];

	size_t count = 0;
	for (auto riter = context.points.rbegin(); count < 4; ++count, ++riter)
	{
		ps[count] = static_cast<std::shared_ptr<Point>>(*riter);
	}
	addSpline(ps[0], ps[1], ps[2], ps[3]);
}

void GeometryController::addSpline(
	std::shared_ptr<Point> P1,
	std::shared_ptr<Point> P2,
	std::shared_ptr<Point> P3,
	std::shared_ptr<Point> P4)
{
	auto new_spline = std::make_shared<Bezie>(P1, P2, P3, P4);
	context.bezies.emplace_back(new_spline);
}
