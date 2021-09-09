#include "Application.h"

Application::Application()
{
	if (!rwnd.Create(L"Test render window", WS_OVERLAPPEDWINDOW))
	{
		Application::~Application();
	}
	ShowWindow(rwnd.Window(), SW_SHOWDEFAULT);
}

Application::~Application()
{

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
		const auto& points = gc.context.points;
		const auto& lines = gc.context.lines;
		const auto& splines = gc.context.bezies;


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
			if (!gc.getSelectedPoint())
			{
				gc.SetPoint();
			}
			if (!gc.OnDrawingLine())
			{
				gc.StartDraggingPoint();
			}
			if (points.size() % 2 == 0 && points.size() > 2)
			{
				gc.MakeSpline();
			}

			break;
		}
		case Mouse::Event::Type::LRelease:
		{
			gc.EndMakingLine();
			gc.StopDraggingPoint();

			break;
		}
		}
	}
}

void Application::DoFrame()
{
	rwnd.GetRes().Render(gc.context);
}
