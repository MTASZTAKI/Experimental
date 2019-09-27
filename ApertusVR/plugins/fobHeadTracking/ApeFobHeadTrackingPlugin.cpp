#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "apeFobHeadTrackingPlugin.h"

ape::apeFobHeadTrackingPlugin::apeFobHeadTrackingPlugin()
{
	APE_LOG_FUNC_ENTER();
	mpCoreConfig = ape::ICoreConfig::getSingletonPtr();
	mpEventManager = ape::IEventManager::getSingletonPtr();
	mpEventManager->connectEvent(ape::Event::Group::NODE, std::bind(&apeFobHeadTrackingPlugin::eventCallBack, this, std::placeholders::_1));
	mpEventManager->connectEvent(ape::Event::Group::CAMERA, std::bind(&apeFobHeadTrackingPlugin::eventCallBack, this, std::placeholders::_1));
	mpSceneManager = ape::ISceneManager::getSingletonPtr();
	mpFobTracker = nullptr;
	mCameraCount = 0;
	mCameraDoubleQueue = ape::DoubleQueue<ape::CameraWeakPtr>();
	mTrackerConfig = ape::HeadTrackerConfig();
	mDisplayConfigList = ape::HeadTrackerDisplayConfigList();
	mTrackedViewerPosition = ape::Vector3();
	mTrackedViewerOrientation = ape::Quaternion();
	mTrackedViewerOrientationYPR = ape::Euler();
	mNearClip = 0.0f;
	mFarClip = 0.0f;
	mC = 0.0f;
	mD = 0.0f;
	APE_LOG_FUNC_LEAVE();
}

ape::apeFobHeadTrackingPlugin::~apeFobHeadTrackingPlugin()
{
	APE_LOG_FUNC_ENTER();
	mpEventManager->disconnectEvent(ape::Event::Group::NODE, std::bind(&apeFobHeadTrackingPlugin::eventCallBack, this, std::placeholders::_1));
	mpEventManager->disconnectEvent(ape::Event::Group::CAMERA, std::bind(&apeFobHeadTrackingPlugin::eventCallBack, this, std::placeholders::_1));
	APE_LOG_FUNC_LEAVE();
}

void ape::apeFobHeadTrackingPlugin::setCameraConfigByName(std::string cameraName, ape::CameraWeakPtr cameraWkPtr)
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_DEBUG("cameraName: " << cameraName);
	for (int i = 0; i < mDisplayConfigList.size(); i++)
	{
		if (mDisplayConfigList[i].cameraLeftName == cameraName)
		{
			mDisplayConfigList[i].cameraLeft = cameraWkPtr;
			APE_LOG_DEBUG("mDisplayConfigList[" << i << "].cameraLeftName: " << cameraName << " - found");
			return;
		}
		else if (mDisplayConfigList[i].cameraRightName == cameraName)
		{
			mDisplayConfigList[i].cameraRight = cameraWkPtr;
			APE_LOG_DEBUG("mDisplayConfigList[" << i << "].cameraRightName: " << cameraName << " - found");
			return;
		}
	}
	APE_LOG_FUNC_LEAVE();
}

void ape::apeFobHeadTrackingPlugin::eventCallBack(const ape::Event& event)
{
	if (event.type == ape::Event::Type::CAMERA_CREATE)
	{
		if (auto camera = std::static_pointer_cast<ape::ICamera>(mpSceneManager->getEntity(event.subjectName).lock()))
		{
			APE_LOG_DEBUG("camera: " << camera->getName());
			mCameraDoubleQueue.push(camera);
		}
	}
}

