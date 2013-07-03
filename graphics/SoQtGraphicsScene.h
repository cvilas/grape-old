//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtGraphicsScene.h
// Brief    : Open-Inventor scenegraph rendering surface with default camera and lights
//
// Note     :   This file is a derivative work of 'SoViewer' library available at
//              http://code.google.com/p/openinventorviewer/. A BSD-style license
//              applies. See License.txt
//==============================================================================

#ifndef GRAPE_SOQTGRAPHICSSCENE_H
#define GRAPE_SOQTGRAPHICSSCENE_H

#include "SoQtRenderArea.h"
#include "SoScreenAnnotationKit.h"
#include <QString>
#include <Inventor/SbRotation.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>

namespace Grape
{

/// Read an OpenInventor iv file and return a scene graph.
/// \param fileName (input) Name of file to read.
/// \param envVar	 (input) Environment variable containing additional search path for the file.
/// \return		 SceneGraph root node, NULL in case of error opening or reading file.
GRAPEGRAPHICS_DLL_API SoSeparator* readIvFile(const QString& fileName, const QString& envVar);

/// \class SoQtGraphicsScene
/// \ingroup graphics
/// \brief Open-Inventor scenegraph rendering surface with default camera and
///		   headlights.
///
/// SoQtGraphicsScene extends SoQtRenderArea with methods for camera handling,
/// automatic headlight configuration and 2D annotation.
/// This class has no visual appearence (hence, it is not a 'viewer'). To view
/// the scene, application programmers must use the SoSceneViewer class.
///
/// This class enables the following mouse interactions with the scene:
/// - Hold left button and drag for orientation change
/// - Hold middle button and drag for position change
/// - Thumbwheel for zoom
///
/// \todo
/// - Reimplement setStereoMode() and enable/disable stereo buffers in QGLWidget
/// - setCursor in setViewing(SbBool state)
/// - Scenegraph interaction capabilities, such as with draggers and manipulators.
/// - BUG: Directional light direction doesn't work when the scenegraph defines a camera.
/// - Test for memory leaks
///
/// Example program:
/// See examples in /examples/Inventor
class GRAPEGRAPHICS_DLL_API SoQtGraphicsScene : public SoQtRenderArea
{
    Q_OBJECT
public:
    /// The constructor initializes the Open-Inventor library.
    explicit SoQtGraphicsScene(QObject* pParent = 0);

    /// cleans up resources on destruction.
    virtual ~SoQtGraphicsScene();


    //-------- Scene-graph methods ---------

    /// Sets the scene graph to be rendered. This class will add
    /// a directional light, and will add a camera if one is not
    /// found in the scenegraph. In multithreaded applications,
    /// use SoDB::writeLock() and associated methods to ensure that
    /// scenegraph operations do not happen during the time it is being
    /// modified.
    /// \param pScene (input) Pointer to open-inventor scene graph.
    virtual void setSceneGraph(SoNode* pScene);

    /// \return Gets the scene graph to be rendered in this component's window, excluding
    ///		 any scenegraph elements added by this class (directional light, camera).
    virtual SoNode* getSceneGraph() const { return _pSceneGraph; }

    //-------- Screen annotation methods ---------

    /// Add an annotation to the scenegraph display screen. To be
    /// able to reference the annotation by name in other methods such as
    /// remove2DAnnotation(), call setName() on the node before adding it to
    /// the scene. In multithreaded applications, use SoDB::writeLock() and
    /// associated methods to ensure that scenegraph operations do not happen
    /// during the time it is being modified.
    /// \code
    /// SoScreenTextAnnotation* pTextAnnot = new SoScreenTextAnnotation;
    /// pTextAnnot->setName("unique_name"); // note: spaces are not allowed. Eg: "unique name" is invalid
    /// pScene->addAnnotation(pTextAnnot);  // pScene is a pointer to SoQtGraphicsScene
    /// \endcode
    /// \param pNode	(input) Pointer to the annotation node.
    virtual void addAnnotation(SoScreenAnnotationKit* pNode);

    /// Remove an annotation referenced by name.
    /// \param name	(input) The name that was set on the node by calling setName(). See
    ///				addAnnotation().
    virtual void removeAnnotation(const SbString& name);

    /// Obtain a pointer to an annotation referenced by name.
    /// \param name	(input) The name that was set on the node by calling setName(). See
    ///				addAnnotation().
    SoNode* getAnnotation(const SbString& name) const;

    //-------- Headlight configuration methods ---------

    /// Turn camera headlight on or off. Default is on.
    virtual void setHeadlight(SbBool enable) { _pDirectionalLight->on.setValue(enable); }

    /// Get state of the camera headlight.
    /// \return true if headlight is on, else false.
    SbBool isHeadlight() const { return _pDirectionalLight->on.getValue(); }

    /// Access the camera headlight.
    /// \return Pointer to the viewer headlight for user editing.
    SoDirectionalLight* getHeadlight() const { return _pDirectionalLight; }

    //-------- Camera configuration methods ---------

