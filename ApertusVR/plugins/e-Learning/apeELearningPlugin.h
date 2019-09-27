/*MIT License

Copyright (c) 2018 MTA SZTAKI

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef APE_ELEARNINGPLUGIN_H
#define APE_ELEARNINGPLUGIN_H

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "plugin/apePluginAPI.h"
#include "managers/apeILogManager.h"
#include "managers/apeIEventManager.h"
#include "managers/apeISceneManager.h"
#include "managers/apeICoreConfig.h"
#include "sceneelements/apeIFileGeometry.h"
#include "sceneelements/apeIPlaneGeometry.h"
#include "sceneelements/apeIFileMaterial.h"
#include "sceneelements/apeIRayGeometry.h"
#include "sceneelements/apeIBrowser.h"
#include "sceneelements/apeIIndexedLineSetGeometry.h"
#include "sceneelements/apeINode.h"
#include "sceneelements/apeICamera.h"
#include "sceneelements/apeIManualTexture.h"
#include "sceneelements/apeIFileTexture.h"
#include "sceneelements/apeIManualMaterial.h"
#include "macros/userInput/apeUserInputMacro.h"
#include "macros/sceneMaker/apeSceneMakerMacro.h"
#include "datatypes/apeEuler.h"
#include "apeELearningPluginConfig.h"

#define THIS_PLUGINNAME "apeELearningPlugin"

namespace ape
{
	struct RotationPose
	{
		int angle;

		std::string type;

		std::string name;

		RotationPose() {};

		RotationPose(int angle, std::string type, std::string name)
		{
			this->angle = angle;
			this->type = type;
			this->name = name;
		}

		bool operator<(const RotationPose& r) const
		{
			if (angle != r.angle)
				return angle < r.angle;
			return angle < r.angle;
		}

		bool operator>(const RotationPose& r) const
		{
			if (angle != r.angle)
				return angle > r.angle;
			return angle > r.angle;
		}
	};
	class apeELearningPlugin : public ape::IPlugin
	{
	private:
		ape::ISceneManager* mpSceneManager;

		ape::IEventManager* mpEventManager;

		ape::UserInputMacro* mpApeUserInputMacro;

		ape::SceneMakerMacro* mpSceneMakerMacro;

		ape::ICoreConfig* mpCoreConfig;

		std::map<std::string, quicktype::Hotspot> mNodeNamesHotSpots;

		std::vector<quicktype::Room> mRooms;

		std::vector<RotationPose> mCurrentRotationPoses;

		int mCurrentRotationPoseID;

		std::map<std::string, std::string> mGameURLResourcePath;

		ape::Vector3 mUserDeadZone;

		ape::FileGeometryWeakPtr mSphereGeometryLeft;

		ape::FileGeometryWeakPtr mSphereGeometryRight;

		ape::Vector2 mMouseMovedValueAbs;

		int mMouseScrolledValue;

		int mCurrentRoomID;

		ape::UserInputMacro::OverlayBrowserCursor mOverlayBrowserCursor;

		ape::NodeWeakPtr mControllerNode;

		ape::Vector3 mLastHmdPosition;

		ape::Quaternion mLastHmdOrientation;

		ape::RayGeometryWeakPtr mRayGeometry;

		ape::BrowserWeakPtr mBrowser;

		ape::NodeWeakPtr mBrowserNode;

		bool mIsTouchPadPressed;

		bool mIsForwardMovementEnabled;

		int mCurrentSphereAngle;

		quicktype::Welcome mConfig;

	public:
		apeELearningPlugin();

		~apeELearningPlugin();

		ape::FileGeometryWeakPtr createSphere(std::string cameraName, std::string sphereNodeName, std::string meshName, unsigned int visibility);

		void createRoomTextures();

		void createHotSpots();

		void createBrowser();

		void loadRoom(std::string name);

		void loadHotSpots();

		void loadRoomTextures();

		void rotateSpheres(int angle);

		void eventCallBack(const ape::Event& event);

		void keyPressedStringEventCallback(const std::string& keyValue);

		void mousePressedStringEventCallback(const std::string& keyValue);

		void mouseReleasedStringEventCallback(const std::string& keyValue);

		void mouseMovedCallback(const ape::Vector2& mouseMovedValueRel, const ape::Vector2& mouseMovedValueAbs);

		void mouseScrolledCallback(const int& mouseScrolledValue);
		
		void controllerMovedValueCallback(const ape::Vector3& controllerPosition, const ape::Quaternion& controllerOrientation, const ape::Vector3& controllerScale);

		void hmdMovedEventCallback(const ape::Vector3& hmdMovedValuePos, const ape::Quaternion& hmdMovedValueOri, const ape::Vector3& hmdMovedValueScl);

		void controllerTouchpadPressedValue(const ape::Vector2& axis);

		void controllerTouchpadReleasedValue(const ape::Vector2& axis);

		void controllerButtonPressedStringValue(const std::string& buttonValue);

		void findClosestRotationPose(int currentSphereAngle, std::string command);
		
		void Init() override;

		void Run() override;

		void Step() override;

		void Stop() override;

		void Suspend() override;

		void Restart() override;
	};

	APE_PLUGIN_FUNC ape::IPlugin* CreateapeELearningPlugin()
	{
		return new ape::apeELearningPlugin;
	}

	APE_PLUGIN_FUNC void DestroyapeELearningPlugin(ape::IPlugin *plugin)
	{
		delete (ape::apeELearningPlugin*)plugin;
	}

	APE_PLUGIN_DISPLAY_NAME(THIS_PLUGINNAME);

	APE_PLUGIN_ALLOC()
	{
		APE_LOG_DEBUG(THIS_PLUGINNAME << "_CREATE");
		apeRegisterPlugin(THIS_PLUGINNAME, CreateapeELearningPlugin, DestroyapeELearningPlugin);
		return 0;
	}
}

#endif
