#include "Application.h"

Application::Application()
{
	if (!rwnd.Create(L"Bezier spline graphic editor", WS_OVERLAPPEDWINDOW))
	{
		Application::~Application();
	}
	ShowWindow(rwnd.Window(), SW_SHOWDEFAULT);
}


int Application::Go()
{
	while (true)
	{
		if (const auto ecode = RenderWindow::ProcessMessages())
		{
			return *ecode;
		}

		HandleInput();
		DoFrame();
	}

	return 0;
}


void Application::HandleInput()
{
	while (const auto e = rwnd.mouse.Read())
	{
		const auto& sup_points = gc.context.sup_points;
		const auto& main_points = gc.context.main_points;
		const auto& lines = gc.context.lines;
		const auto& splines = gc.context.beziers;


		switch (e->GetType())
		{
		case Mouse::Event::Type::Move:
		{
			gc.MousePosChanged(e->GetPos());
			if (gc.OnDrawingLine())
			{
				gc.ResizeLine();
			}
			if (gc.OnDraggingPoint())
			{
				gc.dragPoint();
			}
			break;
		}
		case Mouse::Event::Type::LPress:
		{
			if (gc.isNewSpline())
			{
				gc.addNode(Node::Type::Single);
				gc.continueSpline();
			}
			else
			if (!gc.getSelectedMainPoint())
			{
				gc.addNode(Node::Type::Last);
			}
			else
			{
				gc.deleteNode();
			}

			gc.StartDraggingPoint();
			break;
		}
		case Mouse::Event::Type::LRelease:
		{
			gc.EndMakingLine();
			gc.StopDraggingPoint();

			break;
		}
		case Mouse::Event::Type::RPress:
		{
			gc.OnNewSpline();
		}
		}
	}
}

void Application::DoFrame()
{
	rwnd.GetRes().Render(gc.context);
}
