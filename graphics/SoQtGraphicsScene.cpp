//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtGraphicsScene.h
//
// Note     :   This file is a derivative work of 'SoViewer' library available at
//              http://code.google.com/p/openinventorviewer/.
//
// Copyright (c) 2012, Vilas Chitrakaran
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================

#include "SoQtGraphicsScene.h"
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>

#include <Inventor/projectors/SbSphereSheetProjector.h>

namespace grape
{

//==============================================================================
SoSeparator* readIvFile(const QString& fileName, const QString& envVar)
//==============================================================================
{
    SoInput soInput;
    if( envVar.length() )
    {
        soInput.addEnvDirectoriesLast(envVar.toLatin1(), ";,\t ");
    }
    if (!soInput.openFile(fileName.toLatin1()))
    {
        return NULL;
    }
    SoSeparator* pNode = SoDB::readAll(&soInput);
    soInput.closeFile();
    return pNode;
}

//==============================================================================
SoQtGraphicsScene::SoQtGraphicsScene(QObject* pParent)
//==============================================================================
: SoQtRenderArea(pParent),
_pCamera(0),
_pSceneGraph(0),
_interactMode(VIEWING)
{
    SoQtg::init();

    _mouseButton = SoMouseButtonEvent::ANY;
    _pRootGraph = new SoSeparator;
    _pRootGraph->ref();

    _pAnnotations = new SoSeparator;
    _pAnnotations->ref();

    _pDirectionalLight = new SoDirectionalLight;
    _pDirectionalLight->ref();

    _cameraType = SoPerspectiveCamera::getClassTypeId();

    setInteractionMode(VIEWING);

    SoQtRenderArea::setSceneGraph(_pRootGraph);
    setViewClippingStrategy(SoQtGraphicsScene::AUTO);
}

//------------------------------------------------------------------------------
SoQtGraphicsScene::~SoQtGraphicsScene()
//------------------------------------------------------------------------------
{
    _pAnnotations->unref();
    _pDirectionalLight->unref();
    _pRootGraph->unref();
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::setSceneGraph(SoNode* pScene)
//------------------------------------------------------------------------------
{
    _pRootGraph->removeAllChildren();
    _pSceneGraph = NULL;

    if( !pScene )
    {
        return;
    }

    SoSearchAction sa;
    sa.setType(SoCamera::getClassTypeId());
    sa.setSearchingAll(false);
    sa.apply(pScene);

    bool myCamera = false;

    if( sa.getPath() ) // camera found in scene graph
    {
        _pCamera = (SoCamera*)((SoFullPath *)sa.getPath())->getTail();
        _cameraType = _pCamera->getTypeId();
    }
    else
    {
        myCamera = true;
        if( _cameraType == SoPerspectiveCamera::getClassTypeId() )
        {
            _pCamera = new SoPerspectiveCamera;
        }
        else
        {
            _pCamera = new SoOrthographicCamera;
        }
        _pRootGraph->addChild(_pCamera);
        SoQtRenderArea::setCamera(_pCamera);
    }
    _pRootGraph->addChild(_pDirectionalLight);
    _pRootGraph->addChild(_pAnnotations);
    _pRootGraph->addChild(pScene);
    if( myCamera )
    {
        viewAll();
    }
    saveHomePosition();
    _pSceneGraph = pScene;
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::addAnnotation(SoScreenAnnotationKit* pNodeKit)
//------------------------------------------------------------------------------
{
    _pAnnotations->addChild(pNodeKit);
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::removeAnnotation(const SbString& name)
//------------------------------------------------------------------------------
{
    SoNode* pNode = _pAnnotations->getByName(name);
    if( pNode )
    {
        _pAnnotations->removeChild(pNode);
    }
}

//------------------------------------------------------------------------------
SoNode* SoQtGraphicsScene::getAnnotation(const SbString& name) const
//------------------------------------------------------------------------------
{
    return _pAnnotations->getByName(name);
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::setCameraType(SoType type)
//------------------------------------------------------------------------------
{
    if( _cameraType == type )
    {
        return;
    }

    SoType perspectiveType = SoPerspectiveCamera::getClassTypeId();
    if( !type.isDerivedFrom( perspectiveType ) &&
        !type.isDerivedFrom( SoOrthographicCamera::getClassTypeId() ) )
    {
        return;
    }

    _cameraType = type;

    if( !_pCamera )
    {
        // no scenegraph active. Appropriate camera initialised after call
        // to setSceneGraph with a scene not containing a camera.
        return;
    }

    // replace camera in the scene
    SoCamera* pCamera = NULL;
    if( _cameraType == perspectiveType )
    {
        pCamera = new SoPerspectiveCamera;
    }
    else
    {
        pCamera = new SoOrthographicCamera;
    }
    setCamera(pCamera);
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::setCamera(SoCamera* pCamera)
//------------------------------------------------------------------------------
{
    if( !_pCamera || !pCamera || (_pCamera == pCamera) )
    {
        return;
    }

    SoQtGraphicsScene::CameraHome camHome;
    getCameraPosition(camHome._position, camHome._orientation, camHome._focalDistance);
    _pRootGraph->replaceChild(_pCamera, pCamera);
    _pCamera = pCamera;
    SoQtRenderArea::setCamera(_pCamera);
    setCameraPosition(camHome._position, camHome._orientation, camHome._focalDistance);
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::setCameraProperties(float focalDist, float vfov, float aspectRatio,
                                            SbBool cropViewToAspectRatio)
//------------------------------------------------------------------------------
{
    if(!_pCamera)
    {
        return;
    }
    _pCamera->aspectRatio.setValue(aspectRatio);
    _pCamera->focalDistance.setValue(focalDist);
    if( cropViewToAspectRatio )
    {
        _pCamera->viewportMapping.setValue(SoCamera::CROP_VIEWPORT_NO_FRAME);
    }
    else
    {
        _pCamera->viewportMapping.setValue(SoCamera::ADJUST_CAMERA);
    }

    if( _pCamera->getTypeId() == SoPerspectiveCamera::getClassTypeId() )
    {
        ((SoPerspectiveCamera*)_pCamera)->heightAngle.setValue(vfov);
    }
    else
    {
        ((SoOrthographicCamera*)_pCamera)->height.setValue(vfov);
    }
}

//------------------------------------------------------------------------------
SbBool SoQtGraphicsScene::getCameraProperties(float &focalDist, float &vfov, float &aspectRatio,
                                              SbBool &cropViewToAspectRatio)
//------------------------------------------------------------------------------
{
    if(!_pCamera)
    {
        return false;
    }

    focalDist = _pCamera->focalDistance.getValue();
    aspectRatio = _pCamera->aspectRatio.getValue();
    cropViewToAspectRatio = (_pCamera->viewportMapping.getValue() != SoCamera::ADJUST_CAMERA);

    if( _pCamera->getTypeId() == SoPerspectiveCamera::getClassTypeId() )
    {
        vfov = ((SoPerspectiveCamera*)_pCamera)->heightAngle.getValue();
    }
    else
    {
        vfov = ((SoOrthographicCamera*)_pCamera)->height.getValue();
    }

    return true;
}

//-----------------------------------------------------------------------------
void SoQtGraphicsScene::setCameraPosition(const SbVec3f &position, const SbRotation &orientation)
//-----------------------------------------------------------------------------
{
    if(!_pCamera)
    {
        return;
    }
    _pCamera->position.setValue(position);
    _pCamera->orientation.setValue(orientation);
}

//-----------------------------------------------------------------------------
void SoQtGraphicsScene::setCameraPosition(const SbVec3f &position, const SbRotation &orientation, float focalDist)
//-----------------------------------------------------------------------------
{
    if(!_pCamera)
    {
        return;
    }
    _pCamera->position.setValue(position);
    _pCamera->orientation.setValue(orientation);
    _pCamera->focalDistance.setValue(focalDist);
}

//-----------------------------------------------------------------------------
SbBool SoQtGraphicsScene::getCameraPosition(SbVec3f &position, SbRotation &orientation)
//-----------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return false;
    }
    position = _pCamera->position.getValue();
    orientation = _pCamera->orientation.getValue();
    return true;
}

//-----------------------------------------------------------------------------
SbBool SoQtGraphicsScene::getCameraPosition(SbVec3f &position, SbRotation &orientation, float &focalDist)
//-----------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return false;
    }
    position = _pCamera->position.getValue();
    orientation = _pCamera->orientation.getValue();
    focalDist = _pCamera->focalDistance.getValue();
    return true;
}

//-----------------------------------------------------------------------------
void SoQtGraphicsScene::showTopView()
//-----------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return;
    }
    _pCamera->orientation.setValue(SbVec3f(1.f, 0.f, 0.f), -M_PI/2.0f);
    viewAll();
}

//-----------------------------------------------------------------------------
void SoQtGraphicsScene::showFrontView()
//-----------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return;
    }
    _pCamera->orientation.setValue(SbVec3f(0.f, 1.f, 0.f), 0.f);
    viewAll();
}

