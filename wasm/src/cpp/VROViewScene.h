//
//  VROViewScene.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/4/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#include <memory>
#include "emscripten.h"
#include "emscripten/html5.h"

class VRORenderer;
class VROInputControllerWasm;
class VRODriverOpenGLWasm;

class VROViewScene {
public:
    VROViewScene();
    virtual ~VROViewScene();
    
    void drawFrame();
    void update();
    
    void buildTestScene();
    void onResize();
    void onBlur();
    void onFocus();
    
private:
    
    int _frame;
    int _width, _height;
    float _suspendedNotificationTime;
    
    std::shared_ptr<VRORenderer> _renderer;
    std::shared_ptr<VROInputControllerWasm> _inputController;
    std::shared_ptr<VRODriverOpenGLWasm> _driver;
    
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE _context;
    
};