    /// If this method is called before calling setSceneGraph(), this viewer
    /// will create a camera of the type specified if one is not found in the scenegraph.
    /// If this method is called when there is a scenegraph and camera already set up, the old
    /// camera is deleted and replaced by a new camera of the specified type.
    /// The default is SoPerspectiveCamera. (See SoPerspectiveCamera and SoOrthographicCamera).
    /// Note: The camera properties will have to be set again. See setCameraProperties().
    /// As an example, to set a perspective type camera, we do:
    /// \code
    /// SoQtGraphicsScene::setCameraType(SoOrthographicCamera::getClassTypeId());
    /// \endcode
    /// \param type (input) Camera type.
    virtual void setCameraType (SoType type);

    /// \return The type of camera currently active in the viewer. Note that the camera
    ///		 type might change on call to setSceneGraph() with a scenegraph already
    ///		 containing a camera.
    SoType getCameraType (void) const { return _cameraType; }

    /// Set scene camera. This will replace the first camera found in the
    /// scene when setting the scene graph. The new camera will be positioned at the same location
    /// as the camera it replaces.
    virtual void setCamera(SoCamera* pCamera);

    /// Get the active scene camera.
    /// \return Pointer to camera object
    virtual SoCamera* getCamera() const { return _pCamera; }

    /// Set properties for the camera.
    /// \param focalDist	(input) Focal distance (mm). Set this parameter to distance between the camera location and the
    ///					object of interest. When using the mouse to manipulate the view, the viewing sphere is centered
    ///					at focalDist from the camera.
    /// \param vfov	(input) Vertical field of view.
    ///				Note: The interpretation of this parameter depends on the camera type:
    ///				- SoPerspectiveCamera: vfov is height angle in radians. widthAngle/2 = atan2(tan(vfov/2) * aspectRatio)
    ///				- SoOrthographicCamera: vfov is the height in mm. Width = vfov * aspectRatio
    /// \param aspectRatio	(input) Ratio of camera viewing width to height. A few pre-defined aspect
    ///					ratios are defined in SoCamera.h (for example SO_ASPECT_VIDEO for 4:3).
    /// \param cropViewToAspectRatio (input) If set to true, viewport mapping is set to CROP_VIEWPORT_NO_FRAME
    ///							  else it is set to ADJUST_CAMERA.
    virtual void setCameraProperties(float focalDist, float vfov, float aspectRatio, SbBool cropViewToAspectRatio);

    /// Get camera properties.
    /// \param focalDist	(output) Focal distance (mm). This is also the center of rotation when the camera view
    ///					is manipulated with the mouse.
    /// \param vfov		(output) Vertical field of view.
    ///					Note: The interpretation of this parameter depends on the camera type:
    ///					- SoPerspectiveCamera: vfov is height angle in radians. widthAngle = vfov * aspectRatio
    ///					- SoOrthographicCamera: vfov is the height in mm. Width = vfov * aspectRatio
    /// \param aspectRatio	(output) Ratio of camera viewing width to height. A few pre-defined aspect
    ///					ratios are defined in SoCamera.h (for example SO_ASPECT_VIDEO for 4:3).
    /// \param cropViewToAspectRatio (output) If set to true, viewport mapping is set to CROP_VIEWPORT_NO_FRAME
    ///							  else it is set to ADJUST_CAMERA.
    /// \return true on success.
    SbBool getCameraProperties(float &focalDist, float &vfov, float &aspectRatio, SbBool &cropViewToAspectRatio);

    /// Re-position the camera to any arbitrary location in the scene.
    /// \param position	(input) Position of the camera specified in
    ///					world coordinates.
    /// \param orientation	(input) Orientation of the camera in world coordinates
    virtual void setCameraPosition(const SbVec3f &position, const SbRotation &orientation);

    /// Re-position the camera to any arbitrary location in the scene.
    /// \param position	(input) Position of the camera specified in
    ///					world coordinates.
    /// \param orientation	(input) Orientation of the camera in world coordinates
    /// \param focalDist	(input) Focal distance (mm). Set this parameter to distance between the camera location and the
    ///					object of interest. When using the mouse to manipulate the view, the viewing sphere is centered
    ///					at focalDist from the camera.
    virtual void setCameraPosition(const SbVec3f &position, const SbRotation &orientation, float focalDist);

    /// Convenience function to re-position camera to display a top view of the scene.
    void showTopView();

    /// Convenience function to re-position camera to display a front view of the scene.
    void showFrontView();

    /// Convenience function to re-position camera to display a right side view of the scene.
    void showRightSideView();

    /// Convenience function to re-position camera to display a left side view of the scene.
    void showLeftSideView();

    /// Get the current position and orientation of camera.
    /// \param position	(output) Position of the camera specified in
    ///					world coordinates.
    /// \param orientation	(output) Orientation of the camera in world coordinates
    /// \return true if a scene camera exists, in which case valid position and orientation
    ///		 are returned. If false, there isn't a scene camera and the position
    ///		 and orientation values are unchanged by this method.
    SbBool getCameraPosition(SbVec3f &position, SbRotation &orientation);

