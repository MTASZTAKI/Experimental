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

#ifndef APE_MULTIKINECTPLUGIN_H
#define APE_MULTIKINECTPLUGIN_H

#include <iostream>
#include <string>
#include <thread> 
#include <fstream>
#include <strsafe.h>
#include <sstream>
#include <string>
#include <iostream>
#include "utils/apeInterpolator.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "plugin/apePluginAPI.h"
#include "plugin/apeIPlugin.h"
#include "managers/apeICoreConfig.h"
#include "sceneelements/apeINode.h"
#include "managers/apeISceneManager.h"
#include "managers/apeIEventManager.h"
#include "managers/apeILogManager.h"
#include "sceneelements/apeICamera.h"
#include "sceneelements/apeITextGeometry.h"
#include "sceneelements/apeIManualMaterial.h"
#include "sceneelements/apeISphereGeometry.h"
#include "sceneelements/apeIManualPass.h"
#include "sceneelements/apeITubeGeometry.h"
#include "sceneelements/apeIPointCloud.h"
#include "sceneelements/apeIFileGeometry.h"
#include "libfreenect2\libfreenect2.hpp"
#include "libfreenect2\frame_listener_impl.h"
#include "libfreenect2\registration.h"
#include "libfreenect2\packet_pipeline.h"
#include "libfreenect2\depth_packet_processor.h"
#include "libfreenect2\logger.h"

#define THIS_PLUGINNAME "apeMultiKinectPlugin"

namespace ape
{
	class MultiKinectPlugin : public ape::IPlugin
	{
	public:
		MultiKinectPlugin();

		~MultiKinectPlugin();

		void Init() override;

		void Run() override;

		void Step() override;

		void Stop() override;

		void Suspend() override;

		void Restart() override;

		void eventCallBack(const ape::Event& event);

		struct Sensor
		{
			std::string serial;
			int id;
			libfreenect2::Registration* registration;
			libfreenect2::Frame* undistorted;
			libfreenect2::Frame* registered;
			libfreenect2::Freenect2Device* device;
			libfreenect2::SyncMultiFrameListener* listener;
			libfreenect2::FrameMap frames;
			libfreenect2::PacketPipeline* packePipeline;
			float maxDepth;
			ape::PointCloudPoints points;
			ape::PointCloudColors colors;
			ape::Vector3 position;
			ape::Quaternion orientation;
			ape::PointCloudWeakPtr pointCloud;
			ape::NodeWeakPtr pointCloudNode;
			float pointSize;
			bool pointScale;
			float pointScaleOffset;
			float unitScaleDistance;
			float scaleFactor;

			Sensor() : serial(std::string()), id(-1), registration(nullptr), undistorted(nullptr), registered(nullptr), device(nullptr),
				listener(nullptr),
				frames(libfreenect2::FrameMap()), packePipeline(nullptr), maxDepth(0.0f), points(ape::PointCloudPoints()), colors(ape::PointCloudColors()),
				position(ape::Vector3()), orientation(ape::Quaternion()), pointCloud(ape::PointCloudWeakPtr()), pointCloudNode(ape::NodeWeakPtr()), pointSize(0.0f),
				pointScale(false), pointScaleOffset(0.0f),
				unitScaleDistance(0.0f), scaleFactor(0.0f) {}

			Sensor(std::string serial, int id, libfreenect2::Registration* registration, libfreenect2::Frame* undistorted, libfreenect2::Frame* registered,
				libfreenect2::Freenect2Device* device, libfreenect2::SyncMultiFrameListener* listener, libfreenect2::FrameMap frames, libfreenect2::PacketPipeline* packePipeline, 
				float maxDepth, ape::PointCloudPoints points, ape::PointCloudColors colors,
			    ape::Vector3 position, ape::Quaternion orientation, ape::PointCloudWeakPtr pointCloud, ape::NodeWeakPtr pointCloudNode, float pointSize, bool pointScale,
				float pointScaleOffset, float unitScaleDistance, float scaleFactor)
			{
				this->serial = serial;
				this->id = id;
				this->registration = registration;
				this->undistorted = undistorted;
				this->registered = registered;
				this->device = device;
				this->listener = listener;
				this->frames = frames;
				this->packePipeline = packePipeline;
				this->maxDepth = maxDepth;
				this->points = points;
				this->colors = colors;
				this->position = position;
				this->orientation = orientation;
				this->pointCloud = pointCloud;
				this->pointCloudNode = pointCloudNode;
				this->pointSize = pointSize;
				this->pointScale = pointScale;
				this->pointScaleOffset = pointScaleOffset;
				this->unitScaleDistance = unitScaleDistance;
				this->scaleFactor = scaleFactor;
			}
		};

	private:
		libfreenect2::Freenect2 mFreenect2;

		std::vector<Sensor> mSensors;

		const unsigned int mWidth = 512;

		const unsigned int mHeight = 424;

		ape::ISceneManager* mpScene;

		ape::ICoreConfig* mpCoreConfig;

		ape::IEventManager* mpEventManager;
	};

	APE_PLUGIN_FUNC ape::IPlugin* CreateMultiKinectPlugin()
	{
		return new ape::MultiKinectPlugin;
	}

	APE_PLUGIN_FUNC void DestroyMultiKinectPlugin(ape::IPlugin *plugin)
	{
		delete (ape::MultiKinectPlugin*)plugin;
	}

	APE_PLUGIN_DISPLAY_NAME(THIS_PLUGINNAME);

	APE_PLUGIN_ALLOC()
	{
		APE_LOG_DEBUG(THIS_PLUGINNAME << "_CREATE");
		apeRegisterPlugin(THIS_PLUGINNAME, CreateMultiKinectPlugin, DestroyMultiKinectPlugin);
		return 0;
	}
}
#endif