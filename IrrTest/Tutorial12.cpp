#include "Tutorials.hpp"

class MyEventReceiver12 : public IEventReceiver
{
    public:
        MyEventReceiver12(scene::ISceneNode* terrain, scene::ISceneNode* skybox, scene::ISceneNode* skydome) : Terrain(terrain), Skybox(skybox), Skydome(skydome), showBox(true)
        {
            Skybox->setVisible(true);
            Skydome->setVisible(false);
        }

        bool OnEvent(const SEvent& event)
        {
            if(event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
            {
                switch(event.KeyInput.Key)
                {
                    case irr::KEY_KEY_W:
                        Terrain->setMaterialFlag(video::EMF_WIREFRAME, !Terrain->getMaterial(0).Wireframe);
                        Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
                        return true;
                    case irr::KEY_KEY_P:
                        Terrain->setMaterialFlag(video::EMF_POINTCLOUD,!Terrain->getMaterial(0).PointCloud);
                        Terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
                    case irr::KEY_KEY_D:
                        Terrain->setMaterialType(Terrain->getMaterial(0).MaterialType==video::EMT_SOLID ? video::EMT_DETAIL_MAP : video::EMT_SOLID);
                        return true;
                    case irr::KEY_KEY_S:
                        showBox=!showBox;
                        Skybox->setVisible(showBox);
                        Skydome->setVisible(!showBox);
                        return true;
                    default:
                        break;
                }
            }
            return false;
        }


    private:
        scene::ISceneNode* Terrain;
        scene::ISceneNode* Skybox;
        scene::ISceneNode* Skydome;
        bool showBox;
};


bool Tutorial12::Run()
{
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<u32>(180,120), 16, false, false, false, 0);

    if(device==0)
    {
        return false;
    }

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* env = device->getGUIEnvironment();

    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    env->addImage(driver->getTexture("../irrlicht/irrlicht-1.7.3/media/irrlichtlogo2.png"), core::position2d<s32>(10,10));
    env->getSkin()->setFont(env->getFont("../irrlicht/irrlicht-1.7.3/media/fontlucida.png"));
    scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0,100.0f, 1.2f);

    camera->setPosition(core::vector3df(2700*2, 255*2, 2600*2));
    camera->setTarget(core::vector3df(2397*2, 343*2, 2700*2));
    camera->setFarValue(42000.0f);

    device->getCursorControl()->setVisible(false);

    scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("../irrlicht/irrlicht-1.7.3/media/terrain-heightmap.bmp",
                                                                    0,                                      // parent node                                      // parent node
                                                                    -1,                                     // node id
                                                                    core::vector3df(0.f, 0.f, 0.f),         // position
                                                                    core::vector3df(0.f, 0.f, 0.f),         // rotation
                                                                    core::vector3df(40.f, 4.4f, 40.f),      // scale
                                                                    video::SColor ( 255, 255, 255, 255 ),   // vertexColor
                                                                    5,                                      // maxLOD
                                                                    scene::ETPS_17,                         // patchSize
                                                                    4);                                       // smoothFactor

    terrain->setMaterialFlag(video::EMF_LIGHTING, false);
    terrain->setMaterialTexture(0, driver->getTexture("../irrlicht/irrlicht-1.7.3/media/terrain-texture.jpg"));
    terrain->setMaterialTexture(1, driver->getTexture("../irrlicht/irrlicht-1.7.3/media/detailmap3.jpg"));
    terrain->setMaterialType(video::EMT_DETAIL_MAP);
    terrain->scaleTexture(1.0f, 20.0f);

    //collision
    scene::ITriangleSelector* selector = smgr->createTerrainTriangleSelector(terrain,0);
    terrain->setTriangleSelector(selector);

    scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, core::vector3df(60,100,60), core::vector3df(0,0,0), core::vector3df(0,50,0));
    selector->drop();
    camera->addAnimator(anim);
    anim->drop();


    //add collision responder to camera, i.e. gravity!
    anim = smgr->createCollisionResponseAnimator(
                        selector, camera, core::vector3df(10,20,10),
                        core::vector3df(0,-5,0), core::vector3df(0,30,0));
                selector->drop(); // As soon as we're done with the selector, drop it.
                camera->addAnimator(anim);
                anim->drop();  // And likewise, drop the animator when we're done referring to it.

    //to access terrain data, do this!
    scene::CDynamicMeshBuffer* buffer = new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
    terrain->getMeshBufferForLOD(*buffer, 0);
    video::S3DVertex2TCoords* data = (video::S3DVertex2TCoords*)buffer->getVertexBuffer().getData();
    //work on data or get the IndexBuffer with a similar call
    buffer->drop();


    //create skybox and skydome
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
        driver->getTexture("../irrlicht/irrlicht-1.7.3/media/irrlicht2_up.jpg"),
        driver->getTexture("../irrlicht/irrlicht-1.7.3/media/irrlicht2_dn.jpg"),
        driver->getTexture("../irrlicht/irrlicht-1.7.3/media/irrlicht2_lf.jpg"),
        driver->getTexture("../irrlicht/irrlicht-1.7.3/media/irrlicht2_rt.jpg"),
        driver->getTexture("../irrlicht/irrlicht-1.7.3/media/irrlicht2_ft.jpg"),
        driver->getTexture("../irrlicht/irrlicht-1.7.3/media/irrlicht2_bk.jpg"));
    scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../irrlicht/irrlicht-1.7.3/media/skydome.jpg"),16,8,0.95f, 2.0f);

    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    //event receiver
    MyEventReceiver12 receiver(terrain, skybox, skydome);
    device->setEventReceiver(&receiver);

    int lastFPS = -1;

    while(device->run())
    if (device->isWindowActive())
    {
            driver->beginScene(true, true, 0 );

            smgr->drawAll();
            env->drawAll();

            driver->endScene();

            // display frames per second in window title
            int fps = driver->getFPS();
            if (lastFPS != fps)
            {
                    core::stringw str = L"Terrain Renderer - Irrlicht Engine [";
                    str += driver->getName();
                    str += "] FPS:";
                    str += fps;
                    // Also print terrain height of current camera position
                    // We can use camera position because terrain is located at coordinate origin
                    str += " Height: ";
                    str += terrain->getHeight(camera->getAbsolutePosition().X,
                                    camera->getAbsolutePosition().Z);

                    device->setWindowCaption(str.c_str());
                    lastFPS = fps;
            }
    }

    device->drop();

    return false;
}
