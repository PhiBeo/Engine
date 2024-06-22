#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace SpringEngine;
using namespace SpringEngine::Physics;

namespace
{
	std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

PhysicsWorld::~PhysicsWorld()
{
	ASSERT(mDynamicWorld == nullptr, "PhysicsWorld: terminate must be called");
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
	ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is already initializeed!");
	sPhysicsWorld = std::make_unique<PhysicsWorld>();
	sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
	if (sPhysicsWorld != nullptr)
	{
		sPhysicsWorld->Terminate();
		sPhysicsWorld.reset();
	}
}

PhysicsWorld* PhysicsWorld::Get()
{
	ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld: is not initialized!");
	return sPhysicsWorld.get();
}

void PhysicsWorld::Initialize(const Settings& settings)
{
	mSettings = settings;
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mInterface = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();
	mDynamicWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
	mDynamicWorld->setGravity(ConvertTobtVector3(settings.gravity));
	mDynamicWorld->setDebugDrawer(&mPhysicsDebugDraw);

	//mSoftBodyWorld = new btSoftRigidDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
	//mSoftBodyWorld->setGravity(ConvertTobtVector3(settings.gravity));
	//->setDebugDrawer(&mPhysicsDebugDraw);
}

void PhysicsWorld::Terminate()
{
	//SafeDelete(mSoftBodyWorld);
	SafeDelete(mDynamicWorld);
	SafeDelete(mSolver);
	SafeDelete(mInterface);
	SafeDelete(mDispatcher);
	SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
	mDynamicWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
	//mSoftBodyWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update();
	}
}

void PhysicsWorld::DebugUI()
{
	if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("DebugDraw", &mDebugDraw);
		if (mDebugDraw)
		{
			int debugDraw = mPhysicsDebugDraw.getDebugMode();
			bool isEnabled = (debugDraw & btIDebugDraw::DBG_DrawWireframe);
			if (ImGui::Checkbox("[DBG]Wireframe", &isEnabled))
			{
				debugDraw = (isEnabled) ? debugDraw | btIDebugDraw::DBG_DrawWireframe : debugDraw & ~btIDebugDraw::DBG_DrawWireframe;
			}
			isEnabled = (debugDraw & btIDebugDraw::DBG_DrawAabb);
			if (ImGui::Checkbox("[DBG]DrawAABB", &isEnabled))
			{
				debugDraw = (isEnabled) ? debugDraw | btIDebugDraw::DBG_DrawAabb : debugDraw & ~btIDebugDraw::DBG_DrawAabb;
			}

			mPhysicsDebugDraw.setDebugMode(debugDraw);
			mDynamicWorld->debugDrawWorld();
			//mSoftBodyWorld->debugDrawWorld();
		}
	}
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter == mPhysicsObjects.end())
	{
		mPhysicsObjects.push_back(physicsObject);
		if (physicsObject->GetSoftBody() != nullptr)
		{
			//mSoftBodyWorld->addSoftBody(physicsObject->GetSoftBody());
		}
		else if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicWorld->addRigidBody(physicsObject->GetRigidBody());
		}
	}
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter != mPhysicsObjects.end())
	{
		if (physicsObject->GetSoftBody() != nullptr)
		{
			//mSoftBodyWorld->removeSoftBody(physicsObject->GetSoftBody());
		}
		else if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicWorld->removeRigidBody(physicsObject->GetRigidBody());
		}
		mPhysicsObjects.erase(iter);
	}
}

btSoftBody* SpringEngine::Physics::PhysicsWorld::CreateSoftBody(int nodeCount)
{
	btSoftBody* softBody = new btSoftBody(&mSoftBodyWorld->getWorldInfo(), nodeCount, nullptr, nullptr);
	return softBody;
}
