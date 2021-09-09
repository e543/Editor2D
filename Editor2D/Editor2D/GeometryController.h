#pragma once
#include "Geometry.h"
#include <list>
#include <memory>


class GeometryController
{
	friend class Application;
public:
	struct RenderContext
	{
		std::list<std::shared_ptr<Point>> points;
		std::list<std::shared_ptr<Line>> lines;
		std::list<std::shared_ptr<Spline>> splines;
	};
private:
	RenderContext context;
	bool lineChain = true;
	bool lineIsDrawing = false;
	bool pointIsDragging = false;
	std::pair<int, int> mouse_pos;
	std::shared_ptr<Point> last_selected;

	// Mouse stuff
	void MousePosChanged(std::pair<int, int> pos);

	// Point stuff
	void checkSelectedPoint();
	std::shared_ptr<Point> getSelectedPoint();
	std::shared_ptr<Point> addPoint(int x, int y);
	void addPoint();
	void addPoint(std::pair<int, int> mouse_pos);
	void addPoint(std::shared_ptr<Point> new_point);
	void addLine(std::shared_ptr<Point> begin_point, std::shared_ptr<Point> end_point);
	void addLine(int x1, int y1, int x2, int y2);

	// Making Line
	void StartMakingLine();
	void ResizeLine();
	void EndMakingLine();
	bool OnDrawingLine();

	// Dragging point
	void StartDraggingPoint();
	bool OnDraggingPoint();
	void StopDraggingPoint();
	void dragPoint();

	// Making spline
	void MakeSpline();
	void addSpline(
		std::shared_ptr<Point> P1,
		std::shared_ptr<Point> P2,
		std::shared_ptr<Point> P3,
		std::shared_ptr<Point> P4);
};

