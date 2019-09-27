#include "apeZedPlugin.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

ape::ZedPlugin::ZedPlugin()
{
	APE_LOG_FUNC_ENTER();
	mpSceneManager = ape::ISceneManager::getSingletonPtr();
	mpEventManager = ape::IEventManager::getSingletonPtr();
	mpCoreConfig = ape::ICoreConfig::getSingletonPtr();
	mpEventManager->connectEvent(ape::Event::Group::NODE, std::bind(&ZedPlugin::eventCallBack, this, std::placeholders::_1));
	mZedResolutionWidth = 0;
	mZedResolutionHeight = 0;
	mPointCloudSize = 0;
	mPointCloudBoundingSphereRadius = 100000;
	mApePointCloudPoints = ape::PointCloudPoints();
	mApePointCloudColors = ape::PointCloudColors();
	mApePointCloudNode = ape::NodeWeakPtr();
	mApePointCloud = ape::PointCloudWeakPtr();
	APE_LOG_FUNC_LEAVE();
}

ape::ZedPlugin::~ZedPlugin()
{
	APE_LOG_FUNC_ENTER();
	mpEventManager->disconnectEvent(ape::Event::Group::NODE, std::bind(&ZedPlugin::eventCallBack, this, std::placeholders::_1));
	mZedPointCloud.free(sl::MEM_CPU);
	mZed.close();
	APE_LOG_FUNC_LEAVE();
}

void ape::ZedPlugin::eventCallBack(const ape::Event& event)
{

}

void ape::ZedPlugin::Init()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_DEBUG("try to init Zed");
	sl::InitParameters initParameters;
	initParameters.camera_resolution = sl::RESOLUTION_HD1080;
	initParameters.depth_mode = sl::DEPTH_MODE_ULTRA;
	initParameters.coordinate_system = sl::COORDINATE_SYSTEM_RIGHT_HANDED_Y_UP;
	initParameters.coordinate_units = sl::UNIT_CENTIMETER;
	sl::ERROR_CODE err = mZed.open(initParameters);
	if (err != sl::SUCCESS) {
		APE_LOG_DEBUG("Zed error: " << sl::toString(err));
		mZed.close();
	}
	mZed.setConfidenceThreshold(95);
	mZedResolutionWidth = (int)mZed.getResolution().width;
	mZedResolutionHeight = (int)mZed.getResolution().height;
	mPointCloudSize = mZedResolutionWidth * mZedResolutionHeight;
	mApePointCloudPoints.resize(mPointCloudSize * 3);
	mApePointCloudColors.resize(mPointCloudSize * 3);
	APE_LOG_DEBUG("Zed init was successful, point cloud size: " << mPointCloudSize);
	if (auto apePointCloudNode = mpSceneManager->createNode("ZedPointCloudNode").lock())
	{
		if (auto apePointCloud = std::static_pointer_cast<ape::IPointCloud>(mpSceneManager->createEntity("ZedPointCloud", ape::Entity::Type::POINT_CLOUD).lock()))
		{
			apePointCloud->setParameters(mApePointCloudPoints, mApePointCloudColors, 100000, 1.0f, true, 500.0f, 500.0f, 3.0f);
			apePointCloud->setParentNode(apePointCloudNode);
			mApePointCloud = apePointCloud;
		}
		if (auto text = std::static_pointer_cast<ape::ITextGeometry>(mpSceneManager->createEntity("ZedText", ape::Entity::GEOMETRY_TEXT).lock()))
		{
			text->showOnTop(true);
			text->setCaption("ZedMini");
			text->setParentNode(apePointCloudNode);
		}
		mApePointCloudNode = apePointCloudNode;
	}
	APE_LOG_FUNC_LEAVE();
}

void ape::ZedPlugin::Run()
{
	APE_LOG_FUNC_ENTER();
	sl::RuntimeParameters runtime_parameters;
	runtime_parameters.sensing_mode = sl::SENSING_MODE_STANDARD;
	while (true)
	{
		if (mZed.grab(runtime_parameters) == sl::SUCCESS)
		{
			mApePointCloudPoints.clear();
			mApePointCloudColors.clear();
			mZed.retrieveMeasure(mZedPointCloud, sl::MEASURE_XYZRGBA);
			for (int i = 0; i < mZedResolutionWidth; i++)
			{
				for (int j = 0; j < mZedResolutionHeight; j++)
				{
					sl::float4 point_cloud_value;
					mZedPointCloud.getValue(i, j, &point_cloud_value);
					mApePointCloudPoints.push_back(point_cloud_value.x);
					mApePointCloudPoints.push_back(point_cloud_value.y);
					mApePointCloudPoints.push_back(point_cloud_value.z);
					dataUnionBytesFloat myUnion;
					myUnion.f = point_cloud_value.w;
					mApePointCloudColors.push_back(myUnion.fBuff[0] / 255.0f);
					mApePointCloudColors.push_back(myUnion.fBuff[1] / 255.0f);
					mApePointCloudColors.push_back(myUnion.fBuff[2] / 255.0f);
				}
			}
			if (auto apePointCloud = mApePointCloud.lock())
			{
				apePointCloud->updatePoints(mApePointCloudPoints);
				apePointCloud->updateColors(mApePointCloudColors);
			}
		}
	}
	APE_LOG_FUNC_LEAVE();
}

void ape::ZedPlugin::Step()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_FUNC_LEAVE();
}

void ape::ZedPlugin::Stop()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_FUNC_LEAVE();
}

void ape::ZedPlugin::Suspend()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_FUNC_LEAVE();
}

void ape::ZedPlugin::Restart()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_FUNC_LEAVE();
}
