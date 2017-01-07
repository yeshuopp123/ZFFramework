/**
 * @page DocTag_Tutorial_UsingInterface Using interface
 *
 * similar to Interface in Java and protocol in Object-C,
 * ZFFramework has ZFInterface for interface types\n
 * to declare a ZFInterface:
 * @code
 *   zfinterface MyInterface : zfextends ZFInterface
 *   {
 *       ZFINTERFACE_DECLARE(MyInterface, ZFInterface)
 *   };
 *
 *   // in cpp files
 *   ZFOBJECT_REGISTER(MyInterface)
 * @endcode
 * -  use zfinterface to show it's an ZFInterface type
 * -  due to some limitatation,
 *   all interface type must be inherited from ZFInterface
 *   by "zfextends ZFInterface"
 * -  use ZFINTERFACE_DECLARE for necessary macro expansion
 * -  similar to ZFObject,
 *   use ZFOBJECT_REGISTER to register your interface
 *   if you want it to be reflectable
 *
 * \n
 * once declared, an interface can be implemented by other object
 * @code
 *   zfclass MyObject : zfextends ParentObject
 *                    , zfimplements MyInterface,
 *                    , zfimplements AnotherInterface
 *   {
 *       ZFOBJECT_DECLARE(MyObject, ParentObject)
 *       ZFIMPLEMENTS_DECLARE(MyInterface, AnotherInterface)
 *   };
 * @endcode
 * -  use zfimplements to implement an interface
 * -  use ZFIMPLEMENTS_DECLARE for necessary macro expansion,
 *   an object can implement more than one interfaces
 *
 * \n
 * @section DocTag_Tutorial_UsingInterface_Limitations Limitations
 * when an object implements an interface that
 * extends from another interface,
 * and none of this object's parent have implements the parent interface,
 * you must also declare the implement relationship with the parent interface:
 * @code
 *   // must inherit from ZFInterface
 *   zfinterface MyInterface : zfextends ZFInterface
 *   {
 *       // declare relationship of inheritance
 *       ZFINTERFACE_DECLARE(MyInterface, ParentInterface)
 *   };
 *
 *   // must also declare inherit from ParentInterface
 *   zfclass MyObject : zfextends ZFObject
 *                    , zfimplements MyInterface
 *                    , zfimplements ParentInterface
 *   {
 *       ZFOBJECT_DECLARE(MyObject, ZFObject)
 *       // must also declare inherit from ParentInterface
 *       ZFIMPLEMENTS_DECLARE(MyInterface, ParentInterface)
 *   };
 * @endcode
 */
