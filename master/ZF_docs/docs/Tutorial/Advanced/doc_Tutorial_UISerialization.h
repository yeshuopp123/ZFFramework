/**
 * @page DocTag_Tutorial_UISerialization UI serialization
 *
 * most of UI elements in ZFFramework is also serializable,
 * and it's easy to serialize from and to xml format\n
 * here's a quick example to see the xml output format:
 * @code
 *   zfblockedAlloc(ZFUIView, parent);
 *   zfblockedAlloc(ZFUIButtonBasic, child);
 *   parent->childAdd(child);
 *   child->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
 *   child->buttonLabelTextSet(zflineAlloc(ZFString, zfText("button")));
 *
 *   ZFSerializableData data = ZFObjectToSerializableData(parent);
 *   zfstring xmlString;
 *   ZFXmlPrint(data, ZFOutputCallbackForString(xmlString));
 *   zfLogTrimT() << zfText("serialized data:");
 *   zfLogTrimT() << xmlString;
 *
 *   ZFSerializableData dataNew = ZFXmlParse(ZFInputCallbackForString(xmlString));
 *   zfautoObject objNew = ZFObjectFromSerializableData(dataNew);
 *   zfLogTrimT() << zfText("re-serialized object:");
 *   zfLogTrimT() << objNew.toObject();
 * @endcode
 * which would output these:
 * @code
 *   serialized data:
 *   <ZFUIView>
 *       <ZFUIButtonBasic category="child" >
 *           <ZFUIViewLayoutParam category="layoutParam" >
 *               <ZFUISizeParam name="sizeParam" value="(Fill, Fill)" />
 *           </ZFUIViewLayoutParam>
 *           <ZFUITextView name="buttonLabelStyleNormal" >
 *               <ZFString name="textContent" >
 *                   <zfstring value="button" category="string" />
 *               </ZFString>
 *           </ZFUITextView>
 *       </ZFUIButtonBasic>
 *   </ZFUIView>
 *   re-serialized object:
 *   <ZFUIView(0x7afcbdd0)>
 * @endcode
 */
