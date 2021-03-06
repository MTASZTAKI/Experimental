//#include "apePresentationScenePlugin.h"
//
//ape::apePresentationScenePlugin::apePresentationScenePlugin()
//{
//	APE_LOG_FUNC_ENTER();
//	mpKeyboard = NULL;
//	mpMouse = NULL;
//	mpCoreConfig = ape::ICoreConfig::getSingletonPtr();
//	mpEventManager = ape::IEventManager::getSingletonPtr();
//	mpEventManager->connectEvent(ape::Event::Group::NODE, std::bind(&apePresentationScenePlugin::eventCallBack, this, std::placeholders::_1));
//	mpSceneManager = ape::ISceneManager::getSingletonPtr();
//	mpMainWindow = ape::IMainWindow::getSingletonPtr();
//	mpEventManager->connectEvent(ape::Event::Group::CAMERA, std::bind(&apePresentationScenePlugin::eventCallBack, this, std::placeholders::_1));
//	mpEventManager->connectEvent(ape::Event::Group::GEOMETRY_RAY, std::bind(&apePresentationScenePlugin::eventCallBack, this, std::placeholders::_1));
//	mOldXMLFormatTranslateVector = ape::Vector3(26.809, 637.943, -22.163);
//	mOldXMLFormatTranslateVectorCamera = ape::Vector3(0, 637.943, 0);
//	mOldXMLFormatRotationQuaternion = ape::Quaternion();
//	ape::Radian angle(1.57f);
//	ape::Vector3 axis(0, 0, 1);
//	ape::Quaternion orientation;
//	orientation.FromAngleAxis(angle, axis);
//	angle = -1.57f;
//	axis = ape::Vector3(1, 0, 0);
//	ape::Quaternion orientation2;
//	orientation2.FromAngleAxis(angle, axis);
//	mOldXMLFormatRotationQuaternion = orientation * orientation2;
//	mKeyCodeMap = std::map<OIS::KeyCode, bool>();
//	mTranslateSpeedFactor = 2;
//	mRotateSpeedFactor = 1;
//	mCurrentStoryElementIndex = 0;
//	mStoryElements = std::vector<StoryElement>();
//	mBrowsers = std::map<std::string, ape::BrowserWeakPtr>();
//	mIsFirstSpacePressed = false;
//	mActiveMouseTexture = ape::UnitTextureWeakPtr();
//	mRayGeometry = ape::RayGeometryWeakPtr();
//	mRayOverlayNode = ape::NodeWeakPtr();
//	mGeometriesMouseTextures = std::map<std::string, ape::UnitTextureWeakPtr>();
//	mBrowserMouseTextures = std::map<std::string, ape::BrowserWeakPtr>();
//	mOverlayMouseTexture = ape::UnitTextureWeakPtr();
//	mOverlayMouseMaterial = ape::ManualMaterialWeakPtr();
//	mLastLeftClickTime = 0;
//	mActiveBrowser = ape::BrowserWeakPtr();
//	mOverlayBrowser = ape::BrowserWeakPtr();
//	mCamera = ape::CameraWeakPtr();
//	mUserNodePositionBeforeFullScreen = ape::Vector3();
//	mUserNodeOrientationBeforeFullScreen = ape::Quaternion();
//	mHumanHeight = 250.0f;
//	mHumanXoffset = 0.0f;
//	mHumanZoffset = 160.0f;
//	mContext = Context::UNKOWN;
//	APE_LOG_FUNC_LEAVE();
//}
//
//ape::apePresentationScenePlugin::~apePresentationScenePlugin()
//{
//	APE_LOG_FUNC_ENTER();
//	APE_LOG_FUNC_LEAVE();
//}
//
//void ape::apePresentationScenePlugin::eventCallBack(const ape::Event& event)
//{
//	if (event.type == ape::Event::Type::CAMERA_CREATE)
//		mCamera = std::static_pointer_cast<ape::ICamera>(mpSceneManager->getEntity(event.subjectName).lock());
//	else if (event.type == ape::Event::Type::GEOMETRY_RAY_INTERSECTION)
//	{
//		if (auto rayGeometry = mRayGeometry.lock())
//		{
//			auto intersections = rayGeometry->getIntersections();
//			for (auto intersection : intersections)
//			{
//				if (auto geometry = intersection.lock())
//				{
//					if (auto mouseTexture = mGeometriesMouseTextures[geometry->getName()].lock())
//					{
//						mActiveMouseTexture = mouseTexture;
//						if (auto overlayMouseMaterial = mOverlayMouseMaterial.lock())
//							overlayMouseMaterial->showOnOverlay(false, 1);
//						if (auto browser = mBrowserMouseTextures[mouseTexture->getName()].lock())
//							mActiveBrowser = browser;
//					}
//				}
//			}
//		}
//	}
//}
//
//void ape::apePresentationScenePlugin::Init()
//{
//	APE_LOG_FUNC_ENTER();
//
//	if (auto userNode = mpSceneManager->getNode(mpCoreConfig->getSceneNetworkConfig().generatedUniqueUserNodeName).lock())
//		mUserNode = userNode;
//
//	std::stringstream hwndStrStream;
//	hwndStrStream << mpMainWindow->getHandle();
//	std::stringstream windowHndStr;
//	windowHndStr << std::stoul(hwndStrStream.str(), nullptr, 16);
//
//	OIS::ParamList pl;
//	pl.insert(std::make_pair("WINDOW", windowHndStr.str()));
//#ifdef WIN32
//	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
//	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
//	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
//	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
//#endif
//	OIS::InputManager* inputManager = OIS::InputManager::createInputSystem(pl);
//	if (inputManager->getNumberOfDevices(OIS::OISKeyboard) > 0)
//	{
//		OIS::Keyboard* keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
//		mpKeyboard = keyboard;
//		mpKeyboard->setEventCallback(this);
//	}
//	if (inputManager->getNumberOfDevices(OIS::OISMouse) > 0)
//	{
//		OIS::Mouse* mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));
//		mpMouse = mouse;
//		mpMouse->setEventCallback(this);
//		const OIS::MouseState &ms = mouse->getMouseState();
//		ms.width = mpMainWindow->getWidth();
//		ms.height = mpMainWindow->getHeight();
//	}
//	/*rayGeomtery for rayquery*/
//	if (auto rayNode = mpSceneManager->createNode("rayNode").lock())
//	{
//		if (auto rayGeometry = std::static_pointer_cast<ape::IRayGeometry>(mpSceneManager->createEntity("rayQuery", ape::Entity::GEOMETRY_RAY).lock()))
//		{
//			rayGeometry->setIntersectingEnabled(true);
//			rayGeometry->setParentNode(rayNode);
//			mRayGeometry = rayGeometry;
//		}
//		rayNode->setParentNode(mUserNode);
//		mRayOverlayNode = rayNode;
//	}
//	/*Static things*/
//	std::string name = "metalroom";
//	ape::Vector3 position = ape::Vector3();
//	ape::Quaternion orientation = ape::Quaternion();
//	createMesh(name);
//	name = "ID10_monitor";
//	position = ape::Vector3(-100, 710, 87) - mOldXMLFormatTranslateVector;
//	orientation = ape::Quaternion(-0.981, -0, -0.195, -0);
//	createMesh(name, position, orientation);
//	name = "ID9_monitortalp";
//	position = ape::Vector3(-186, 633, 56) - mOldXMLFormatTranslateVector;
//	orientation = ape::Quaternion(-0.995, -0, -0.098, -0);
//	createMesh(name, position, orientation);
//	StoryElement storyElement;
//	storyElement.browserName = "periodicTable";
//	storyElement.browserURL = "http://sciencenotes.org/wp-content/uploads/2015/01/PeriodicTableMuted.png";
//	storyElement.browserPosition = ape::Vector3(442.29, 364.444, -543.149) - mOldXMLFormatTranslateVector;
//	storyElement.browserOrientation = ape::Quaternion(-0.924, -0, 0.383, -0) *  mOldXMLFormatRotationQuaternion;
//	storyElement.browserWidth = 278;
//	storyElement.browserHeight = 157;
//	storyElement.browserZoom = 0;
//	storyElement.browserResolutionVertical = 1440;
//	storyElement.browserResolutionHorizontal = 900;
//	manageBrowser(storyElement);
//	storyElement.browserName = "feyman";
//	storyElement.browserURL = "https://upload.wikimedia.org/wikipedia/en/4/42/Richard_Feynman_Nobel.jpg";
//	storyElement.browserPosition = ape::Vector3(-551.88, 364.444, 432.824) - mOldXMLFormatTranslateVector;
//	storyElement.browserOrientation = ape::Quaternion(-0.383, -0, -0.924, -0) *  mOldXMLFormatRotationQuaternion;
//	storyElement.browserWidth = 278;
//	storyElement.browserHeight = 157;
//	manageBrowser(storyElement);
//	storyElement.browserName = "universe";
//	storyElement.browserURL = "http://scienceblogs.com/startswithabang/files/2012/08/seqb_0631.jpeg";
//	storyElement.browserPosition = ape::Vector3(-697.103, 364.444, 71.468) - mOldXMLFormatTranslateVector;
//	storyElement.browserOrientation = ape::Quaternion(-0.707, -0, -0.707, -0) *  mOldXMLFormatRotationQuaternion;
//	storyElement.browserWidth = 278;
//	storyElement.browserHeight = 157;
//	manageBrowser(storyElement);
//	storyElement.browserName = "moseley";
//	storyElement.browserURL = "http://www.mhs.ox.ac.uk/moseley/wp-content/uploads/sites/5/2015/05/Moseley_in_his_prime_c1914.jpg";
//	storyElement.browserPosition = ape::Vector3(695.979, 364.444, 71.505) - mOldXMLFormatTranslateVector;
//	storyElement.browserOrientation = ape::Quaternion(-0.707, -0, 0.707, -0) *  mOldXMLFormatRotationQuaternion;
//	storyElement.browserWidth = 278;
//	storyElement.browserHeight = 157;
//	manageBrowser(storyElement);
//	/*apertusVR begin*/
//	/*static elements begin*/
//	//std::string name = "metalroom";
//	//ape::Vector3 position = ape::Vector3();
//	//ape::Quaternion orientation = ape::Quaternion();
//	//createMesh(name);
//	//name = "ID10_monitor";
//	//position = ape::Vector3(-100, 710, 87) - mOldXMLFormatTranslateVector;
//	//orientation = ape::Quaternion(-0.981, -0, -0.195, -0);
//	//createMesh(name, position, orientation);
//	//name = "ID9_monitortalp";
//	//position = ape::Vector3(-186, 633, 56) - mOldXMLFormatTranslateVector;
//	//orientation = ape::Quaternion(-0.995, -0, -0.098, -0);
//	//createMesh(name, position, orientation);
//	//StoryElement storyElement;
//	//storyElement.browserName = "periodicTable";
//	//storyElement.browserURL = "http://sciencenotes.org/wp-content/uploads/2015/01/PeriodicTableMuted.png";
//	//storyElement.browserPosition = ape::Vector3(442.29, 364.444, -543.149) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.924, -0, 0.383, -0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 278;
//	//storyElement.browserHeight = 157;
//	//storyElement.browserZoom = 0;
//	//storyElement.browserResolutionVertical = 1024;
//	//storyElement.browserResolutionHorizontal = 768;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "feyman";
//	//storyElement.browserURL = "https://upload.wikimedia.org/wikipedia/en/4/42/Richard_Feynman_Nobel.jpg";
//	//storyElement.browserPosition = ape::Vector3(-551.88, 364.444, 432.824) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.383, -0, -0.924, -0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 278;
//	//storyElement.browserHeight = 157;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "universe";
//	//storyElement.browserURL = "http://scienceblogs.com/startswithabang/files/2012/08/seqb_0631.jpeg";
//	//storyElement.browserPosition = ape::Vector3(-697.103, 364.444, 71.468) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.707, -0, -0.707, -0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 278;
//	//storyElement.browserHeight = 157;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "moseley";
//	//storyElement.browserURL = "http://www.mhs.ox.ac.uk/moseley/wp-content/uploads/sites/5/2015/05/Moseley_in_his_prime_c1914.jpg";
//	//storyElement.browserPosition = ape::Vector3(695.979, 364.444, 71.505) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.707, -0, 0.707, -0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 278;
//	//storyElement.browserHeight = 157;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "first";
//	//storyElement.browserURL = "http://google.com";
//	//storyElement.browserPosition = ape::Vector3(15, 367, 206) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(1, 0, 0, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 267;
//	//storyElement.browserHeight = 150;
//	//storyElement.browserResolutionVertical = 2048;
//	//storyElement.browserResolutionHorizontal = 1024;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "linkageDesigner";
//	//storyElement.browserURL = "http://www.linkagedesigner.com/";
//	//storyElement.browserPosition = ape::Vector3(9.504, 364.896, -423) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(0, -0, -1, -0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 143;
//	//storyElement.browserHeight = 85;
//	//storyElement.browserResolutionVertical = 1024;
//	//storyElement.browserResolutionHorizontal = 768;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "t-system";
//	//storyElement.browserURL = "https://www.t-systems.com/";
//	//storyElement.browserPosition = ape::Vector3(199.758, 677.017, -702.303) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.957, -0, 0.29, -0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 77;
//	//storyElement.browserHeight = 46;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "endo2";
//	//storyElement.browserURL = "http://srv.mvv.sztaki.hu/temp/endo2";
//	//storyElement.browserPosition = ape::Vector3(-74, 727, 726) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(0.098, 0, -0.995, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 77;
//	//storyElement.browserHeight = 46;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "karsai";
//	//storyElement.browserURL = "http://www.karsai.hu";
//	//storyElement.browserPosition = ape::Vector3(-706, 727, 51) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.831, 0, -0.556, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 77;
//	//storyElement.browserHeight = 46;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "motohotwear";
//	//storyElement.browserURL = "http://www.motohotwear.com/";
//	//storyElement.browserPosition = ape::Vector3(-706, 677, 51) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.831, 0, -0.556, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 77;
//	//storyElement.browserHeight = 46;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "endo5";
//	//storyElement.browserURL = "http://srv.mvv.sztaki.hu/temp/endo5";
//	//storyElement.browserPosition = ape::Vector3(188, 727, 708) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(0.195, 0, -0.981, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 77;
//	//storyElement.browserHeight = 46;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "endo";
//	//storyElement.browserURL = "https://www.endo-kapszula.hu/";
//	//storyElement.browserPosition = ape::Vector3(-201, 723, 692) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.195, 0, -0.981, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 143;
//	//storyElement.browserHeight = 85;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "endo1";
//	//storyElement.browserURL = "http://srv.mvv.sztaki.hu/temp/endo1";
//	//storyElement.browserPosition = ape::Vector3(62, 723, 729) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(0, 0, -1, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 143;
//	//storyElement.browserHeight = 85;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "endo3";
//	//storyElement.browserURL = "http://srv.mvv.sztaki.hu/temp/endo3";
//	//storyElement.browserPosition = ape::Vector3(-74, 677, 726) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(0.098, 0, -0.995, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 77;
//	//storyElement.browserHeight = 46;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "arUserManual";
//	//storyElement.browserURL = "https://www.youtube.com/embed/eVV5tUmky6c?vq=hd480&autoplay=1&loop=1&playlist=eVV5tUmky6c";
//	//storyElement.browserPosition = ape::Vector3(-688, 723, 184) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.556, 0, -0.831, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 143;
//	//storyElement.browserHeight = 85;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "endo4";
//	//storyElement.browserURL = "http://srv.mvv.sztaki.hu/temp/endo4";
//	//storyElement.browserPosition = ape::Vector3(188, 677, 708) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(0.195, 0, -0.981, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 77;
//	//storyElement.browserHeight = 46;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "ios-android";
//	//storyElement.browserURL = "https://nowitzkiblog.files.wordpress.com/2017/03/ios-android-icon_-e1447071162262.jpg?w=580";
//	//storyElement.browserPosition = ape::Vector3(199, 727, -702) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.957, 0, 0.29, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 77;
//	//storyElement.browserHeight = 46;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "bimGeometry";
//	//storyElement.browserURL = "http://www.teslacad.ca/blog/wp-content/uploads/2014/04/MEP-Services-Samples-13.jpg";
//	//storyElement.browserPosition = ape::Vector3(65, 723, -711) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.995, 0, -0.098, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 143;
//	//storyElement.browserHeight = 85;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "smartProfile";
//	//storyElement.browserURL = "http://kotem.com/Pages/Products.htm";
//	//storyElement.browserPosition = ape::Vector3(-544, 706, 440) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.383, 0, -0.924, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 278;
//	//storyElement.browserHeight = 157;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "qvii";
//	//storyElement.browserURL = "https://www.qvii.com/";
//	//storyElement.browserPosition = ape::Vector3(-392, 749, 592) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.383, 0, -0.924, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 139;
//	//storyElement.browserHeight = 78;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "kotem";
//	//storyElement.browserURL = "http://kotem.com/";
//	//storyElement.browserPosition = ape::Vector3(-392, 663, 592) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(-0.383, 0, -0.924, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 139;
//	//storyElement.browserHeight = 78;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "pridgeonandclay";
//	//storyElement.browserURL = "http://www.pridgeonandclay.com/";
//	//storyElement.browserPosition = ape::Vector3(542, 706, 441) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(0.383, 0, -0.924, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 278;
//	//storyElement.browserHeight = 157;
//	//manageBrowser(storyElement);
//	//storyElement.browserName = "arShopFloor";
//	//storyElement.browserURL = "http://thearea.org/wp-content/uploads/2017/03/Augmented-Reality-and-IoT.jpg";
//	//storyElement.browserPosition = ape::Vector3(392, 749, 592) - mOldXMLFormatTranslateVector;
//	//storyElement.browserOrientation = ape::Quaternion(0.383, 0, -0.924, 0) *  mOldXMLFormatRotationQuaternion;
//	//storyElement.browserWidth = 139;
//	//storyElement.browserHeight = 78;
//	//manageBrowser(storyElement);
//	///*story begin*/
//	//mStoryElements.push_back(StoryElement(ape::Vector3(0, 0, 0), ape::Quaternion(1, 0, 0, 0))); //zero
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-12.5, -270.943, 409.5), ape::Quaternion(1, 0, 0, 0))); //indigo
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-24.5, -273.943, -114), ape::Quaternion(1, 0, 0, 0),
//	//	"github", "https://github.com/MTASZTAKI/apertusVR", ape::Vector3(0, 360, -370) - mOldXMLFormatTranslateVector, ape::Quaternion(1, 0, 0, 0) * mOldXMLFormatRotationQuaternion, 240, 150)); //github
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-30.5, -247.443, -161), ape::Quaternion(1, 0, 0, 0),
//	//	"github", "https://github.com/MTASZTAKI/apertusVR#about", ape::Vector3(0, 360, -370) - mOldXMLFormatTranslateVector, ape::Quaternion(1, 0, 0, 0) * mOldXMLFormatRotationQuaternion, 240, 150)); //github#about
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-27.5, -278.943, -121), ape::Quaternion(1, 0, 0, 0),
//	//	"github", "https://github.com/MTASZTAKI/apertusVR#apertusvr", ape::Vector3(0, 360, -370) - mOldXMLFormatTranslateVector, ape::Quaternion(1, 0, 0, 0) * mOldXMLFormatRotationQuaternion, 240, 150)); //github#swarch
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-312.127, 79.2832, 317.68), ape::Quaternion(0.383774, -0.00326215, 0.923396, 0.00784904))); //smartProfile1
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-393.724, 78.6628, 287.429), ape::Quaternion(0.383774, -0.00326215, 0.923396, 0.00784904))); //smartProfile2
//	//mStoryElements.push_back(StoryElement(ape::Vector3(97.6123, 67.057, 422.253), ape::Quaternion(0.171713, -9.77448e-07, 0.985149, 0.0))); //endo
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-197.586, 100.834, 573.667), ape::Quaternion(0.116897, -0.00596791, 0.991839, 0.0506278),
//	//	"endo", "https://www.youtube.com/embed/MSvpU8bYZiE?vq=hd480&autoplay=1&loop=1&playlist=MSvpU8bYZiE", ape::Vector3(-201, 723, 692) - mOldXMLFormatTranslateVector, ape::Quaternion(-0.195, 0, -0.981, 0) *  mOldXMLFormatRotationQuaternion, 143, 85));
//	//mStoryElements.push_back(StoryElement(ape::Vector3(253.464, 88.8106, 280.662), ape::Quaternion(-0.397588, -1.02083e-08, 0.917568, 0.0))); //pridgeonandclay1
//	//mStoryElements.push_back(StoryElement(ape::Vector3(268.471, 112.444, 489.413), ape::Quaternion(-0.257203, -0.00218629, 0.966327, -0.00821397))); //pridgeonandclay2
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-539.024, 82.1165, 121.716), ape::Quaternion(0.653925, -0.0166771, 0.756182, 0.0192854))); //karsai1
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-590.853, 78.5688, 116.881), ape::Quaternion(0.808807, -0.0206272, 0.587596, 0.0149859))); //karsai2
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-607.718, 94.0923, 162.253), ape::Quaternion(0.573593, -0.0121893, 0.818928, 0.0174032))); //karsai3
//	//mStoryElements.push_back(StoryElement(ape::Vector3(16.7394, 84.057, -492.757), ape::Quaternion(0.981627, 0, -0.190809, 0))); //t-system
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-12, 387, -659) - mOldXMLFormatTranslateVectorCamera, ape::Quaternion(0, 0, -1, 0))); //linkageDesigner1
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-16.4233, -261.939, -547.675), ape::Quaternion(0.000344675, 0.0, -0.999992, -0.00424999))); //linkageDesigner2
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-16.4233, -261.939, -547.675), ape::Quaternion(0.000344675, 0.0, -0.999992, -0.00424999),
//	//	"linkageDesigner", "https://www.youtube.com/embed/YxcRr_QbPfg?vq=hd480&autoplay=1&loop=1&playlist=YxcRr_QbPfg", ape::Vector3(9.504, 364.896, -423) - mOldXMLFormatTranslateVector, ape::Quaternion(0, -0, -1, -0) *  mOldXMLFormatRotationQuaternion, 143, 85)); //linkageDesigner3
//	//mStoryElements.push_back(StoryElement(ape::Vector3(13, 359, -52) - mOldXMLFormatTranslateVectorCamera, ape::Quaternion(1, 0, 0, 0),
//	//	"symbio-tic", "http://www.symbio-tic.eu/index.php?option=com_content&view=article&id=2&Itemid=25", ape::Vector3(0, 360, -320) - mOldXMLFormatTranslateVector, ape::Quaternion(1, 0, 0, 0) * mOldXMLFormatRotationQuaternion, 240, 150, 0, 2048, 1024)); //symbio-tic1
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-25.9053, -304.943, -172.531), ape::Quaternion(1, 0, -0.000392581, 0))); //symbio-tic2
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-12.5, -270.943, 461.5), ape::Quaternion(1, 0, 0, 0),
//	//	"first", "https://techcrunch.com/2016/07/30/tech-trends-that-will-impact-your-home/", ape::Vector3(15, 367, 206) - mOldXMLFormatTranslateVector, ape::Quaternion(1, 0, 0, 0) * mOldXMLFormatRotationQuaternion, 267, 150, 0, 2048, 1024)); //thank you for the attention
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-12.5, -270.943, 461.5), ape::Quaternion(1, 0, 0, 0),
//	//	"first", "http://srv.mvv.sztaki.hu/temp/honeywell/tech_arvr.png", ape::Vector3(15, 367, 206) - mOldXMLFormatTranslateVector, ape::Quaternion(1, 0, 0, 0) * mOldXMLFormatRotationQuaternion, 267, 150, 0, 2048, 1024)); //thank you for the attention
//	//mStoryElements.push_back(StoryElement(ape::Vector3(-12.5, -270.943, 461.5), ape::Quaternion(1, 0, 0, 0),
//	//	"first", "http://srv.mvv.sztaki.hu/temp/honeywell/tech_iot.png", ape::Vector3(15, 367, 206) - mOldXMLFormatTranslateVector, ape::Quaternion(1, 0, 0, 0) * mOldXMLFormatRotationQuaternion, 267, 150, 0, 2048, 1024)); //thank you for the attention
//	///*apertusVR end*
//
//	storyElement.browserName = "partners";
//	storyElement.browserURL = "http://srv.mvv.sztaki.hu/temp/partners/collage.png";
//	storyElement.browserPosition = ape::Vector3(-201, 723, 692) - mOldXMLFormatTranslateVector;
//	storyElement.browserOrientation = ape::Quaternion(-0.195, 0, -0.981, 0) *  mOldXMLFormatRotationQuaternion;
//	storyElement.browserWidth = 143;
//	storyElement.browserHeight = 85;
//	manageBrowser(storyElement);
//	storyElement.browserName = "apertusvr";
//	storyElement.browserURL = "http://apertusvr.org";
//	storyElement.browserPosition = ape::Vector3(62, 723, 729) - mOldXMLFormatTranslateVector;
//	storyElement.browserOrientation = ape::Quaternion(0, 0, -1, 0) *  mOldXMLFormatRotationQuaternion;
//	storyElement.browserWidth = 143;
//	storyElement.browserHeight = 85;
//	manageBrowser(storyElement);
//	mStoryElements.push_back(StoryElement(ape::Vector3(0, 0, 0), ape::Quaternion(1, 0, 0, 0)));
//	mStoryElements.push_back(StoryElement(ape::Vector3(-24.5, -273.943, -114), ape::Quaternion(1, 0, 0, 0),
//		"github", "https://github.com/MTASZTAKI/apertusVR", ape::Vector3(0, 360, -370) - mOldXMLFormatTranslateVector,
//		ape::Quaternion(1, 0, 0, 0) * mOldXMLFormatRotationQuaternion, 240, 150));
//	mStoryElements.push_back(StoryElement(ape::Vector3(97.6123, 67.057, 422.253), ape::Quaternion(0.171713, -9.77448e-07, 0.985149, 0.0)));
//	mStoryElements.push_back(StoryElement(ape::Vector3(-81.5577, 88.1323, 618.38), ape::Quaternion(0.129627, -0.00440998, 0.990982, 0.0337064),
//		"Medical", "https://www.youtube.com/embed/MSvpU8bYZiE?vq=small&autoplay=1", ape::Vector3(-74, 727, 726) - mOldXMLFormatTranslateVector,
//		ape::Quaternion(0.098, 0, -0.995, 0) *  mOldXMLFormatRotationQuaternion, 77, 46));
//	mStoryElements.push_back(StoryElement(ape::Vector3(178.651, 71.7762, 623.509), ape::Quaternion(0.062044, 0.00158281, 0.997748, -0.0254543),
//		"Robotics&PointCloudStream", "https://www.youtube.com/embed/Tym-1bHug0A?vq=small&autoplay=1", ape::Vector3(188, 727, 708) - mOldXMLFormatTranslateVector,
//		ape::Quaternion(0.195, 0, -0.981, 0) *  mOldXMLFormatRotationQuaternion, 77, 46));
//	mStoryElements.push_back(StoryElement(ape::Vector3(-70.8102, 24.42, 603.963), ape::Quaternion(0.0787294, 0.00603394, 0.993966, -0.076172),
//		"HighPoly", "https://www.youtube.com/embed/gs2aGnJbn6Q?vq=small&autoplay=1", ape::Vector3(-74, 677, 726) - mOldXMLFormatTranslateVector,
//		ape::Quaternion(0.098, 0, -0.995, 0) *  mOldXMLFormatRotationQuaternion, 77, 46));
//	mStoryElements.push_back(StoryElement(ape::Vector3(146.501, 52.3024, 612.29), ape::Quaternion(-0.014453, 0.000242695, 0.999752, 0.0169786),
//		"IoT", "https://www.youtube.com/embed/SYDymKCu0jc?vq=small&autoplay=1", ape::Vector3(188, 677, 708) - mOldXMLFormatTranslateVector,
//		ape::Quaternion(0.195, 0, -0.981, 0) *  mOldXMLFormatRotationQuaternion, 77, 46));
//	mStoryElements.push_back(StoryElement(ape::Vector3(146.501, 52.3024, 612.29), ape::Quaternion(-0.014453, 0.000242695, 0.999752, 0.0169786),
//		"IoT", "https://www.youtube.com/embed/6UZf2Ex6l08?vq=small&autoplay=1", ape::Vector3(188, 677, 708) - mOldXMLFormatTranslateVector,
//		ape::Quaternion(0.195, 0, -0.981, 0) *  mOldXMLFormatRotationQuaternion, 77, 46));
//
//	//Overlay																																																																   //}	/*overlay begin*/
//	//if (auto browser = std::static_pointer_cast<ape::IBrowser>(mpSceneManager->createEntity("overlay_frame", ape::Entity::BROWSER).lock()))
//	//{
//	//	browser->setResoultion(2048, 1024);
//	//	browser->setURL("http://srv.mvv.sztaki.hu/temp/indigo/bg/index.html");
//	//	browser->showOnOverlay(true, 0);
//	//	mOverlayBrowser = browser;
//	//	mActiveBrowser = browser;
//	//	/*mouse begin*/
//	//	if (auto mouseMaterial = std::static_pointer_cast<ape::IManualMaterial>(mpSceneManager->createEntity("mouseMaterial", ape::Entity::MATERIAL_MANUAL).lock()))
//	//	{
//	//		mouseMaterial->setEmissiveColor(ape::Color(1.0f, 1.0f, 1.0f));
//	//		mouseMaterial->setSceneBlending(ape::Pass::SceneBlendingType::TRANSPARENT_ALPHA);
//	//		mouseMaterial->setLightingEnabled(false);
//	//		if (auto mouseTexture = std::static_pointer_cast<ape::IUnitTexture>(mpSceneManager->createEntity("mouseTexture", ape::Entity::TEXTURE_UNIT).lock()))
//	//		{
//	//			mouseTexture->setParameters(mouseMaterial, "browserpointer.png");
//	//			mouseTexture->setTextureAddressingMode(ape::Texture::AddressingMode::CLAMP);
//	//			mouseTexture->setTextureFiltering(ape::Texture::Filtering::POINT, ape::Texture::Filtering::LINEAR, ape::Texture::Filtering::F_NONE);
//	//			mActiveMouseTexture = mouseTexture;
//	//			mOverlayMouseTexture = mouseTexture;
//	//			mOverlayMouseMaterial = mouseMaterial;
//	//		}
//	//		mouseMaterial->showOnOverlay(true, 1);
//	//	}
//	//}
//}
//
//void ape::apePresentationScenePlugin::animateToStoryElements(ape::NodeSharedPtr userNode)
//{
//	StoryElement storyElement = mStoryElements[mCurrentStoryElementIndex];
//	if (auto userNode = mUserNode.lock())
//	{
//		auto moveInterpolator = std::make_unique<ape::Interpolator>(false);
//		moveInterpolator->addSection(
//			userNode->getPosition(),
//			storyElement.cameraPosition,
//			4.0,
//			[&](ape::Vector3 pos) { userNode->setPosition(pos); }
//		);
//		auto rotateInterpolator = std::make_unique<ape::Interpolator>(false);
//		rotateInterpolator->addSection(
//			userNode->getOrientation(),
//			storyElement.cameraOrientation,
//			4.0,
//			[&](ape::Quaternion ori) { userNode->setOrientation(ori); }
//		);
//		while (!moveInterpolator->isQueueEmpty() && !rotateInterpolator->isQueueEmpty())
//		{
//			if (!moveInterpolator->isQueueEmpty())
//				moveInterpolator->iterateTopSection();
//			if (!rotateInterpolator->isQueueEmpty())
//				rotateInterpolator->iterateTopSection();
//		}
//	}
//	if (storyElement.browserName.size())
//		manageBrowser(storyElement);
//}
//
//void ape::apePresentationScenePlugin::jumpToStoryElement(ape::NodeSharedPtr userNode)
//{
//	StoryElement storyElement = mStoryElements[mCurrentStoryElementIndex];
//	userNode->setPosition(storyElement.cameraPosition);
//	userNode->setOrientation(storyElement.cameraOrientation);
//	if (storyElement.browserName.size())
//		manageBrowser(storyElement);
//}
//
//void ape::apePresentationScenePlugin::manageBrowser(StoryElement storyElement)
//{
//	if (!mpSceneManager->getNode(storyElement.browserName).lock())
//	{
//		if (auto browserNode = mpSceneManager->createNode(storyElement.browserName).lock())
//		{
//			browserNode->setPosition(storyElement.browserPosition);
//			browserNode->setOrientation(storyElement.browserOrientation);
//			if (auto browserGeometry = std::static_pointer_cast<ape::IPlaneGeometry>(mpSceneManager->createEntity(storyElement.browserName + "geometry", ape::Entity::GEOMETRY_PLANE).lock()))
//			{
//				browserGeometry->setParameters(ape::Vector2(1, 1), ape::Vector2(storyElement.browserWidth, storyElement.browserHeight), ape::Vector2(1, 1));
//				browserGeometry->setParentNode(browserNode);
//				if (auto browser = std::static_pointer_cast<ape::IBrowser>(mpSceneManager->createEntity(storyElement.browserName + "browser", ape::Entity::BROWSER).lock()))
//				{
//					browser->setResoultion(storyElement.browserResolutionVertical, storyElement.browserResolutionHorizontal);
//					browser->setURL(storyElement.browserURL);
//					browser->setGeometry(browserGeometry);
//					mBrowsers[storyElement.browserName] = browser;
//					if (auto browserMouseNode = mpSceneManager->createNode(storyElement.browserName + "mouse").lock())
//					{
//						browserMouseNode->setParentNode(browserNode);
//						browserMouseNode->setPosition(ape::Vector3(0, -1, 0));
//						if (auto mouseGeometry = std::static_pointer_cast<ape::IPlaneGeometry>(mpSceneManager->createEntity(storyElement.browserName + "mouseGeometry", ape::Entity::GEOMETRY_PLANE).lock()))
//						{
//							mouseGeometry->setParameters(ape::Vector2(1, 1), ape::Vector2(storyElement.browserWidth, storyElement.browserHeight), ape::Vector2(1, 1));
//							mouseGeometry->setParentNode(browserMouseNode);
//							if (auto mouseMaterial = std::static_pointer_cast<ape::IManualMaterial>(mpSceneManager->createEntity(storyElement.browserName + "mouseMaterial", ape::Entity::MATERIAL_MANUAL).lock()))
//							{
//								mouseMaterial->setEmissiveColor(ape::Color(1.0f, 1.0f, 1.0f));
//								mouseMaterial->setSceneBlending(ape::Pass::SceneBlendingType::TRANSPARENT_ALPHA);
//								mouseMaterial->setLightingEnabled(false);
//								mouseMaterial->setCullingMode(ape::Material::CullingMode::NONE_CM);
//								if (auto mouseTexture = std::static_pointer_cast<ape::IUnitTexture>(mpSceneManager->createEntity(storyElement.browserName + "mouseTexture", ape::Entity::TEXTURE_UNIT).lock()))
//								{
//									mouseTexture->setParameters(mouseMaterial, "browserpointer.png");
//									mouseTexture->setTextureAddressingMode(ape::Texture::AddressingMode::CLAMP);
//									mouseTexture->setTextureFiltering(ape::Texture::Filtering::POINT, ape::Texture::Filtering::LINEAR, ape::Texture::Filtering::F_NONE);
//									mGeometriesMouseTextures[mouseGeometry->getName()] = mouseTexture;
//									mBrowserMouseTextures[mouseTexture->getName()] = browser;
//								}
//								std::static_pointer_cast<ape::IPlaneGeometry>(mouseGeometry)->setMaterial(mouseMaterial);
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	else
//	{
//		if (auto browser = std::static_pointer_cast<ape::IBrowser>(mpSceneManager->getEntity(storyElement.browserName + "browser").lock()))
//		{
//			browser->setURL(storyElement.browserURL);
//		}
//	}
//}
//
//void ape::apePresentationScenePlugin::createMesh(std::string name, ape::Vector3 position, ape::Quaternion orientation)
//{
//	if (auto node = mpSceneManager->createNode(name).lock())
//	{
//		node->setPosition(position);
//		node->setOrientation(orientation);
//		if (auto meshFile = std::static_pointer_cast<ape::IFileGeometry>(mpSceneManager->createEntity(name + ".mesh", ape::Entity::GEOMETRY_FILE).lock()))
//		{
//			meshFile->setFileName(name + ".mesh");
//			meshFile->setParentNode(node);
//		}
//	}
//}
//
//void ape::apePresentationScenePlugin::moveUserNode()
//{
//	if (mContext == SPACE)
//	{
//		auto userNode = mUserNode.lock();
//		if (userNode)
//		{
//			if (mKeyCodeMap[OIS::KeyCode::KC_PGUP])
//				userNode->translate(ape::Vector3(0, 1 * mTranslateSpeedFactor, 0), ape::Node::TransformationSpace::LOCAL);
//			if (mKeyCodeMap[OIS::KeyCode::KC_PGDOWN])
//				userNode->translate(ape::Vector3(0, -1 * mTranslateSpeedFactor, 0), ape::Node::TransformationSpace::LOCAL);
//			if (mKeyCodeMap[OIS::KeyCode::KC_D])
//				userNode->translate(ape::Vector3(1 * mTranslateSpeedFactor, 0, 0), ape::Node::TransformationSpace::LOCAL);
//			if (mKeyCodeMap[OIS::KeyCode::KC_A])
//				userNode->translate(ape::Vector3(-1 * mTranslateSpeedFactor, 0, 0), ape::Node::TransformationSpace::LOCAL);
//			if (mKeyCodeMap[OIS::KeyCode::KC_W])
//				userNode->translate(ape::Vector3(0, 0, -1 * mTranslateSpeedFactor), ape::Node::TransformationSpace::LOCAL);
//			if (mKeyCodeMap[OIS::KeyCode::KC_S])
//				userNode->translate(ape::Vector3(0, 0, 1 * mTranslateSpeedFactor), ape::Node::TransformationSpace::LOCAL);
//			if (mKeyCodeMap[OIS::KeyCode::KC_LEFT])
//				userNode->rotate(0.017f * mRotateSpeedFactor, ape::Vector3(0, 1, 0), ape::Node::TransformationSpace::WORLD);
//			if (mKeyCodeMap[OIS::KeyCode::KC_RIGHT])
//				userNode->rotate(-0.017f * mRotateSpeedFactor, ape::Vector3(0, 1, 0), ape::Node::TransformationSpace::WORLD);
//			if (mKeyCodeMap[OIS::KeyCode::KC_UP])
//				userNode->rotate(0.017f * mRotateSpeedFactor, ape::Vector3(1, 0, 0), ape::Node::TransformationSpace::LOCAL);
//			if (mKeyCodeMap[OIS::KeyCode::KC_DOWN])
//				userNode->rotate(-0.017f * mRotateSpeedFactor, ape::Vector3(1, 0, 0), ape::Node::TransformationSpace::LOCAL);
//			if (mKeyCodeMap[OIS::KeyCode::KC_NUMPAD4])
//				userNode->rotate(0.017f * mRotateSpeedFactor, ape::Vector3(0, 0, 1), ape::Node::TransformationSpace::WORLD);
//			if (mKeyCodeMap[OIS::KeyCode::KC_NUMPAD6])
//				userNode->rotate(-0.017f * mRotateSpeedFactor, ape::Vector3(0, 0, 1), ape::Node::TransformationSpace::WORLD);
//		}
//	}
//}
//
//void ape::apePresentationScenePlugin::saveUserNodePose(ape::NodeSharedPtr userNode)
//{
//	std::ofstream userNodePoseFile;
//	userNodePoseFile.open("userNodePoseFile.txt", std::ios::app);
//	userNodePoseFile << userNode->getPosition().x << "," << userNode->getPosition().y << "," << userNode->getPosition().z << " : " <<
//		userNode->getOrientation().w << "," << userNode->getOrientation().x << "," << userNode->getOrientation().y << "," << userNode->getOrientation().z << std::endl;
//	userNodePoseFile.close();
//}
//
//bool ape::apePresentationScenePlugin::keyPressed(const OIS::KeyEvent& e)
//{
//	if (auto activeBrowser = mActiveBrowser.lock())
//	{
//		mContext = Context::BROWSER;
//		std::string keyAsString = mpKeyboard->getAsString(e.key);
//		std::transform(keyAsString.begin(), keyAsString.end(), keyAsString.begin(), ::tolower);
//		//APE_LOG_DEBUG("keyAsString:" << keyAsString);
//		std::wstring keyAsWString(keyAsString.begin(), keyAsString.end());
//		if (e.key == OIS::KeyCode::KC_BACK)
//			keyAsWString = 8;
//		else if (e.key == OIS::KeyCode::KC_TAB)
//			keyAsWString = 9;
//		else if (e.key == OIS::KeyCode::KC_RETURN)
//			keyAsWString = 13;
//		else if (e.key == OIS::KeyCode::KC_LSHIFT)
//			keyAsWString = 14;
//		activeBrowser->keyASCIIValue(keyAsWString[0]);
//		auto userNode = mUserNode.lock();
//		if (userNode)
//		{
//			if (e.key == OIS::KeyCode::KC_ESCAPE)
//			{
//				mActiveMouseTexture = mOverlayMouseTexture;
//				mActiveBrowser = mOverlayBrowser;
//				if (auto overlayMouseMaterial = mOverlayMouseMaterial.lock())
//					overlayMouseMaterial->showOnOverlay(true, 1);
//				//userNode->setPosition(mUserNodePositionBeforeFullScreen);
//				//userNode->setOrientation(mUserNodeOrientationBeforeFullScreen);
//				mContext = Context::SPACE;
//			}
//		}
//	}
//	else
//	{
//		mKeyCodeMap[e.key] = true;
//		auto userNode = mUserNode.lock();
//		if (userNode)
//		{
//			if (mKeyCodeMap[OIS::KeyCode::KC_SPACE])
//			{
//				if (mCurrentStoryElementIndex < mStoryElements.size())
//				{
//					mCurrentStoryElementIndex++;
//					if (mCurrentStoryElementIndex == mStoryElements.size())
//						mCurrentStoryElementIndex = 0;
//					animateToStoryElements(userNode);
//				}
//				if (!mIsFirstSpacePressed)
//				{
//					/*setting up zoom levels*/
//					if (auto browserEndo = mBrowsers["endo"].lock())
//						browserEndo->setZoomLevel(4);
//					if (auto browserEndo = mBrowsers["smartProfile"].lock())
//						browserEndo->setZoomLevel(4);
//					if (auto browserEndo = mBrowsers["kotem"].lock())
//						browserEndo->setZoomLevel(2);
//					if (auto browserEndo = mBrowsers["qvii"].lock())
//						browserEndo->setZoomLevel(2);
//					mIsFirstSpacePressed = true;
//				}
//			}
//			if (mKeyCodeMap[OIS::KeyCode::KC_M])
//			{
//				if (mCurrentStoryElementIndex < mStoryElements.size())
//				{
//					mCurrentStoryElementIndex++;
//					if (mCurrentStoryElementIndex == mStoryElements.size())
//						mCurrentStoryElementIndex = 0;
//					jumpToStoryElement(userNode);
//				}
//			}
//			if (mKeyCodeMap[OIS::KeyCode::KC_N])
//			{
//				if (mCurrentStoryElementIndex > 0)
//				{
//					mCurrentStoryElementIndex--;
//					if (mCurrentStoryElementIndex < 0)
//						mCurrentStoryElementIndex = 0;
//					jumpToStoryElement(userNode);
//				}
//			}
//			if (mKeyCodeMap[OIS::KeyCode::KC_R])
//			{
//				mCurrentStoryElementIndex = 0;
//				jumpToStoryElement(userNode);
//			}
//			if (mKeyCodeMap[OIS::KeyCode::KC_MINUS])
//			{
//				mTranslateSpeedFactor -= 0.5;
//				mRotateSpeedFactor -= 0.5;
//			}
//			if (mKeyCodeMap[OIS::KeyCode::KC_ADD])
//			{
//				mTranslateSpeedFactor += 0.5;
//				mRotateSpeedFactor += 0.5;
//			}
//			if (mKeyCodeMap[OIS::KeyCode::KC_C])
//			{
//				saveUserNodePose(userNode);
//			}
//		}
//		mContext = Context::SPACE;
//	}
//	return true;
//}
//
//bool ape::apePresentationScenePlugin::keyReleased(const OIS::KeyEvent& e)
//{
//	mKeyCodeMap[e.key] = false;
//	return true;
//}
//
//bool ape::apePresentationScenePlugin::mouseMoved(const OIS::MouseEvent & e)
//{
//	if (auto activeMouseTexture = mActiveMouseTexture.lock())
//	{
//		if (auto activeBrowser = mActiveBrowser.lock())
//		{
//			ape::Vector2 cursorTexturePosition;
//			cursorTexturePosition.x = (float)-e.state.X.abs / (float)mpMainWindow->getWidth();
//			cursorTexturePosition.y = (float)-e.state.Y.abs / (float)mpMainWindow->getHeight();
//			activeMouseTexture->setTextureScroll(cursorTexturePosition.x, cursorTexturePosition.y);
//			ape::Vector2 cursorBrowserPosition;
//			cursorBrowserPosition.x = ((float)e.state.X.abs / (float)mpMainWindow->getWidth()) * activeBrowser->getResoultion().x;
//			cursorBrowserPosition.y = ((float)e.state.Y.abs / (float)mpMainWindow->getHeight()) * activeBrowser->getResoultion().y;
//			activeBrowser->mouseMoved(cursorBrowserPosition);
//			activeBrowser->mouseScroll(ape::Vector2(0, e.state.Z.rel));
//			//APE_LOG_DEBUG("cursorTexturePosition:" << cursorTexturePosition.x << ";" << cursorTexturePosition.y);
//			//APE_LOG_DEBUG("cursorBrowserPosition:" << cursorBrowserPosition.x << ";" << cursorBrowserPosition.y);
//		}
//	}
//	return true;
//}
//
//bool ape::apePresentationScenePlugin::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
//{
//	if (id == OIS::MouseButtonID::MB_Left)
//	{
//		if (auto activeBrowser = mActiveBrowser.lock())
//		{
//			activeBrowser->mouseClick(ape::Browser::MouseClick::LEFT, true);
//		}
//		else
//		{
//			if (auto rayOverlayNode = mRayOverlayNode.lock())
//			{
//				rayOverlayNode->setPosition(ape::Vector3(e.state.X.abs, e.state.Y.abs, 0));
//				if (auto rayGeomtery = mRayGeometry.lock())
//					rayGeomtery->fireIntersectionQuery();
//			}
//			clock_t currentLeftClickTime;
//			currentLeftClickTime = clock();
//			double elapsedTimInMSecs = (currentLeftClickTime - mLastLeftClickTime) / (CLOCKS_PER_SEC / 1000);
//			if (elapsedTimInMSecs < 250)
//			{
//				float planeWidth, planeHeight, windowHeight, windowWidth, planeRatio, windowRatio, cameraDistance = 0.0f;
//				if (auto activeBrowser = mActiveBrowser.lock())
//				{
//					if (auto geometry = std::static_pointer_cast<ape::IPlaneGeometry>(activeBrowser->getGeometry().lock()))
//					{
//						planeHeight = geometry->getParameters().size.x;
//						planeWidth = geometry->getParameters().size.y;
//						windowWidth = mpMainWindow->getWidth();
//						windowHeight = mpMainWindow->getHeight();
//						planeRatio = planeWidth / planeHeight;
//						windowRatio = windowWidth / windowHeight;
//						if (auto camera = mCamera.lock())
//						{
//							if (windowRatio < planeRatio)
//							{
//								float fieldOfViewX = 2 * atan(tan(camera->getFOVy().getRadian() * 0.5) * windowRatio);
//								cameraDistance = (planeWidth / 2) / (tan(fieldOfViewX / 2));
//							}
//							else
//								cameraDistance = (planeHeight / 2) / (tan(camera->getFOVy().getRadian() / 2));
//						}
//						if (auto userNode = mUserNode.lock())
//						{
//							if (auto geometryNode = geometry->getParentNode().lock())
//							{
//								ape::Vector3 position = geometryNode->getDerivedOrientation() * ape::Vector3(0, -cameraDistance, 0) + geometryNode->getDerivedPosition();
//								ape::Quaternion orientation = geometryNode->getDerivedOrientation();
//								mUserNodePositionBeforeFullScreen = userNode->getPosition();
//								mUserNodeOrientationBeforeFullScreen = userNode->getOrientation();
//								userNode->setPosition(position);
//								userNode->setOrientation(orientation * mOldXMLFormatRotationQuaternion.Inverse());
//							}
//						}
//					}
//				}
//			}
//			mLastLeftClickTime = currentLeftClickTime;
//		}
//	}
//	else if (id == OIS::MouseButtonID::MB_Right)
//	{
//		if (auto activeBrowser = mActiveBrowser.lock())
//			activeBrowser->mouseClick(ape::Browser::MouseClick::RIGHT, e.state.buttonDown(id));
//	}
//	else if (id == OIS::MouseButtonID::MB_Middle)
//	{
//		if (auto activeBrowser = mActiveBrowser.lock())
//			activeBrowser->mouseClick(ape::Browser::MouseClick::MIDDLE, e.state.buttonDown(id));
//	}
//	return true;
//}
//
//bool ape::apePresentationScenePlugin::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
//{
//	if (id == OIS::MouseButtonID::MB_Left)
//	{
//		if (auto activeBrowser = mActiveBrowser.lock())
//			activeBrowser->mouseClick(ape::Browser::MouseClick::LEFT, false);
//	}
//	return true;
//}
//
//void ape::apePresentationScenePlugin::Run()
//{
//	while (true)
//	{
//		if (mpKeyboard)
//			mpKeyboard->capture();
//		if (mpMouse)
//			mpMouse->capture();
//		moveUserNode();
//		std::this_thread::sleep_for(std::chrono::milliseconds(20));
//	}
//	mpEventManager->disconnectEvent(ape::Event::Group::NODE, std::bind(&apePresentationScenePlugin::eventCallBack, this, std::placeholders::_1));
//	mpEventManager->disconnectEvent(ape::Event::Group::CAMERA, std::bind(&apePresentationScenePlugin::eventCallBack, this, std::placeholders::_1));
//}
//
//void ape::apePresentationScenePlugin::Step()
//{
//	APE_LOG_FUNC_ENTER();
//	APE_LOG_FUNC_LEAVE();
//}
//
//void ape::apePresentationScenePlugin::Stop()
//{
//	APE_LOG_FUNC_ENTER();
//	APE_LOG_FUNC_LEAVE();
//}
//
//void ape::apePresentationScenePlugin::Suspend()
//{
//	APE_LOG_FUNC_ENTER();
//	APE_LOG_FUNC_LEAVE();
//}
//
//void ape::apePresentationScenePlugin::Restart()
//{
//	APE_LOG_FUNC_ENTER();
//	APE_LOG_FUNC_LEAVE();
//}
