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

	while (const auto e = rwnd.kbd.ReadKey())
	{
		switch (e->GetCode())
		{
		case VK_SPACE:
			if (e->IsPress())
			{
				gc.context.changeSupVisible();
				gc.context.changeLinesVisible();
			}
			break;
		case VK_ESCAPE:
			if (e->IsPress())
			{
				gc.resetContext();
			}
			break;
		}
	}
}

void Application::DoFrame()
{
	rwnd.GetRes().Render(gc.context);
}
