#include "Tutorials.hpp"

class MyEventReceiver : public IEventReceiver
{
    public:
        virtual bool OnEvent(const SEvent& event)
        {
            if(event.EventType==irr::EET_KEY_INPUT_EVENT)
            {
                KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
                return false;
            }
        }

        virtual bool IsKeyDown(EKEY_CODE keyCode) const
        {
            return KeyIsDown[keyCode];
        }

        MyEventReceiver()
        {
            for(u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            {
                KeyIsDown[i] = false;
            }
        }


    private:
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

bool Tutorial4::Run()
{
    //create device
    MyEventReceiver receiver;

    IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(320,240), 16, false, false, false, &receiver);

    if(device == 0)
    {
        return false;
    }

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

    scene::ISceneNode* sphere = smgr->addSphereSceneNode();
    if(sphere)
    {
        sphere->setPosition(core::vector3df(0,0,30));
        sphere->setMaterialTexture(0,driver->getTexture("../irrlicht/irrlicht-1.7.3/media/wall.bmp"));
        sphere->setMaterialFlag(video::EMF_LIGHTING, false);
    }

    scene::ISceneNode* cube = smgr->addCubeSceneNode();
    if(cube)
    {
        cube->setMaterialTexture(0, driver->getTexture("../irrlicht/irrlicht-1.7.3/media/t351sml.jpg"));
        cube->setMaterialFlag(video::EMF_LIGHTING, false);
        scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(core::vector3df(0,0,30), 20.0f);
        if(anim)
        {
            cube->addAnimator(anim);
            anim->drop();
        }
    }

    scene::IAnimatedMeshSceneNode* anms = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../irrlicht/irrlicht-1.7.3/media/ninja.b3d"));
    if(anms)
    {
        scene::ISceneNodeAnimator* anim = smgr->createFlyStraightAnimator(core::vector3df(100,0,60), core::vector3df(-100,0,60), 3500, true);
        if(anim)
        {
            anms->addAnimator(anim);
            anim->drop();
        }

        anms->setMaterialFlag(video::EMF_LIGHTING, false);
        anms->setFrameLoop(0,13);
        anms->setAnimationSpeed(15);
//        anms->setMD2Animation(Scene::EMAT_RUN);


        anms->setScale(core::vector3df(2.f,2.f,2.f));
        anms->setRotation(core::vector3df(0,-90,0));
        anms->setMaterialTexture(0, driver->getTexture("../irrlicht/irrlicht-1.7.3/media/sydney.bmp"));
    }

    smgr->addCameraSceneNodeFPS();
    device->getCursorControl()->setVisible(false);

    device->getGUIEnvironment()->addImage(driver->getTexture("../irrlicht/irrlicht-1.7.3/media/irrlichtlogoalpha2.tga"), core::position2d<s32>(10,20));
    gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"",core::rect<s32>(10,10,400,20));
    diagnostics->setOverrideColor(video::SColor(255,255,255,0));

    //draw!
    int lastFPS = -1;

    u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = 5.f;

    while(device->run())
    {
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f;
        then = now;

        core::vector3df nodePosition = sphere->getPosition();
        if(receiver.IsKeyDown(irr::KEY_KEY_W))
        {
            nodePosition.Y+=MOVEMENT_SPEED * frameDeltaTime;
        }
        else if(receiver.IsKeyDown(irr::KEY_KEY_S))
        {
            nodePosition.Y-=MOVEMENT_SPEED * frameDeltaTime;
        }
        if(receiver.IsKeyDown(irr::KEY_KEY_A))
        {
            nodePosition.X-=MOVEMENT_SPEED * frameDeltaTime;
        }
        else if(receiver.IsKeyDown(irr::KEY_KEY_D))
        {
            nodePosition.X+=MOVEMENT_SPEED * frameDeltaTime;
        }

        sphere->setPosition(nodePosition);

        driver->beginScene(true, true, video::SColor(255,113,113,133));
        smgr->drawAll();
        device->getGUIEnvironment()->drawAll();
        driver->endScene();

        int fps = driver->getFPS();

        if(lastFPS!=fps)
        {
            core::stringw tmp(L"Movement Example [");
            tmp+=driver->getName();
            tmp+=L"] fps:";
            tmp+=fps;

            device->setWindowCaption(tmp.c_str());
            lastFPS= fps;
        }
    }

    device->drop();
    return true;

}
