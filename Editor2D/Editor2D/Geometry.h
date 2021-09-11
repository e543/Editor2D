#pragma once
#include <d2d1.h>
#include <memory>

struct IDrawable
{
	virtual void Draw(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush) = 0;
};

struct ISelectable
{
	bool isSelected;
	virtual bool Selected(std::pair<int, int> mouse_pos) = 0;
	bool isLocked;
	void Lock()		{ isLocked = true; }
	bool OnLock()	{ return isLocked; }
	void Unlock()	{ isLocked = false; }
};

struct IDeletable
{
	bool onDelete = false;
	void Delete() { onDelete = true; }
	bool OnDelete() { return onDelete; }
};

class Point : public IDrawable, public ISelectable, public IDeletable
{
	D2D1_ELLIPSE ellipse;
	D2D1::ColorF color = D2D1::ColorF::Black;
	static constexpr float rad = 5.0f;
	static constexpr float detection_gap = 10.0f;
	static constexpr float detection_rad = rad + detection_gap;
public:
	Point(float x, float y)
	{
		ellipse.point.x = x;
		ellipse.point.y = y;
		ellipse.radiusX = ellipse.radiusY = rad;
		isLocked = false;
		isSelected = false;
	}

	void SetColor(D2D1::ColorF color) { this->color = color; }

	void Draw(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush) override
	{
		pBrush->SetColor(color);
		pRT->FillEllipse(ellipse, pBrush);
		if (isSelected)
		{
			pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			pRT->DrawEllipse(ellipse, pBrush, 2.0f);
		}
	}

	bool Selected(std::pair<int, int> mouse_pos) override
	{
		const float x1 = mouse_pos.first - ellipse.point.x;
		const float y1 = mouse_pos.second - ellipse.point.y;
		const float sqr_d = (x1 * x1) + (y1 * y1);
		isSelected = (sqrt(sqr_d) <= detection_rad);
		return isSelected;
	}


	int getX() const
	{
		return ellipse.point.x;
	}
	int getY() const
	{
		return ellipse.point.y;
	}
	void setX(int _x)
	{
		ellipse.point.x = _x;
	}
	void setY(int _y)
	{
		ellipse.point.y = _y;
	}
	void SetPos(std::pair<int, int> pos)
	{
		ellipse.point.x = pos.first;
		ellipse.point.y = pos.second;
	}
	std::pair<int, int> GetPos()
	{
		return std::make_pair(ellipse.point.x, ellipse.point.y);
	}
};

struct Line : public IDrawable
{
	std::shared_ptr<Point> begin;
	std::shared_ptr<Point> end;
	D2D1::ColorF color = D2D1::ColorF::Orange;
	Line(std::shared_ptr<Point> begin, std::shared_ptr<Point> end) : begin(begin), end(end) {};

	void Draw(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush) override
	{
		pBrush->SetColor(color);

		// if selected
		//pBrush->SetColor(isSelect ? D2D1::ColorF(D2D1::ColorF::White) : D2D1::ColorF(D2D1::ColorF::Black));

		pRT->DrawLine(D2D1::Point2F(begin->getX(), begin->getY()), D2D1::Point2F(end->getX(), end->getY()), pBrush, 2.0f);
	}

	void SetBegin(std::shared_ptr<Point> new_begin)
	{
		begin = new_begin;
	}
	void SetEnd(std::shared_ptr<Point> new_end)
	{
		end = new_end;
	}
	void SetBegin(int x, int y)
	{
		begin->setX(x);
		begin->setY(y);
	}
	void SetEnd(int x, int y)
	{
		end->setX(x);
		end->setY(y);
	}
	void SetBegin(std::pair<int, int> pos)
	{
		begin->setX(pos.first);
		begin->setY(pos.second);
	}
	void SetEnd(std::pair<int, int> pos)
	{
		end->setX(pos.first);
		end->setY(pos.second);
	}

