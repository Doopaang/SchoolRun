#include "AppDelegate.h"
#include "MainScene.h"
#include "AudioEngine.h"

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(1024, 576);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1152);

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("SchoolRun", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("SchoolRun");
#endif
        director->setOpenGLView(glview);
    }

    // 정보 출력 여부
    director->setDisplayStats(false);

    // FPS 설정
    director->setAnimationInterval(1.0 / 60);

    // 해상도 설정
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    auto frameSize = glview->getFrameSize();
    // 화면 높이가 보통 사이즈보다 클 때
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // 화면 높이가 작은 사이즈보다 클 때
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // 화면 높이가 보통 사이즈보다 작을 때
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
	
    register_all_packages();

    // 신 생성
    auto scene = MainScene::createScene();

    // 작동
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // 만약 오디오 엔진을 쓴다면 pause 배경음악 해야함.
	AudioEngine::pauseAll();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // 만약 오디오 엔진을 쓴다면 resume 배경음악 해야함.
	AudioEngine::resumeAll();
}
