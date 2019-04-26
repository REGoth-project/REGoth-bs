#pragma once

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

/**
 * For use in the actual components header. Declares the functions for accessing
 * the RTTI-instance of the component.
 *
 * Example:
 *
 *    class MyComponent : public bs::Component
 *    {
 *    public:
 *      REGOTH_COMPONENT_DECLARE_RTTI(MyComponent)
 *
 *    protected:
 *      MyComponent() = default; // For RTTI
 *    }
 *
 * The parameterless constructor is still required to be defined manually. Also
 * note that there is not semicolon after the makro!
 * Futhermore, the makro invocation needs to be in `public`-land of the component.
 *
 * @note There seems to be a problem with `friend` declaring the RTTI-class
 *       since that doesn't seem to get access to the constructor. So you
 *       might have to make the parameterless constructor public until that
 *       is figured out.
 */
#define REGOTH_DECLARE_RTTI(classname) \
  friend class RTTI_##classname;                 \
  static bs::RTTITypeBase* getRTTIStatic();      \
  bs::RTTITypeBase* getRTTI() const override;

/**
 * For use in the actual components source. Defines the functions for accessing
 * the RTTI-instance of the component.
 *
 * Example:
 *
 *     namespace REGoth
 *     {
 *       REGOTH_COMPONENT_DEFINE_RTTI(MyComponent)
 *     }
 *
 * The current namespace needs direct access to the components class.
 * Also note that there is not semicolon after the makro!
 */
#define REGOTH_DEFINE_RTTI(classname) \
                                                \
  bs::RTTITypeBase* classname::getRTTIStatic()  \
  {                                             \
    return RTTI_##classname::instance();        \
  }                                             \
                                                \
  bs::RTTITypeBase* classname::getRTTI() const  \
  {                                             \
    return classname::getRTTIStatic();          \
  }
