#include <RayTracer/Camera.h>
#include <RayTracer/CameraInteraction.h>
#include <RayTracer/Math.h>
#include <RayTracer/Renderer.h>

#include <QApplication>
#include <QDockWidget>
#include <QDoubleSpinbox>
#include <QEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QResizeEvent>
#include <QTimer>
#include <iostream>

class MainWindow : public QMainWindow
{
public:
	MainWindow(int w, int h)

		: m_camera{45., 0.1, 100.}
		, m_cameraInter{m_camera}
	{
		m_camera.resize(w, h);
		m_imageWidget = new QLabel();
		m_updateTimeLabel = new QLabel();
		m_updateTimeLabel->setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed});
		auto vLayout = new QVBoxLayout();
		vLayout->addWidget(m_updateTimeLabel);

		{
			auto sphereControl = new QGroupBox();
			sphereControl->setMaximumWidth(200);
			auto sphereControlLayout = new QHBoxLayout();

			auto xControl = new QDoubleSpinBox();
			auto yControl = new QDoubleSpinBox();
			auto zControl = new QDoubleSpinBox();

			xControl->setSingleStep(0.1);
			yControl->setSingleStep(0.1);
			zControl->setSingleStep(0.1);

			xControl->setRange(-1., 1.);
			yControl->setRange(-1., 1.);
			zControl->setRange(-10., 10.);

			sphereControlLayout->addWidget(xControl);
			sphereControlLayout->addWidget(yControl);
			sphereControlLayout->addWidget(zControl);

			sphereControl->setTitle("Sphere Control");
			sphereControl->setLayout(sphereControlLayout);
			vLayout->addWidget(sphereControl);


			Sphere& s = m_spheres.emplace_back(vec3::Zero(), 1.0, vec3(0.2, 0.3, 0.5));

			connect(xControl,
					static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
					this,
					[this, p = &s, xControl](double val) {
						p->origin.x() = val;
						doUpdate();
					});
			connect(yControl,
					static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
					this,
					[this, p = &s, yControl](double val) {
						p->origin.y() = val;
						doUpdate();
					});
			connect(zControl,
					static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
					this,
					[this, p = &s, zControl](double val) {
						p->origin.z() = val;
						doUpdate();
					});
		}

		vLayout->addStretch();

		auto mainWidget = new QWidget();
		auto hLayout = new QHBoxLayout();
		hLayout->addWidget(m_imageWidget);
		hLayout->addLayout(vLayout);
		mainWidget->setLayout(hLayout);

		m_img = std::make_unique<QImage>(w, h, QImage::Format_ARGB32);

		setCentralWidget(mainWidget);

		setFocusPolicy(Qt::StrongFocus);

		auto myTimer = new QTimer(this);
		myTimer->setSingleShot(false);
		myTimer->start(500);
		connect(myTimer, &QTimer::timeout, this, &MainWindow::doUpdate);

		setMouseTracking(true);
	}

	void render()
	{
		auto start = std::chrono::high_resolution_clock::now();
		m_renderer.render(*m_img, m_camera, m_spheres);
		auto stop = std::chrono::high_resolution_clock::now();
		m_updateTimeLabel->setText(
			QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()));
		m_imageWidget->setPixmap(QPixmap::fromImage(*m_img));
	}

	void doUpdate() { QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest)); }


	bool event(QEvent* event) override
	{
		if (event->type() == QEvent::UpdateRequest)
		{
			render();
			return QMainWindow::event(event);
		}

		event->setAccepted(false);

		//return QMainWindow::event(event);
		if (event->type() == QEvent::Type::Resize)
		{
			auto re = static_cast<QResizeEvent*>(event);

			m_cameraInter.resizeEvent(m_imageWidget->width(), m_imageWidget->height());

			m_img = std::make_unique<QImage>(m_imageWidget->width(), m_imageWidget->height(), QImage::Format_ARGB32);

			doUpdate();

			MainWindow::resizeEvent(re);
		}
		if (m_cameraInter.sceneEvent(event))
		{
			doUpdate();
			event->accept();
			return true;
		}
		return QMainWindow::event(event);
	}

private:
	std::unique_ptr<QImage> m_img{};
	Renderer m_renderer{};
	QLabel* m_imageWidget;
	QLabel* m_updateTimeLabel;
	std::vector<Sphere> m_spheres;
	Camera m_camera;
	CameraInteraction m_cameraInter;
};

int main(int argc, char* argv[])
{
	const int w = 800;
	const int h = 600;
	QApplication app(argc, argv);
	MainWindow mw{w, h};


	//{

	//	QDockWidget* dockWidget = new QDockWidget(&mw);
	//	QWidget* dockWidgetContents = new QWidget();
	//	QLayout* vLayout = new QVBoxLayout();
	//	vLayout->addWidget(new QLabel("Controls"));
	//	dockWidgetContents->setLayout(vLayout);

	//	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
	//		Qt::RightDockWidgetArea);
	//	dockWidget->setWidget(dockWidgetContents);
	//	mw.addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
	//}


	mw.resize(w, h);
	mw.show();
	app.exec();
}