void ape::apeFobHeadTrackingPlugin::Init()
{
	APE_LOG_FUNC_ENTER();
	mpFobTracker = trackdInitTrackerReader(4126);
	
	std::stringstream fileFullPath;
	fileFullPath << mpCoreConfig->getConfigFolderPath() << "\\apeFobHeadTrackingPlugin.json";
	FILE* apeFobHeadTrackingPluginConfigFile = std::fopen(fileFullPath.str().c_str(), "r");
	char readBuffer[65536];
	if (apeFobHeadTrackingPluginConfigFile)
	{
		rapidjson::FileReadStream jsonFileReaderStream(apeFobHeadTrackingPluginConfigFile, readBuffer, sizeof(readBuffer));
		rapidjson::Document jsonDocument;
		jsonDocument.ParseStream(jsonFileReaderStream);
		if (jsonDocument.IsObject())
		{
			rapidjson::Value& tracker = jsonDocument["tracker"];
			for (rapidjson::Value::MemberIterator trackerMemberIterator = 
					tracker.MemberBegin(); trackerMemberIterator != tracker.MemberEnd(); ++trackerMemberIterator)
			{
				if (trackerMemberIterator->name == "rotation")
				{
					ape::Degree angle;
					ape::Vector3 axis;
					for (rapidjson::Value::MemberIterator rotationMemberIterator =
						tracker[trackerMemberIterator->name].MemberBegin();
						rotationMemberIterator != tracker[trackerMemberIterator->name].MemberEnd(); ++rotationMemberIterator)
					{
						if (rotationMemberIterator->name == "angle")
							angle = rotationMemberIterator->value.GetFloat();
						else if (rotationMemberIterator->name == "x")
							axis.x = rotationMemberIterator->value.GetFloat();
						else if (rotationMemberIterator->name == "y")
							axis.y = rotationMemberIterator->value.GetFloat();
						else if (rotationMemberIterator->name == "z")
							axis.z = rotationMemberIterator->value.GetFloat();
					}
					mTrackerConfig.rotation.FromAngleAxis(ape::Radian(angle.toRadian()), axis);
				}
				else if (trackerMemberIterator->name == "translate")
				{
					for (rapidjson::Value::MemberIterator translateMemberIterator =
						tracker[trackerMemberIterator->name].MemberBegin();
						translateMemberIterator != tracker[trackerMemberIterator->name].MemberEnd(); ++translateMemberIterator)
					{
						if (translateMemberIterator->name == "x")
							mTrackerConfig.translate.x = translateMemberIterator->value.GetFloat();
						else if (translateMemberIterator->name == "y")
							mTrackerConfig.translate.y = translateMemberIterator->value.GetFloat();
						else if (translateMemberIterator->name == "z")
							mTrackerConfig.translate.z = translateMemberIterator->value.GetFloat();
					}
				}
				else if (trackerMemberIterator->name == "scale")
				{
					for (rapidjson::Value::MemberIterator scaleMemberIterator =
						tracker[trackerMemberIterator->name].MemberBegin();
						scaleMemberIterator != tracker[trackerMemberIterator->name].MemberEnd(); ++scaleMemberIterator)
					{
						if (scaleMemberIterator->name == "x")
							mTrackerConfig.scale.x = scaleMemberIterator->value.GetFloat();
						else if (scaleMemberIterator->name == "y")
							mTrackerConfig.scale.y = scaleMemberIterator->value.GetFloat();
						else if (scaleMemberIterator->name == "z")
							mTrackerConfig.scale.z = scaleMemberIterator->value.GetFloat();
					}
				}
				else if (trackerMemberIterator->name == "leftEyeOffset")
				{
					mTrackerConfig.leftEyeOffset = ape::Vector3(trackerMemberIterator->value.GetFloat(), 0, 0);
				}
				else if (trackerMemberIterator->name == "rightEyeOffset")
				{
					mTrackerConfig.rightEyeOffset = ape::Vector3(trackerMemberIterator->value.GetFloat(), 0, 0);
				}
			}
			rapidjson::Value& displays = jsonDocument["displays"];
			for (auto& display : displays.GetArray())
			{
				ape::HeadTrackerDisplayConfig fobHeadTrackingDisplayConfig;
				for (rapidjson::Value::MemberIterator displayMemberIterator = 
					display.MemberBegin(); displayMemberIterator != display.MemberEnd(); ++displayMemberIterator)
				{
					if (displayMemberIterator->name == "name")
					{
						fobHeadTrackingDisplayConfig.name = displayMemberIterator->value.GetString();
					}
					else if (displayMemberIterator->name == "cameras")
					{
						for (rapidjson::Value::MemberIterator camerasMemberIterator =
							display[displayMemberIterator->name].MemberBegin();
							camerasMemberIterator != display[displayMemberIterator->name].MemberEnd(); ++camerasMemberIterator)
						{
							if (camerasMemberIterator->name == "left")
							{
								fobHeadTrackingDisplayConfig.cameraLeftName = camerasMemberIterator->value.GetString();
								mCameraCount++;
							}
							else if (camerasMemberIterator->name == "right")
							{
								fobHeadTrackingDisplayConfig.cameraRightName = camerasMemberIterator->value.GetString();
								mCameraCount++;
							}
						}
					}
					else if (displayMemberIterator->name == "size")
					{
						for (rapidjson::Value::MemberIterator sizeMemberIterator = 
							display[displayMemberIterator->name].MemberBegin();
							sizeMemberIterator != display[displayMemberIterator->name].MemberEnd(); ++sizeMemberIterator)
						{
							if (sizeMemberIterator->name == "width")
								fobHeadTrackingDisplayConfig.size.x = sizeMemberIterator->value.GetFloat();
							else if (sizeMemberIterator->name == "height")
								fobHeadTrackingDisplayConfig.size.y = sizeMemberIterator->value.GetFloat();
						}
					}
					else if (displayMemberIterator->name == "position")
					{
						for (rapidjson::Value::MemberIterator positionMemberIterator =
							display[displayMemberIterator->name].MemberBegin();
							positionMemberIterator != display[displayMemberIterator->name].MemberEnd(); ++positionMemberIterator)
						{
							if (positionMemberIterator->name == "x")
								fobHeadTrackingDisplayConfig.position.x = positionMemberIterator->value.GetFloat();
							else if (positionMemberIterator->name == "y")
								fobHeadTrackingDisplayConfig.position.y = positionMemberIterator->value.GetFloat();
							else if (positionMemberIterator->name == "z")
								fobHeadTrackingDisplayConfig.position.z = positionMemberIterator->value.GetFloat();
						}
					}
					else if (displayMemberIterator->name == "orientation")
					{
					    ape::Degree angle;
						ape::Vector3 axis;
						for (rapidjson::Value::MemberIterator orientationMemberIterator =
							display[displayMemberIterator->name].MemberBegin();
							orientationMemberIterator != display[displayMemberIterator->name].MemberEnd(); ++orientationMemberIterator)
						{
							if (orientationMemberIterator->name == "angle")
								angle = orientationMemberIterator->value.GetFloat();
							else if (orientationMemberIterator->name == "x")
								axis.x = orientationMemberIterator->value.GetFloat();
							else if (orientationMemberIterator->name == "y")
								axis.y = orientationMemberIterator->value.GetFloat();
							else if (orientationMemberIterator->name == "z")
								axis.z = orientationMemberIterator->value.GetFloat();
						}
						fobHeadTrackingDisplayConfig.orientation.FromAngleAxis(ape::Radian(angle.toRadian()), axis);
					}
					else if (displayMemberIterator->name == "corners")
					{
						rapidjson::Value& corners = display[displayMemberIterator->name];
						for (auto& corner : corners.GetArray())
						{
							for (rapidjson::Value::MemberIterator cornerMemberIterator =
								corner.MemberBegin(); cornerMemberIterator != corner.MemberEnd(); ++cornerMemberIterator)
							{
								if (cornerMemberIterator->name == "bottomLeftCorner")
								{
									for (rapidjson::Value::MemberIterator memberIterator =
										corner[cornerMemberIterator->name].MemberBegin();
										memberIterator != corner[cornerMemberIterator->name].MemberEnd(); ++memberIterator)
									{
										if (memberIterator->name == "x")
											fobHeadTrackingDisplayConfig.bottomLeftCorner.x = memberIterator->value.GetFloat();
										else if (memberIterator->name == "y")
											fobHeadTrackingDisplayConfig.bottomLeftCorner.y = memberIterator->value.GetFloat();
										else if (memberIterator->name == "z")
											fobHeadTrackingDisplayConfig.bottomLeftCorner.z = memberIterator->value.GetFloat();
									}
								}
								else if (cornerMemberIterator->name == "bottomRightCorner")
								{
									for (rapidjson::Value::MemberIterator memberIterator =
										corner[cornerMemberIterator->name].MemberBegin();
										memberIterator != corner[cornerMemberIterator->name].MemberEnd(); ++memberIterator)
									{
										if (memberIterator->name == "x")
											fobHeadTrackingDisplayConfig.bottomRightCorner.x = memberIterator->value.GetFloat();
										else if (memberIterator->name == "y")
											fobHeadTrackingDisplayConfig.bottomRightCorner.y = memberIterator->value.GetFloat();
										else if (memberIterator->name == "z")
											fobHeadTrackingDisplayConfig.bottomRightCorner.z = memberIterator->value.GetFloat();
									}
								}
								else if (cornerMemberIterator->name == "topLeftCorner")
								{
									for (rapidjson::Value::MemberIterator memberIterator =
										corner[cornerMemberIterator->name].MemberBegin();
										memberIterator != corner[cornerMemberIterator->name].MemberEnd(); ++memberIterator)
									{
										if (memberIterator->name == "x")
											fobHeadTrackingDisplayConfig.topLeftCorner.x = memberIterator->value.GetFloat();
										else if (memberIterator->name == "y")
											fobHeadTrackingDisplayConfig.topLeftCorner.y = memberIterator->value.GetFloat();
										else if (memberIterator->name == "z")
											fobHeadTrackingDisplayConfig.topLeftCorner.z = memberIterator->value.GetFloat();
									}
								}
							}
						}
					}
					fobHeadTrackingDisplayConfig.width = fobHeadTrackingDisplayConfig.bottomRightCorner - fobHeadTrackingDisplayConfig.bottomLeftCorner;
					fobHeadTrackingDisplayConfig.width.normalise();
					fobHeadTrackingDisplayConfig.height = fobHeadTrackingDisplayConfig.topLeftCorner - fobHeadTrackingDisplayConfig.bottomLeftCorner;
					fobHeadTrackingDisplayConfig.height.normalise();
					fobHeadTrackingDisplayConfig.normal = fobHeadTrackingDisplayConfig.width.crossProduct(fobHeadTrackingDisplayConfig.height);
					fobHeadTrackingDisplayConfig.normal.normalise();

					fobHeadTrackingDisplayConfig.transform = ape::Matrix4(
						fobHeadTrackingDisplayConfig.width.x, fobHeadTrackingDisplayConfig.width.y, fobHeadTrackingDisplayConfig.width.z, 0,
						fobHeadTrackingDisplayConfig.height.x, fobHeadTrackingDisplayConfig.height.y, fobHeadTrackingDisplayConfig.height.z, 0,
						fobHeadTrackingDisplayConfig.normal.x, fobHeadTrackingDisplayConfig.normal.y, fobHeadTrackingDisplayConfig.normal.z, 0,
						0, 0, 0, 1);
				}
				APE_LOG_DEBUG("mDisplayConfigList.push_back(" << fobHeadTrackingDisplayConfig.name << ")");
				mDisplayConfigList.push_back(fobHeadTrackingDisplayConfig);
			}
		}
		fclose(apeFobHeadTrackingPluginConfigFile);
	}
	APE_LOG_DEBUG("mCameraCount: " << mCameraCount);
	APE_LOG_FUNC_LEAVE();
}

