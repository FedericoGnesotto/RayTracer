#include <RayTracer/MainWindow.h>

#include <QCoreApplication>
#include <QDoubleSpinbox>
#include <QEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QResizeEvent>
#include <QTimer>

MainWindow::MainWindow(int w, int h)
	: m_camera{45., 0.1, 100.}
	, m_cameraInter{m_camera}
{
	m_camera.resize(w, h);
	m_imageWidget = new QLabel();
	m_updateTimeLabel = new QLabel();
	m_updateTimeLabel->setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed});
	auto vLayout = new QVBoxLayout();
	vLayout->addWidget(m_updateTimeLabel);

	// set the scene
	m_scene.lightDir = vec3(1., -1., -1).normalized();
	m_scene.spheres.emplace_back(vec3{0, 0, -1}, .50, vec3(0.1, 0.6, 0.8));
	m_scene.spheres.emplace_back(vec3{0, 0, -1}, 0.25, vec3(0.8, 0.1, 0.4));

	for (auto i = 0; i < m_scene.spheres.size(); ++i)
	{
		auto sphereControl = new QGroupBox();
		sphereControl->setMaximumWidth(200);
		sphereControl->setTitle("Sphere " + QString::number(i + 1));

		auto sphereControlLayout = new QHBoxLayout();

		auto xControl = new QDoubleSpinBox();
		auto yControl = new QDoubleSpinBox();
		auto zControl = new QDoubleSpinBox();

		xControl->setSingleStep(0.1);
		yControl->setSingleStep(0.1);
		zControl->setSingleStep(0.1);

		xControl->setRange(-10., 10.);
		yControl->setRange(-10., 10.);
		zControl->setRange(-10., 10.);

		xControl->setValue(m_scene.spheres[i].origin.x());
		yControl->setValue(m_scene.spheres[i].origin.y());
		zControl->setValue(m_scene.spheres[i].origin.z());

		sphereControlLayout->addWidget(xControl);
		sphereControlLayout->addWidget(yControl);
		sphereControlLayout->addWidget(zControl);

		vLayout->addWidget(sphereControl);

		sphereControl->setLayout(sphereControlLayout);

		connect(xControl,
				static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
				this,
				[this, xControl, i](double val) {
					m_scene.spheres[i].origin.x() = val;
					doUpdate();
				});
		connect(yControl,
				static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
				this,
				[this, i, yControl](double val) {
					m_scene.spheres[i].origin.y() = val;
					doUpdate();
				});
		connect(zControl,
				static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
				this,
				[this, i, zControl](double val) {
					m_scene.spheres[i].origin.z() = val;
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

void MainWindow::render()
{
	auto start = std::chrono::high_resolution_clock::now();
	m_renderer.render(*m_img, m_camera, m_scene);
	auto stop = std::chrono::high_resolution_clock::now();
	m_updateTimeLabel->setText(QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()));
	m_imageWidget->setPixmap(QPixmap::fromImage(*m_img));
}

void MainWindow::doUpdate() { QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest)); }


bool MainWindow::event(QEvent* event)
{
	if (event->type() == QEvent::UpdateRequest)
	{
		render();
		return QMainWindow::event(event);
	}

	event->setAccepted(false);

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
