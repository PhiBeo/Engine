#include "Precompiled.h"
#include "PhysicsDebugDrawer.h"

#include <Graphics/Inc/SimpleDraw.h>

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Physics;

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	SimpleDraw::AddLine(ConvertToVector3(from), ConvertToVector3(to), ConvertToColor(color));
}

void PhysicsDebugDraw::drawContactPoint(const btVector3& PontOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	//Empty
}

void PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
	LOG("[PhysicsWarning] %s", warningString);
}

void PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
	//Empty
}

void PhysicsDebugDraw::setDebugMode(int debugMode)
{
	mDebugMode = debugMode;
}

int PhysicsDebugDraw::getDebugMode() const
{
	return mDebugMode;
}