ape::Matrix4 ape::apeFobHeadTrackingPlugin::perspectiveOffCenter(float& displayDistanceLeft, float& displayDistanceRight, float& displayDistanceBottom, float& displayDistanceTop)
{
	float x = 2.0f * mNearClip / (displayDistanceRight - displayDistanceLeft);
	float y = 2.0f * mNearClip / (displayDistanceTop - displayDistanceBottom);
	float a = (displayDistanceRight + displayDistanceLeft) / (displayDistanceRight - displayDistanceLeft);
	float b = (displayDistanceTop + displayDistanceBottom) / (displayDistanceTop - displayDistanceBottom);
	ape::Matrix4 m(
		x, 0, a, 0,
		0, y, b, 0,
		0, 0, mC, mD,
		0, 0, -1, 0);
	return m;
}

ape::Matrix4 ape::apeFobHeadTrackingPlugin::calculateCameraProjection(ape::HeadTrackerDisplayConfig& displayConfig, ape::Vector3& trackedEyePosition)
{
	ape::Vector3 trackedViewerDistanceToDisplayBottomLeftCorner, trackedViewerDistanceToDisplayBottomRightCorner, trackedViewerDistanceToDisplayTopLeftCorner;

	float displayDistanceLeft, displayDistanceRight, displayDistanceBottom, displayDistanceTop, trackedViewerDistanceToDisplay;

	trackedViewerDistanceToDisplayBottomLeftCorner = displayConfig.bottomLeftCorner - trackedEyePosition;
	trackedViewerDistanceToDisplayBottomRightCorner = displayConfig.bottomRightCorner - trackedEyePosition;
	trackedViewerDistanceToDisplayTopLeftCorner = displayConfig.topLeftCorner - trackedEyePosition;

	trackedViewerDistanceToDisplay = -(trackedViewerDistanceToDisplayBottomLeftCorner.dotProduct(displayConfig.normal));

	displayDistanceLeft = (displayConfig.width.dotProduct(trackedViewerDistanceToDisplayBottomLeftCorner) * mNearClip) / trackedViewerDistanceToDisplay;
	displayDistanceRight = (displayConfig.width.dotProduct(trackedViewerDistanceToDisplayBottomRightCorner) * mNearClip) / trackedViewerDistanceToDisplay;
	displayDistanceBottom = (displayConfig.height.dotProduct(trackedViewerDistanceToDisplayBottomLeftCorner) * mNearClip) / trackedViewerDistanceToDisplay;
	displayDistanceTop = (displayConfig.height.dotProduct(trackedViewerDistanceToDisplayTopLeftCorner) * mNearClip) / trackedViewerDistanceToDisplay;

	ape::Matrix4 perspectiveOffCenterProjection = perspectiveOffCenter(displayDistanceLeft, displayDistanceRight, displayDistanceBottom, displayDistanceTop);

	ape::Matrix4 trackedViewerTranslate(
		1, 0, 0, -trackedEyePosition.x,
		0, 1, 0, -trackedEyePosition.y,
		0, 0, 1, -trackedEyePosition.z,
		0, 0, 0, 1);

	ape::Matrix4 cameraProjection = perspectiveOffCenterProjection * displayConfig.transform * trackedViewerTranslate;

	return cameraProjection;
}

