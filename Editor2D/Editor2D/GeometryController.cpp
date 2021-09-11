#include "GeometryController.h"
#include <vector>




std::shared_ptr<Point> GeometryController::getSelectedSupPoint()
{
	std::shared_ptr<Point> selected_point = nullptr;
	for (auto iter = context.sup_points.begin(); iter != context.sup_points.end(); ++iter)
	{
		if (!(*iter)->OnLock() && (*iter)->Selected(mouse_pos))
			return static_cast<std::shared_ptr<Point>>(*iter);
	}
	return nullptr;
}

std::shared_ptr<Point> GeometryController::getSelectedMainPoint()
{
	std::shared_ptr<Point> selected_point = nullptr;
	for (auto iter = context.main_points.begin(); iter != context.main_points.end(); ++iter)
	{
		if (!(*iter)->OnLock() && (*iter)->Selected(mouse_pos))
			return static_cast<std::shared_ptr<Point>>(*iter);
	}
	return nullptr;
}


void GeometryController::checkSelectedSupPoint()
{
	std::shared_ptr<Point> selected_point = nullptr;
	for (auto iter = context.sup_points.begin(); iter != context.sup_points.end(); ++iter)
	{
		if (!(*iter)->OnLock())
			(*iter)->Selected(mouse_pos);
	}
}

void GeometryController::checkSelectedMainPoint()
{
	std::shared_ptr<Point> selected_point = nullptr;
	for (auto iter = context.main_points.begin(); iter != context.main_points.end(); ++iter)
	{
		if (!(*iter)->OnLock())
			(*iter)->Selected(mouse_pos);
	}
}



void GeometryController::MousePosChanged(std::pair<int, int> pos)
{
	checkSelectedSupPoint();
	checkSelectedMainPoint();
	mouse_pos = pos;
}


std::shared_ptr<Point> GeometryController::addPoint(int x, int y)
{
	auto new_point = std::make_shared<Point>(x, y);
	context.main_points.emplace_back(new_point);
	return new_point;
}

std::shared_ptr<Point> GeometryController::addPoint()
{
	auto new_point = std::make_shared<Point>(mouse_pos.first, mouse_pos.second);
	context.main_points.emplace_back(new_point);
	return new_point;
}
void GeometryController::addPoint(std::pair<int, int> pos)
{
	auto new_point = std::make_shared<Point>(pos.first, pos.second);
	context.main_points.emplace_back(new_point);
}
void GeometryController::addPoint(std::shared_ptr<Point> new_point)
{
	context.main_points.emplace_back(new_point);
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

std::shared_ptr<Point> GeometryController::addMainPoint()
{
	auto new_main_point = std::make_shared<Point>(mouse_pos.first, mouse_pos.second);
	context.main_points.emplace_back(new_main_point);
	return new_main_point;
}

void GeometryController::addMainPoint(std::shared_ptr<Point> new_main_point)
{
	context.main_points.emplace_back(new_main_point);
}

std::shared_ptr<Point> GeometryController::addSupPoint()
{
	auto new_sup_point = std::make_shared<Point>(mouse_pos.first, mouse_pos.second);
	new_sup_point->SetColor(D2D1::ColorF::Brown);
	new_sup_point->Lock();
	context.sup_points.emplace_back(new_sup_point);
	return new_sup_point;
}
std::shared_ptr<Point> GeometryController::addSupPoint(std::pair<int,int> pos)
{
	auto new_sup_point = std::make_shared<Point>(pos.first, pos.second);
	new_sup_point->SetColor(D2D1::ColorF::Brown);
	new_sup_point->Lock();
	context.sup_points.emplace_back(new_sup_point);
	return new_sup_point;
}

void GeometryController::addSupPoint(std::shared_ptr<Point> new_sup_point)
{
	new_sup_point->SetColor(D2D1::ColorF::Brown);
	new_sup_point->Lock();
	context.sup_points.emplace_back(new_sup_point);
}

void GeometryController::addNode(Node::Type type)
{
	std::shared_ptr<Node> new_node;

	switch (type)
	{
	case Node::Type::First:
	{
		new_node = std::make_shared<Node>(type, addPoint());
		spline.emplace_back(new_node);
		break;
	}
	case Node::Type::Last:
	{
		auto last_node = spline.back();

		// last duty
		if (!spline.empty())
		{
			last_node->bindNextSup(addSupPoint(last_node->getMain()->GetPos()));
			if (spline.size() > 1)
			{
				last_node->setType(Node::Type::Internal);
			}
		}

		new_node = std::make_shared<Node>
			(
				type,
				addPoint(),
				addSupPoint(),
				last_node->getMain()
			);
		last_node->bindSecond(new_node->getMain());
		spline.emplace_back(new_node);
		MakeBezie();
		break;
	}
	}

}



void GeometryController::StartMakingLine()
{
	std::shared_ptr<Point> begin_point;
	if (context.main_points.empty())
	{
		begin_point = std::make_shared<Point>(mouse_pos.first, mouse_pos.second);
		addPoint(begin_point);
	}
	else
		begin_point = context.main_points.back();

	auto end_point = std::make_shared<Point>(mouse_pos.first, mouse_pos.second);

	addPoint(end_point);
	addLine(begin_point, end_point);

	lineIsDrawing = true;
}

void GeometryController::ResizeLine()
{

	auto point = context.main_points.back();
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
	auto selected = getSelectedSupPoint();
	last_selected = selected ? selected : getSelectedMainPoint();
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
		calcSpline();
	}
}

void GeometryController::calcSpline()
{
	for (auto riter = spline.rbegin(); riter != spline.rend(); ++riter)
	{
		(*riter)->calcSupPoints();
	}
}

void GeometryController::MakeBezie()
{
	auto* nodes = new std::shared_ptr<Node>[2];

	size_t count = 0;
	for (auto riter = spline.rbegin(); count < 2; ++count, ++riter)
	{
		nodes[count] = static_cast<std::shared_ptr<Node>>(*riter);
	}
	nodes[1]->calcSupPoints();
	if (nodes[1]->getType() == Node::Type::Internal)
	{
		context.lines.emplace_back(nodes[1]->getSupLine());
	}
	addSpline(nodes[0]->getMain(), nodes[0]->getSup1(), nodes[1]->getSup2(), nodes[1]->getMain());
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
