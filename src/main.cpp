#include <RayTracer/MainWindow.h>

#include <QApplication>

int main(int argc, char* argv[])
{
	const int w = 800;
	const int h = 600;
	QApplication app(argc, argv);
	MainWindow mw{w, h};

	mw.resize(w, h);
	mw.show();
	app.exec();
}