void ape::apeFobHeadTrackingPlugin::Run()
{
	APE_LOG_FUNC_ENTER();
	int cameraCount = 0;
    while (cameraCount < mCameraCount)
	{
		mCameraDoubleQueue.swap();
		while (!mCameraDoubleQueue.emptyPop())
		{
			if (auto camera = mCameraDoubleQueue.front().lock())
			{
				APE_LOG_DEBUG("camera: " << camera->getName());
				setCameraConfigByName(camera->getName(), camera);
				mNearClip = camera->getNearClipDistance();
				mFarClip = camera->getFarClipDistance();
				mC = -(mFarClip + mNearClip) / (mFarClip - mNearClip);
				mD = -(2.0f * mFarClip * mNearClip) / (mFarClip - mNearClip);
				cameraCount++;
			}
			mCameraDoubleQueue.pop();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	while (mpFobTracker)
	{
		float positionDataFromTracker[3];
		float orientationDataFromTracker[3];
		if (trackdGetPosition(mpFobTracker, 0, positionDataFromTracker))
			mTrackedViewerPosition = (ape::Vector3(positionDataFromTracker[0], positionDataFromTracker[1], positionDataFromTracker[2]) * mTrackerConfig.scale) + mTrackerConfig.translate;
		if (trackdGetEulerAngles(mpFobTracker, 0, orientationDataFromTracker))
		{
			mTrackedViewerOrientationYPR = ape::Euler(ape::Degree(orientationDataFromTracker[1]).toRadian(), ape::Degree(orientationDataFromTracker[2]).toRadian() - 1.57f, ape::Degree(orientationDataFromTracker[0]).toRadian());
			mTrackedViewerOrientation = mTrackedViewerOrientationYPR.toQuaternion() * mTrackerConfig.rotation;
		}
		for (int i = 0; i < mDisplayConfigList.size(); i++)
		{
			auto displayConfig = mDisplayConfigList[i];
			if (auto cameraLeft = displayConfig.cameraLeft.lock())
			{
				ape::Vector3 trackedViewerLeftEyePosition = mTrackedViewerPosition + (mTrackedViewerOrientation * mTrackerConfig.leftEyeOffset);
				cameraLeft->setProjection(calculateCameraProjection(displayConfig, trackedViewerLeftEyePosition));
			}
			if (auto cameraRight = displayConfig.cameraRight.lock())
			{
				ape::Vector3 trackedViewerRightEyePosition = mTrackedViewerPosition + (mTrackedViewerOrientation * mTrackerConfig.rightEyeOffset);
				cameraRight->setProjection(calculateCameraProjection(displayConfig, trackedViewerRightEyePosition));
			}
		}
	}
	APE_LOG_FUNC_LEAVE();
}

void ape::apeFobHeadTrackingPlugin::Step()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_FUNC_LEAVE();
}

void ape::apeFobHeadTrackingPlugin::Stop()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_FUNC_LEAVE();
}

void ape::apeFobHeadTrackingPlugin::Suspend()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_FUNC_LEAVE();
}

void ape::apeFobHeadTrackingPlugin::Restart()
{
	APE_LOG_FUNC_ENTER();
	APE_LOG_FUNC_LEAVE();
}
