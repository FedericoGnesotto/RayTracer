#pragma once

#include <RayTracer/Camera.h>
#include <RayTracer/CameraInteraction.h>
#include <RayTracer/Math.h>
#include <RayTracer/Renderer.h>
#include <RayTracer/Scene.h>

#include <QMainWindow>

class QLabel;
class QEvent;

class MainWindow : public QMainWindow
{
public:
	MainWindow(int w, int h);

	void render();

	void doUpdate();

	bool event(QEvent* event) override;

private:
	std::unique_ptr<QImage> m_img{};
	Renderer m_renderer{};
	QLabel* m_imageWidget;
	QLabel* m_updateTimeLabel;
	Scene m_scene;
	Camera m_camera;
	CameraInteraction m_cameraInter;
};