#pragma once

#include <QObject>
#include <optional>

class Camera;

class CameraInteraction : public QObject
{
public:
	CameraInteraction(Camera& camera);

	bool sceneEvent(const QEvent* event);

	void resizeEvent(int viewPortWidth, int viewPortHeight);

private:
	vec3 handleTranslation();
	vec3 handleRotation(const vec2&);
	double m_tranlationSpeed = 0.1;
	double m_rotationSpeed = 0.03;
	vec2 m_lastMousePosition = vec2::Zero();
	int m_downKey = -1;
	vec3 m_forwardDirection = vec3::Zero();
	Camera* m_camera;
};