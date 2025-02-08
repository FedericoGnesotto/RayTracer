#include <RayTracer/Camera.h>
#include <RayTracer/CameraInteraction.h>
#include <RayTracer/Math.h>

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <iostream>
CameraInteraction::CameraInteraction(Camera& camera)
	: m_camera{&camera}
	, m_forwardDirection{camera.forwardDir()}
{
}

bool CameraInteraction::sceneEvent(const QEvent* event)
{
	if (event->type() == QEvent::MouseMove)
	{
		auto mouseEvent = static_cast<const QMouseEvent*>(event);

		if (mouseEvent->buttons() == Qt::MouseButton::RightButton)
		{
			QPoint pos = mouseEvent->pos();
			vec2 posVec2 = vec2(pos.x(), pos.y());
			vec2 mouseDelta = posVec2 - m_lastMousePosition;
			mouseDelta *= 0.002;
			m_lastMousePosition = posVec2;

			vec3 forwardDir = handleRotation(mouseDelta);
			if (forwardDir != vec3::Zero())
				m_camera->update(forwardDir, vec3::Zero());
			return true;
		}

		return false;
	}
	if (event->type() == QEvent::KeyPress)
	{
		auto keyEvent = dynamic_cast<const QKeyEvent*>(event);
		if (keyEvent->isAutoRepeat())
		{
			const_cast<QKeyEvent*>(keyEvent)->ignore();
			return false;
		}
		if (keyEvent->key() == Qt::Key_R)
		{
			m_camera->reset();
			return true;
		}
		m_downKey = keyEvent->key();
		return false;
	}

	if (event->type() == QEvent::KeyRelease)
	{
		auto keyEvent = dynamic_cast<const QKeyEvent*>(event);
		if (keyEvent->isAutoRepeat())
		{
			const_cast<QKeyEvent*>(keyEvent)->ignore();
			return false;
		}
		m_downKey = -1;
		return false;
	}

	if (m_downKey != -1)
	{
		auto positionDelta = handleTranslation();

		if (positionDelta != vec3::Zero())
		{
			m_camera->update(m_camera->forwardDir(), positionDelta);
			return true;
		}
	}

	return false;
}


void CameraInteraction::resizeEvent(int viewPortWidth, int viewPortHeight) { m_camera->resize(viewPortWidth, viewPortHeight); }


vec3 CameraInteraction::handleRotation(const vec2& posDelta)
{
	if (posDelta.x() != 0.0 || posDelta.y() != 0.0)
	{
		const vec3 upDirection(0.0, 1.0, 0.0);
		const vec3 rightDirection = m_forwardDirection.cross(upDirection);

		float pitchDelta = posDelta.y() * m_rotationSpeed;
		float yawDelta = posDelta.x() * m_rotationSpeed;

		Eigen::Quaterniond q =
			Eigen::AngleAxisd(-pitchDelta, rightDirection) * Eigen::AngleAxisd(-yawDelta, vec3(0.f, 1.0f, 0.0f));
		m_forwardDirection = q.toRotationMatrix() * m_forwardDirection;

		return m_forwardDirection;
	}
	return vec3::Zero();
}

vec3 CameraInteraction::handleTranslation()
{
	if (m_downKey == -1)
		return vec3::Zero();

	const vec3 upDirection(0.0, 1.0, 0.0);
	const vec3 rightDirection = m_forwardDirection.cross(upDirection).normalized();

	if (m_downKey == Qt::Key_W)    // forward
	{
		return m_forwardDirection * m_tranlationSpeed;
	}
	else if (m_downKey == Qt::Key_S)    // backward
	{
		return -m_forwardDirection * m_tranlationSpeed;
	}
	else if (m_downKey == Qt::Key_D)    // right
	{
		return rightDirection * m_tranlationSpeed;
	}
	else if (m_downKey == Qt::Key_A)    // left
	{
		return -rightDirection * m_tranlationSpeed;
	}
	else if (m_downKey == Qt::Key_E)    // up
	{
		return upDirection * m_tranlationSpeed;
	}
	else if (m_downKey == Qt::Key_Q)    // down
	{
		return -upDirection * m_tranlationSpeed;
	}

	return vec3::Zero();
}
