#pragma once

#include <RTTI/RTTIUtil.hpp>

#include <BsPrerequisites.h>
#include <Reflection/BsIReflectable.h>

namespace REGoth
{
  /**
   * Result of a shadow sampling query.
   */
  struct ShadowSample
  {
    float brightness;
  };

  /**
   * Abstract representation of a technique that can be used to obtain shadow information for a query
   * object (possibly influencing the object's visual appearance).
   *
   * The actual sampling is implementation specific and could e. g. use information from the spatial
   * neighborhood of the query object.
   */
  class ShadowSampler : public bs::IReflectable
  {
  public:
    virtual ~ShadowSampler() = default;

    /**
     * Perform shadow sampling for a given scene object.
     *
     * @param  querySO  The object to obtain the sample for.
     * @param  sample   Reference to which the query result is written.
     *                  Only filled with valid data if the return value is true.
     *
     * @return True if sampling for the given object was successful, false otherwise.
     */
    virtual bool sampleFor(bs::HSceneObject querySO, ShadowSample& sample) const = 0;

    REGOTH_DECLARE_RTTI(ShadowSampler)
  };
}  // namespace REGoth