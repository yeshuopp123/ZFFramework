/**
 * @page DocTag_Tutorial_Styleable Styleable
 *
 * with styleable logic, you may copy contents from another object easily\n
 * to declare a styleable, it's recommended to declare like this:
 * @code
 *   // in header files:
 *   zfclass YourStyleableObject : zfextends ParentStyleableObject
 *   {
 *       ZFOBJECT_DECLARE(YourStyleableObject, ParentStyleableObject)
 *       ZFSTYLE_DEFAULT_DECLARE(YourStyleableObject)
 *   };
 *
 *   // in cpp files
 *   ZFSTYLE_DEFAULT_DEFINE(YourStyleableObject)
 * @endcode
 *
 * once declared, object's style can be copied as styleable easily:
 * @code
 *   obj->styleableCopyFrom(anotherStyleableObject);
 * @endcode
 * and you can access default style by the macro generated method:
 * @code
 *   YourStyle::DefaultStyle();
 * @endcode
 * then, it's convenient to chain different styleable object to achieve simple style logic:
 * @code
 *   zfclass SomeStyle : zfextends Parent
 *   {
 *       ZFOBJECT_DECLARE(SomeStyle, Parent)
 *
 *       ZFPROPERTY_RETAIN_WITH_INIT(SomeType, someName,
 *                                   ZFPropertyInitValue(OtherStyle::DefaultStyle()->sth()))
 *   };
 * @endcode
 *
 * @section DocTag_Tutorial_Styleable_AdvancedStyleable Advanced styleable
 * styleable is useful to achieve advance skin logic,
 * which already has a typical impl supplied by #zfSkin:
 * @code
 *   zfSkinApplyZFStyleable(yourObj, zfText("your_skin_key"));
 * @endcode
 * this code register yourObj to #zfSkin,
 * once the skin value associated with "your_skin_key" has changed by #zfSkinSet,
 * yourObj would be notified and copy style from it automatically\n
 * \n
 * also, thanks to ZFClass's instance observer,
 * you may achieve more complex style logic:
 * @code
 *   ZFLISTENER_LOCAL(myObjCreated, {
 *       MyStyleableObject *obj = listenerData.sender->toAny();
 *       // change default setting for all instance of MyStyleableObject
 *       obj->myPropertySet(xxx);
 *       // or apply style copy (consume more CPU)
 *       obj->styleableCopyFrom(yyy);
 *   })
 *   MyStyleableObject::ClassData()->instanceObserverAdd(myObjCreated);
 * @endcode
 */

