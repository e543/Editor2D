#pragma once
#include <d2d1.h>
#include <memory>

struct IDrawable
{
	virtual void Draw(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush) = 0;
	bool isSelected;
	virtual bool Selected(std::pair<int, int> mouse_pos) = 0;
};


struct Point : public IDrawable
{
	D2D1_ELLIPSE ellipse;
	const D2D1::ColorF color = D2D1::ColorF::Black;
	const float rad = 4.0f;
	const float gap = 10.0f;
	Point(int x, int y)
	{
		ellipse.point.x = x;
		ellipse.point.y = y;
		ellipse.radiusX = ellipse.radiusY = rad;
	}


	void Draw(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush) override
	{
		pBrush->SetColor(color);
		pRT->FillEllipse(ellipse, pBrush);
		pBrush->SetColor(isSelected ? D2D1::ColorF(D2D1::ColorF::Red) : D2D1::ColorF(D2D1::ColorF::Black));
		pRT->DrawEllipse(ellipse, pBrush, 2.0f);
	}

	bool Selected(std::pair<int, int> mouse_pos) override
	{
		const float x1 = mouse_pos.first - ellipse.point.x;
		const float y1 = mouse_pos.second - ellipse.point.y;
		const float sqr_d = (x1 * x1) + (y1 * y1);
		isSelected = (sqrt(sqr_d) <= (rad + gap));
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
};

struct Line : public IDrawable
{
	std::shared_ptr<Point> begin;
	std::shared_ptr<Point> end;
	D2D1::ColorF color = D2D1::ColorF::Yellow;
	Line(std::shared_ptr<Point> begin, std::shared_ptr<Point> end) : begin(begin), end(end) {};

	void Draw(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush) override
	{
		pBrush->SetColor(color);

		// if selected
		//pBrush->SetColor(isSelect ? D2D1::ColorF(D2D1::ColorF::White) : D2D1::ColorF(D2D1::ColorF::Black));

		pRT->DrawLine(D2D1::Point2F(begin->getX(), begin->getY()), D2D1::Point2F(end->getX(), end->getY()), pBrush, 2.0f);
	}

	bool Selected(std::pair<int, int> mouse_pos) override
	{
		return false;
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
	static float getLength(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
	{
		auto delta = getDelta(p1, p2);
		auto dx = delta.first;
		auto dy = delta.second;

		return sqrtf(dx * dx + dy * dy);
	}

	void translate(const float offset_x = 0.0f, const float offset_y = 0.0f)
	{
		auto this_line = std::make_shared<Line>(*this);
		translate(this_line, offset_x, offset_y);
	}

	static std::shared_ptr<Point> getMiddle(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
	{
		float x1 = p1->getX();
		float y1 = p1->getY();
		float x2 = p2->getX();
		float y2 = p2->getY();
		float middle_x = (x2 - x1) / 2.0f;
		float middle_y = (y2 - y1) / 2.0f;

		return std::make_shared<Point>(middle_x, middle_y);
	}
	static void translate(std::shared_ptr<Line> line, const float offset_x = 0.0f, const float offset_y = 0.0f)
	{
		auto p1 = line->begin;
		auto p2 = line->end;

		p1->setX(p1->getX() + offset_x);
		p1->setY(p1->getY() + offset_y);

		p2->setX(p2->getX() + offset_x);
		p2->setY(p2->getY() + offset_y);
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
	bool Selected(std::pair<int, int> mouse_pos) override
	{
		return false;
	}
	inline int func(const float t, int p1, int p2, int p3, int p4)
	{
		//P = (1−t)^3 * P1 + 3(1−t)^2 * t * P2 +3 * (1−t)*t^2*P3 + t^3*P4
		const float nt = (1.0f - t);
		return nt * nt * nt * p1 + 3 * nt * nt * t * p2 + 3 * nt * t * t * p3 + t * t * t * p4;
	}
};

struct Spline
{
	
};