/**
 * @page DocTag_Tutorial_Reflection Reflection
 *
 * @section DocTag_Tutorial_Reflection_Class Reflect class
 * all ZFObject class or ZFInterface class can be reflected if:
 * -  any instance of the class have ever been created
 * -  ClassData have ever been called
 * -  ZFOBJECT_REGISTER have been declared
 *
 * if matched any of the condition above,
 * you may reflect the class's meta data:
 * @code
 *   // find the class meta data, null if not found
 *   const ZFClass *cls = ZFClass::classForName(zfText("MyObject"));
 *   // you may create the object's instance by the meta data
 *   zfautoObject objTmp = cls->newInstance();
 *   ZFObject *obj = objTmp.toObject();
 *   // cast to desired type if necessary
 *   MyObject *myObj = ZFCastZFObject(MyObject *, obj);
 *   // newly created object should be released after use
 *   zfRelease(obj);
 *
 *   // also, you may access class's info by ZFClass's method
 *   zfLogT() << zfText("class name:") << cls->className();
 * @endcode
 *
 * @section DocTag_Tutorial_Reflection_Method Reflect method
 * a method is reflectable
 * if it's declared by ZFMETHOD_XXX_DECLARE_XXX series,
 * you may reflect it like this:
 * @code
 *   // find the method info by its owner ZFClass's method,
 *   // return null if not found
 *   const ZFMethod *method = cls->methodForName(zfText("myFunc"));
 *   // invoke the method,
 *   // you must explicitly specify the return type and param type,
 *   // you must ensure the proto type of the method is right,
 *   // and must ensure obj is the right object that have the method
 *   method->execute<ReturnType, ParamType0, ParamType1>(obj, param0, param1);
 * @endcode
 *
 * @section DocTag_Tutorial_Reflection_Property Reflect property
 * also, a property is reflectable
 * if it's declared by ZFPROPERTY_XXX series,
 * you may reflect it like this:
 * @code
 *   // find the property info by its owner ZFClass's method,
 *   // return null if not found
 *   const ZFProperty *property = cls->propertyForName(zfText("myProperty"));
 *   // change the property's value by setter method,
 *   // also, must ensure the type is all right
 *   property->setterMethod()->execute<void, Type const &>(obj, newValue);
 * @endcode
 */