//-----------------------------------------------------------------------------
void SoQtGraphicsScene::showRightSideView()
//-----------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return;
    }
    _pCamera->orientation.setValue(SbVec3f(0.f, 1.f, 0.f), M_PI/2.0f);
    viewAll();
}

//-----------------------------------------------------------------------------
void SoQtGraphicsScene::showLeftSideView()
//-----------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return;
    }
    _pCamera->orientation.setValue(SbVec3f(0.f, 1.f, 0.f), -M_PI/2.0f);
    viewAll();
}

//-----------------------------------------------------------------------------
void SoQtGraphicsScene::saveHomePosition()
//-----------------------------------------------------------------------------
{
    getCameraPosition(_cameraHome._position, _cameraHome._orientation, _cameraHome._focalDistance);
}

//-----------------------------------------------------------------------------
void SoQtGraphicsScene::resetToHomePosition()
//-----------------------------------------------------------------------------
{
    setCameraPosition( _cameraHome._position, _cameraHome._orientation, _cameraHome._focalDistance );
}

//-----------------------------------------------------------------------------
SbBool SoQtGraphicsScene::setViewClipping(float near, float far)
//-----------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return false;
    }
    _pCamera->farDistance.setValue(far);
    _pCamera->nearDistance.setValue(near);
    return true;
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::viewAll()
//------------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return;
    }

    _pCamera->viewAll( _pRootGraph, getViewportRegion() );
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::setInteractionMode(InteractionMode mode)
//------------------------------------------------------------------------------
{
    _interactMode = mode;

    switch(mode)
    {
        case VIEWING:
        {
            //TODO
            //setCursor((m_mouseButton == SoMouseButtonEvent::ANY) ? Qt::OpenHandCursor : Qt::ClosedHandCursor);

            // Turn off the selection indicators when we go back from picking
            // mode into viewing mode.
            SoGLRenderAction* pAction = this->getGLRenderAction();
            if (pAction != NULL)
            {
                SoLocateHighlight::turnOffCurrentHighlight(pAction);
            }
        }
        break;

        default:
        {
            //TODO
            //setCursor(Qt::ArrowCursor);

        }
        break;
    };
}