    /// Get the current position and orientation of camera.
    /// \param position	(output) Position of the camera specified in
    ///					world coordinates.
    /// \param orientation	(output) Orientation of the camera in world coordinates
    /// \param focalDist	(output) Focal distance (mm). This is also the center of rotation when the camera view
    ///					is manipulated with the mouse.
    /// \return true if a scene camera exists, in which case valid position and orientation
    ///		 are returned. If false, there isn't a scene camera and the position
    ///		 and orientation values are unchanged by this method.
    SbBool getCameraPosition(SbVec3f &position, SbRotation &orientation, float &focalDist);

    /// Convenience function to store current camera position for later
    /// retrieval with resetToHomePosition().
    virtual void saveHomePosition();

    /// Restore the camera position saved by a call to saveHomePosition().
    virtual void resetToHomePosition ();

    /// Changes the camera position to view the entire scene (the camera zoom and orientation aren't changed).
    virtual void viewAll();

    /// \enum ViewClippingStrategy
    /// \brief Camera view clipping strategies
    enum ViewClippingStrategy
    {
        FIXED,			//!< both near and far planes fixed.
        FIXED_NEAR,		//!< Near plane fixed, far plane automatically adjusted
        FIXED_FAR,		//!< Far plane fixed, near plane automatically adjusted
        AUTO			//!< Both near and far planes automatically adjusted
    };

    /// The camera renders section of the scenegraph that is within it's clipping planes.
    /// Automatic clipping is enabled by default, where the camera will automatically adjust it's
    /// clipping planes to render the entire scene. An application can set this to a different
    /// strategy to let the application control what section of the scene is rendered by the camera.
    /// Also see setViewClipping().
    /// \param strategy (input) View clipping strategy.
    virtual void setViewClippingStrategy(ViewClippingStrategy strategy) { _viewClipStrategy = strategy; }

    /// \return Currently active clipping strategy. See setViewClippingStrategy().
    ViewClippingStrategy getViewClippingStrategy() const { return _viewClipStrategy; }

    /// The camera renders section of the scenegraph that is within it's clipping planes.
    /// Call this method to set distances of clipping planes from the camera.
    /// Note
    /// - Calling this method will do something useful only if auto view clipping
    ///   is not enabled. First call setViewClippingStrategy() and set this to any option
    ///   other than AUTO before calling this method.
    /// - This method must be called again if the camera is changed (by calling setCamera()).
    /// \param near (input) Distance of the near clipping plane from the camera.
    /// \param far  (input) Distance of the far clipping plane from the camera.
    /// \return true if the call succeeded. The call will fail (and return false) if
    ///		 there is no camera active.
    virtual SbBool setViewClipping(float near, float far);

    /// \enum InteractionMode
    /// \brief User interaction modes
    enum InteractionMode
    {
        VIEWING,		//!< Mouse modifies position and orientation of camera
        SCENE_GRAPH,	//!< Forwards the event to the state machine and/or scene graph
        NO_INTERACTION	//!< mouse/keyboard inputs are ignored
    };

    /// Set mode of user interaction. In the default VIEWING mode, user events will be caught and used to
    /// influence the camera position / orientation. If mode is SCENE_GRAPH, all events in the viewer
    /// canvas (like for instance keypresses or mouseclicks and -movements) will be passed along to
    /// the scene graph. When set to NO_INTERACTION, all mouse/keyboard events are ignored.
    /// \param mode (input) Set the interaction mode.
    virtual void setInteractionMode(InteractionMode mode);

    /// Return interaction mode. See setInteractionMode().
    InteractionMode getInteractionMode() const { return _interactMode; }

    /// Resize the rendering area.
    virtual void resize( int width, int height ) { SoQtRenderArea::resize(width, height); }

protected:
    SbVec3f projectPoint(const SbVec2f& pt) const;
    void rotateCamera(const SbRotation& rot);
    void adjustCameraClippingPlanes();

    virtual void soPaintEvent();
    virtual void soKeyPressEvent( SoKeyboardEvent* pEvent);
    virtual void soKeyReleaseEvent( SoKeyboardEvent* pEvent);
    virtual void soMousePressEvent( SoMouseButtonEvent* pEvent );
    virtual void soMouseMoveEvent( SoLocation2Event* pEvent );
    virtual void soMouseReleaseEvent( SoMouseButtonEvent* pEvent );
    virtual void soWheelEvent( SoWheelEvent* pEvent );
private:
    ViewClippingStrategy	_viewClipStrategy;
    SoType					_cameraType;
    SoCamera*				_pCamera;
    SoSeparator*			_pRootGraph;
    SoNode*					_pSceneGraph;
    SoSeparator*			_pAnnotations;
    SoDirectionalLight*		_pDirectionalLight;
    SbVec2f					_startDrag;
    InteractionMode			_interactMode;
    SoMouseButtonEvent::Button _mouseButton;
    /// \cond #doxygen ignores all content until endcond
    class CameraHome //!< Camera home position. For internal use.
    {
    public:
        SbVec3f		_position;
        SbRotation	_orientation;
        float		_focalDistance;
    };
    /// \endcond
    CameraHome			_cameraHome;
}; // SoQtGraphicsScene

} // Grape

#endif // GRAPE_SOQTGRAPHICSSCENE_H