	static std::pair<float, float> getDelta(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
	{
		float x1 = p1->getX();
		float y1 = p1->getY();
		float x2 = p2->getX();
		float y2 = p2->getY();
		float dx = x2 - x1;
		float dy = y2 - y1;

		return std::make_pair(dx, dy);
	}

	static std::pair<float, float> getDelta(std::pair<float, float> pos_begin, std::pair<float, float> pos_end)
	{
		float x1 = pos_begin.first;
		float y1 = pos_begin.second;
		float x2 = pos_end.first;
		float y2 = pos_end.second;
		float dx = x2 - x1;
		float dy = y2 - y1;

		return std::make_pair(dx, dy);
	}

	static float getLength(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
	{
		auto delta = getDelta(p1, p2);
		auto dx = delta.first;
		auto dy = delta.second;

		return sqrtf(dx * dx + dy * dy);
	}

	static std::pair<float, float> getMiddlePos(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
	{
		float x1 = p1->getX();
		float y1 = p1->getY();
		float x2 = p2->getX();
		float y2 = p2->getY();
		float middle_x = (x2 + x1) * 0.5f;
		float middle_y = (y2 + y1) * 0.5f;

		return std::make_pair(middle_x, middle_y);
	}

	static std::pair<float, float> getRelativePos(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, const float relation)
	{
		float x1 = p1->getX();
		float y1 = p1->getY();
		float x2 = p2->getX();
		float y2 = p2->getY();
		float middle_x = (x2 + x1) * relation;
		float middle_y = (y2 + y1) * relation;

		return std::make_pair(middle_x, middle_y);
	}

	static std::pair<float, float> getRelativePos(std::pair<float, float> pos_begin, std::pair<float, float> pos_end, const float relation)
	{
		float x1 = pos_begin.first;
		float y1 = pos_begin.second;
		float x2 = pos_end.first;
		float y2 = pos_end.second;
		float middle_x = x1 + (x2 - x1) * relation;
		float middle_y = y1 + (y2 - y1) * relation;

		return std::make_pair(middle_x, middle_y);
	}

	void translate(std::pair<int, int> offset)
	{
		auto this_line = std::make_shared<Line>(*this);
		translate(this_line, offset);
	}

	static void translate(std::shared_ptr<Line> line, const std::pair<int,int> offset)
	{
		auto p1 = line->begin;
		auto p2 = line->end;

		p1->setX(p1->getX() + offset.first);
		p1->setY(p1->getY() + offset.second);

		p2->setX(p2->getX() + offset.first);
		p2->setY(p2->getY() + offset.second);
	}
};

struct Bezie : public IDrawable
{
	const D2D1::ColorF color = D2D1::ColorF::Red;

	std::shared_ptr<Point> P1, P2, P3, P4;
	static constexpr float N = 100.0f;
	static constexpr float dt = 1 / N;

	Bezie(
		std::shared_ptr<Point> P1,
		std::shared_ptr<Point> P2,
		std::shared_ptr<Point> P3,
		std::shared_ptr<Point> P4)
		: P1(P1), P2(P2), P3(P3), P4(P4) {};
	void Draw(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush)
	{
		pBrush->SetColor(color);
		float xtemp = P1->getX();
		float ytemp = P1->getY();

		for (float t = dt; t < 1.0f; t += dt)
		{
			float x = func(t, P1->getX(), P2->getX(), P3->getX(), P4->getX());
			float y = func(t, P1->getY(), P2->getY(), P3->getY(), P4->getY());
			pRT->DrawLine(D2D1::Point2F(xtemp, ytemp), D2D1::Point2F(x, y), pBrush, 2.0f);

			xtemp = x;
			ytemp = y;
		}
	}
	inline int func(const float t, int p1, int p2, int p3, int p4)
	{
		//P = (1−t)^3 * P1 + 3(1−t)^2 * t * P2 +3 * (1−t)*t^2*P3 + t^3*P4
		const float nt = (1.0f - t);
		return nt * nt * nt * p1 + 3 * nt * nt * t * p2 + 3 * nt * t * t * p3 + t * t * t * p4;
	}
};

class Node
{
public:
	enum class Type
	{
		First,
		Last,
		Internal
	};

private:
	Node::Type type;
	std::shared_ptr<Point> main, sup1, sup2;
	std::shared_ptr<Point> last,next;
	std::shared_ptr<Line> sup_line = nullptr;
public:
	Node(
		Node::Type type,
		std::shared_ptr<Point> main,
		std::shared_ptr<Point> sup1 = nullptr,
		std::shared_ptr<Point> last = nullptr):
		type(type),
		main(main),
		sup1(sup1),
		last(last)
	{}
	void setType(Node::Type new_type)
	{
		type = new_type;
	}
	Node::Type getType()
	{
		return type;
	}
	void bindSecond(std::shared_ptr<Point> second)
	{
		this->next = second;
	}
	std::shared_ptr<Point>  getMain() const
	{
		return main;
	}
	std::shared_ptr<Point>  getSup1() const
	{
		return sup1;
	}
	std::shared_ptr<Point>  getSup2() const
	{
		return sup2;
	}
	void bindLastSup(std::shared_ptr<Point> sup1)
	{
		this->sup1 = sup1;
	}
	void bindNextSup(std::shared_ptr<Point> sup2)
	{
		this->sup2 = sup2;
	}
	void setLast(std::shared_ptr<Point> new_last)
	{
		last = new_last;
	}
	void setNext(std::shared_ptr<Point> new_next)
	{
		next = new_next;
	}

	std::shared_ptr<Line> getSupLine()
	{		
		return sup_line;
	}


	void calcSupPoints()
	{
		switch (type)
		{
		case Node::Type::Internal:
		{
			if (last != nullptr && next != nullptr)
			{
				auto last_mid = Line::getMiddlePos(last, main);
				auto next_mid = Line::getMiddlePos(main, next);

				sup1->SetPos(last_mid);
				sup2->SetPos(next_mid);

				sup_line = std::make_shared<Line>(sup1, sup2);

				auto last_length = Line::getLength(last, main);
				auto next_length = Line::getLength(main, next);

				auto relation = last_length / (last_length + next_length);

				auto relation_point_pos = Line::getRelativePos(last_mid, next_mid, relation);
				auto relation_main_offset = Line::getDelta(relation_point_pos,main->GetPos());

				sup_line->translate(relation_main_offset);
			}
			break;
		}
		default:
			break;
		}	
	}
};