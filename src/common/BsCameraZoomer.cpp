#include "BsCameraZoomer.h"
#include "Math/BsVector3.h"
#include "Utility/BsTime.h"
#include "Math/BsMath.h"
#include "Scene/BsSceneObject.h"
#include "Platform/BsCursor.h"

namespace bs
{
    const float CameraZoomer::ZOOM_STEP = 0.01f;

    CameraZoomer::CameraZoomer(const HSceneObject& parent)
        : Component(parent)
	{
		// Set a name for the component, so we can find it later if needed
        setName("CameraZoomer");

		// Get handles for key bindings. Actual keys attached to these bindings will be registered during app start-up.
        mZoomAxis = VirtualAxis("Zoom");
	}

    void CameraZoomer::update()
	{
        float delta = gVirtualInput().getAxisValue(mZoomAxis) * ZOOM_STEP;
        if(delta != 0.f) //If the input changed
        {
            const Transform &transform = SO()->getTransform();
            Vector3 camPos = transform.getPosition();
            Vector3 camFwd = transform.getForward();

            camPos -= delta * camFwd; //Translate the camera.

            SO()->setPosition(camPos);
        }
    }
}
