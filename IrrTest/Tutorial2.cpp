#include "Tutorials.hpp"


bool Tutorial2::Run()
{
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<u32>(320,240), 16, false, false, false, 0);
    if(!device)
    {
        return 1;
    }

    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();

    device->getFileSystem()->addZipFileArchive("../irrlicht/irrlicht-1.7.3/media/map-20kdm2.pk3");

    scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
    scene::ISceneNode* node = 0;
    if(mesh)
    {
        node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
    }

    if(node)
    {
        node->setPosition(core::vector3df(-1300,-144,-1249));
    }

    smgr->addCameraSceneNodeFPS();

    device->getCursorControl()->setVisible(false);

    int lastFPS=-1;
    while(device->run())
    {
        if(device->isWindowActive())
        {
            driver->beginScene(true, true, video::SColor(255,200,200,200));
            smgr->drawAll();
            driver->endScene();

            int fps = driver->getFPS();
            if(lastFPS!=fps)
            {
                core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
                str+=driver->getName();
                str+="] FPS:";
                str+= fps;

                device->setWindowCaption(str.c_str());
                lastFPS=fps;
            }
        }
        else
        {
            device->yield();
        }
    }

    device->drop();
    return true;


}


