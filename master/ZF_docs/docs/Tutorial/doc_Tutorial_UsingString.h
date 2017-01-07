/**
 * @page DocTag_Tutorial_UsingString Using string
 *
 * ZFFramework use these string types:
 * -  const zfchar *\n
 *   C-style string, use just like "const char *",
 *   however, it's recommended to wrap it with zfText
 * -  zfstring\n
 *   C++-style string, use just like std::string
 * -  ZFString / ZFStringEditable\n
 *   ZFFramework object type string, using native string as container,
 *   would have better performance for large strings
 *
 * @code
 *   // common C-style string, best performance
 *   const zfchar *cStyle = zfText("C-style string");
 *   zfchar cStyle2[32] = {0};
 *   zfscpy(cString2, zfText("copied C-style string"));
 *   zfLogT() << cStyle << cStyle2;
 *
 *   // common C++-style string, high performance yet easy to use
 *   zfstring cppStyle(zfText("C++-style string"));
 *   zfstring cppStyle2;
 *   cppStyle2 = zfText("copied C++-style string")
 *   zfLogT() << cppStyle << cppStyle2.cString();
 *
 *   // string container as object type
 *   zfblockedAlloc(ZFString, zfStyle, zfText("object-style string"));
 *   zfblockedAlloc(ZFStringEditable, zfStyle2);
 *   zfStyle2->stringValueSet(zfText("copied object-style string"));
 *   zfLogT() << zfStyle << zfStyle2->stringValue();
 * @endcode
 * \n
 * by default, all strings in ZFFramework use UTF-8 as string encoding
 */
