#pragma once

#include "BsPrerequisites.h"
#include "Scene/BsComponent.h"
#include "Math/BsMath.h"
#include "Input/BsVirtualInput.h"

namespace bs
{
    /** Component that controls zoom of the camera. */
    class CameraZoomer : public Component
	{
	public:
        CameraZoomer(const HSceneObject& parent);

		/** Triggered once per frame. Allows the component to handle input and move. */
		void update() override;

	private:
        VirtualAxis mZoomAxis; /**< Input device axis used for controlling object's zoom. */

        static const float ZOOM_STEP; /**< Determines scale factor for zooming. */
	};
}
