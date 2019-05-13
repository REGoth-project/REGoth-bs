/**
 * \file
 *
 * Contains utility makros to have less noise inside the actual components for RTTI.
 * RTTI needs at least two things to be working:
 *
 *  1. RTTI-class which lists the fields to serialize and has methods to
 *     create a new instance of the class it describes.
 *  2. in the actual component to get an instance of the RTTI-class.
 *
 * The second part is cluttering the component definition and is always the same,
 * hence a makro is useful here.
 */

#pragma once
#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Reflection/BsRTTIType.h>
#include <Scene/BsSceneObject.h>
#include <exception/Throw.hpp>

/**
 * For use in the actual components header. Declares the functions for accessing
 * the RTTI-instance of the component.
 *
 * Example:
 *
 *    class MyComponent : public bs::Component
 *    {
 *    public:
 *      REGOTH_DECLARE_RTTI(MyComponent)
 *
 *    protected:
 *      MyComponent() = default; // For RTTI
 *    }
 *
 * The parameterless constructor is still required to be defined manually. Also
 * note that there is not semicolon after the makro!
 * Futhermore, the makro invocation needs to be in `public`-land of the component.
 */
#define REGOTH_DECLARE_RTTI(classname)                           \
  friend class bs::SceneObject;                                  \
  friend class RTTI_##classname;                                 \
  static bs::RTTITypeBase* getRTTIStatic();                      \
  decltype(classname::getRTTIStatic()) getRTTI() const override; 

/**
 * For use in the actual components source. Defines the functions for accessing
 * the RTTI-instance of the component.
 *
 * Example:
 *
 *     namespace REGoth
 *     {
 *       REGOTH_DEFINE_RTTI(MyComponent)
 *     }
 *
 * The current namespace needs direct access to the components class.
 * Also note that there is not semicolon after the makro!
 */
#define REGOTH_DEFINE_RTTI(classname)          \
                                               \
  bs::RTTITypeBase* classname::getRTTIStatic() \
  {                                            \
    return RTTI_##classname::instance();       \
  }                                            \
                                               \
  bs::RTTITypeBase* classname::getRTTI() const \
  {                                            \
    return classname::getRTTIStatic();         \
  }

/**
 * For use in the RTTI-class. Implements the glue code to create an instance of the component
 * the RTTI describes.
 *
 * Use this if the RTTI describes a class derived from bs::Component.
 */
#define REGOTH_IMPLEMENT_RTTI_CLASS_FOR_COMPONENT(classname)   \
                                                               \
  bs::SPtr<bs::IReflectable> newRTTIObject() override          \
  {                                                            \
    return bs::SceneObject::createEmptyComponent<classname>(); \
  }                                                            \
                                                               \
  const bs::String& getRTTIName() override                     \
  {                                                            \
    static bs::String name = #classname;                       \
    return name;                                               \
  }                                                            \
                                                               \
  bs::UINT32 getRTTIId() override                              \
  {                                                            \
    return TID_REGOTH_##classname;                             \
  }

/**
 * For use in the RTTI-class. Implements the glue code to create an instance of the object the
 * RTTI describes.
 *
 * Use this if the RTTI describes a class derived from bs::IReflectable.
 */
#define REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(classname) \
                                                               \
  bs::SPtr<bs::IReflectable> newRTTIObject() override          \
  {                                                            \
    return bs::bs_shared_ptr_new<classname>();                 \
  }                                                            \
                                                               \
  const bs::String& getRTTIName() override                     \
  {                                                            \
    static bs::String name = #classname;                       \
    return name;                                               \
  }                                                            \
                                                               \
  bs::UINT32 getRTTIId() override                              \
  {                                                            \
    return TID_REGOTH_##classname;                             \
  }

/**
 * For use in the RTTI-class. Implements the glue code for an abstract class
 * which cannot be instantiated.
 */
#define REGOTH_IMPLEMENT_RTTI_CLASS_ABSTRACT(classname)                                    \
                                                                                           \
  bs::SPtr<bs::IReflectable> newRTTIObject() override                                      \
  {                                                                                        \
    REGOTH_THROW(InvalidParametersException, "Cannot create instance of abstract class!"); \
  }                                                                                        \
                                                                                           \
  const bs::String& getRTTIName() override                                                 \
  {                                                                                        \
    static bs::String name = #classname;                                                   \
    return name;                                                                           \
  }                                                                                        \
                                                                                           \
  bs::UINT32 getRTTIId() override                                                          \
  {                                                                                        \
    return TID_REGOTH_##classname;                                                         \
  }