//------------------------------------------------------------------------------
SbVec3f SoQtGraphicsScene::projectPoint(const SbVec2f & pt) const
//------------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return SbVec3f(0.0f, 0.0f, 0.0f);
    }

    // Avoid problems when width < height.
    SbViewVolume cameraVolume;
    if( _pCamera->viewportMapping.getValue() == SoCamera::ADJUST_CAMERA )
    {
        float aspectRatio = (height() == 0) ? 1.0f : (width() / (float)height());
        cameraVolume = _pCamera->getViewVolume(aspectRatio);

        if (aspectRatio < 1.0) cameraVolume.scale(1.0f / aspectRatio);
    }
    else
    {
        cameraVolume = _pCamera->getViewVolume(0.0f);
    }

    // Map screen location into the camera focal plane
    return cameraVolume.getPlanePoint(_pCamera->focalDistance.getValue(), pt);
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::rotateCamera(const SbRotation &rot)
//------------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return;
    }

    // get center of rotation
    float radius = _pCamera->focalDistance.getValue();

    SbVec3f forward;
    _pCamera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);

    SbVec3f center = _pCamera->position.getValue() + radius * forward;

    // apply new rotation to the camera
    _pCamera->orientation = rot * _pCamera->orientation.getValue();

    // reposition camera to look at pt of interest
    _pCamera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);
    _pCamera->position = center - radius * forward;
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::adjustCameraClippingPlanes()
//------------------------------------------------------------------------------
{
    if( _viewClipStrategy == SoQtGraphicsScene::FIXED )
    {
        return;
    }

    if ( !_pCamera )
    {
        return;
    }

    SoGetBoundingBoxAction clipbox_action(getViewportRegion());
    clipbox_action.apply(_pRootGraph);

    SbBox3f bbox = clipbox_action.getBoundingBox();

    if (bbox.isEmpty())
    {
        return;
    }

    SbSphere bSphere;
    bSphere.circumscribe(bbox);

    SbVec3f forward;
    _pCamera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);

    float denominator = forward.length();
    float numerator = (bSphere.getCenter() - _pCamera->position.getValue()).dot(forward);
    float distToCenter = (forward * (numerator / denominator)).length();

    float farplane = distToCenter + bSphere.getRadius();

    // if scene is behind the camera, don't change the planes
    if (farplane < 0)
    {
        return;
    }

    float nearplane = distToCenter - bSphere.getRadius();

    if ( nearplane < (0.001 * farplane) )
    {
        nearplane = 0.001 * farplane;
    }

    if( _viewClipStrategy != SoQtGraphicsScene::FIXED_NEAR )
    {
        _pCamera->nearDistance = nearplane;
    }

    if( _viewClipStrategy != SoQtGraphicsScene::FIXED_FAR )
    {
        _pCamera->farDistance = farplane;
    }
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::soPaintEvent()
//------------------------------------------------------------------------------
{
    if( !_pCamera )
    {
        return;
    }

    SbVec3f forward;
    _pCamera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);
    _pDirectionalLight->direction = forward;
    adjustCameraClippingPlanes();
    SoQtRenderArea::soPaintEvent();
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::soKeyPressEvent( SoKeyboardEvent* pEvent)
//------------------------------------------------------------------------------
{
    switch( _interactMode )
    {
    case VIEWING:
        break;
    case SCENE_GRAPH:
        SoQtRenderArea::soKeyPressEvent( pEvent );
        break;
    default:
        break;
    };
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::soKeyReleaseEvent( SoKeyboardEvent* pEvent)
//------------------------------------------------------------------------------
{
    switch( _interactMode )
    {
    case VIEWING:
        break;
    case SCENE_GRAPH:
        SoQtRenderArea::soKeyReleaseEvent( pEvent );
        break;
    default:
        break;
    };
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::soMousePressEvent( SoMouseButtonEvent* pEvent )
//------------------------------------------------------------------------------
{
    _mouseButton = pEvent->getButton();

    switch( _interactMode )
    {
    case VIEWING:
        _startDrag = pEvent->getNormalizedPosition(getViewportRegion());
        break;
    case SCENE_GRAPH:
        SoQtRenderArea::soMousePressEvent(pEvent);
        break;
    default:
        break;
    };
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::soMouseMoveEvent( SoLocation2Event* pEvent )
//------------------------------------------------------------------------------
{
    switch( _interactMode )
    {

    case VIEWING:
        {
            if ( !_pCamera )
            {
                break;
            }

            SbVec2f endDrag = pEvent->getNormalizedPosition(getViewportRegion());

            if( _mouseButton == SoMouseButtonEvent::BUTTON1 )
            {
                // Init the sphere projector class
                SbViewVolume vv;
                vv.ortho(-1, 1, -1, 1, -10, 10);
                SbSphereSheetProjector sphereSheet;
                sphereSheet.setViewVolume( vv );
                sphereSheet.setSphere( SbSphere( SbVec3f(0, 0, 0), .7f) );

                // Set the sphere sheet starting point
                SbVec3f lstart = sphereSheet.project(_startDrag);
                SbVec3f lend = sphereSheet.project(endDrag);

                rotateCamera( sphereSheet.getRotation(lstart, lend).inverse() );
            }

            if( _mouseButton == SoMouseButtonEvent::BUTTON2 )
            {
                SbVec3f lstart = projectPoint(_startDrag);
                SbVec3f lend = projectPoint(endDrag);

                // move the camera by the delta 3D position amount
                _pCamera->position = _pCamera->position.getValue() +  (lstart - lend);
            }

            _startDrag = endDrag;
        }
        break;


    case SCENE_GRAPH:
        SoQtRenderArea::soMouseMoveEvent( pEvent );
        break;


    default:
        break;
    };


}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::soMouseReleaseEvent( SoMouseButtonEvent* pEvent )
//------------------------------------------------------------------------------
{
    _mouseButton = SoMouseButtonEvent::ANY;
    switch( _interactMode )
    {
    case VIEWING:
        _startDrag = pEvent->getNormalizedPosition(getViewportRegion());
        break;
    case SCENE_GRAPH:
        SoQtRenderArea::soMouseReleaseEvent(pEvent);
        break;
    default:
        break;
    };
}

//------------------------------------------------------------------------------
void SoQtGraphicsScene::soWheelEvent( SoWheelEvent* pEvent )
//------------------------------------------------------------------------------
{
    switch( _interactMode )
    {
    case VIEWING:
        {
            if ( !_pCamera )
            {
                break;
            }
            float dol = pow(2.0f, -pEvent->getDelta() / 4000.0f);

            if ( _pCamera->isOfType(SoOrthographicCamera::getClassTypeId()) )
            {
                // change the ortho camera height
                SoOrthographicCamera* pCamera = (SoOrthographicCamera *) _pCamera;
                pCamera->height = pCamera->height.getValue() * dol; // on this camera type, dolly changes zoom
            }
            //else
            //{
                // shorter/grow the focal distance given the mouse move
                float focalDistance = _pCamera->focalDistance.getValue();;
                float newFocalDist = focalDistance * dol;

                // finally reposition the camera
                SbVec3f forward;
                _pCamera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);
                _pCamera->position = _pCamera->position.getValue() + (focalDistance - newFocalDist) * forward;
                _pCamera->focalDistance = newFocalDist;
            //}
        }
        break;

    case SCENE_GRAPH:
        SoQtRenderArea::soWheelEvent( pEvent );
        break;

    default:
        break;
    };
}

} // grape
