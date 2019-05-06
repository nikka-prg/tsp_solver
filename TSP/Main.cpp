
# include <Siv3D.hpp> // OpenSiv3D v0.3.0

#include "TspDriver.h"

void Main()
{
	Graphics::SetBackground(ColorF(0.8, 0.9, 1.0));

	const Font font(60);

	TspDriver tsp_driver;

	Stopwatch stop_watch;
	stop_watch.start();

	while (System::Update())
	{
		if (250 <= stop_watch.ms()) {
			tsp_driver.ExecuteOneStep();
			stop_watch.restart();
		}

		tsp_driver.Draw(font);
	}
}
