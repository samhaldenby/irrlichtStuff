#include "Tutorials.hpp"


bool Tutorial1::Run()
{
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<u32>(320,240), 16, false, false, false, 0);
    if(!device)
    {
        return 1;
    }

    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", rect<s32>(10,10,2600,22), true);

    IAnimatedMesh* mesh = smgr->getMesh("../irrlicht/irrlicht-1.7.3/media/sydney.md2");
    if(!mesh)
    {
        device->drop();
        return 1;
    }
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

    if(node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture(0, driver->getTexture("../irrlicht/irrlicht-1.7.3/media/sydney.bmp"));
    }

    smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    while(device->run())
    {
        driver->beginScene(true, true, SColor(255, 100, 101, 140));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
    }

    device->drop();
}


