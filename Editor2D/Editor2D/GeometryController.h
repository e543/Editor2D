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
		std::list<std::shared_ptr<Point>> sup_points;
		std::list<std::shared_ptr<Point>> main_points;
		std::list<std::shared_ptr<Line>> lines;
		std::list<std::shared_ptr<Bezier>> bezies;
	};
private:
	std::list<std::shared_ptr<Node>> spline;
	RenderContext context;
	bool lineChain = true;
	bool lineIsDrawing = false;
	bool pointIsDragging = false;
	std::pair<int, int> mouse_pos;
	std::shared_ptr<Point> last_selected;
	bool sup_selection_locked = true;

	// Mouse stuff
	void MousePosChanged(std::pair<int, int> pos);

	// Point stuff
	void checkSelectedSupPoint();
	void checkSelectedMainPoint();
	std::shared_ptr<Point> getSelectedSupPoint();
	std::shared_ptr<Point> getSelectedMainPoint();
	std::shared_ptr<Point> addPoint(int x, int y);
	std::shared_ptr<Point> addPoint();
	void addPoint(std::pair<int, int> mouse_pos);
	void addPoint(std::shared_ptr<Point> new_point);
	void addLine(std::shared_ptr<Point> begin_point, std::shared_ptr<Point> end_point);
	void addLine(int x1, int y1, int x2, int y2);
	std::shared_ptr<Point> addMainPoint();
	void addMainPoint(std::shared_ptr<Point> new_main_point);
	void addSupPoint(std::shared_ptr<Point> new_sup_point);
	std::shared_ptr<Point> addSupPoint();
	std::shared_ptr<Point> addSupPoint(std::pair<int, int> pos);

	// Node Stuff
	void addNode(Node::Type type);
	void deleteNode();

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
	void calcSpline();
	void MakeBezie();
	void addSpline(
		std::shared_ptr<Point> P1,
		std::shared_ptr<Point> P2,
		std::shared_ptr<Point> P3,
		std::shared_ptr<Point> P4);
};

