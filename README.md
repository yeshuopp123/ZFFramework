# Introduction

welcome to [ZFFramework](http://ZFFramework.com), a cross-platform, lightweight, mid-level application framework in C++

everything here starts with "ZF", which stands for "Zero Framework"

* it's a framework, you are able to write all of your code under ZFFramework to build your app quickly
* it's not a traditional framework, it's able to be loaded like a dynamic library, plug and play

    you are able to embed ZFFramework to your native framework, or embed your native code to ZFFramework

* as a mid-level framework, it's designed to be able to run at any platform that supplies C++03

    normal app, OpenGL app, or even text-based console app, everything's done if you are able to supply your own implementation


# Current status

it's designed to be a complete and complex framework, current code status:

![ZF2048_iOS](https://raw.githubusercontent.com/ZFFramework/zfframework.github.com/master/res/ZFFramework/code_summary.jpg)

* finished
    * core module (memory management, reflection, serialization)
    * basic UI module (view, window, label, image view, button, layout, scroll view, list view)
    * basic algorithm (xml, json, regexp, md5, base64, crc32, encryption)
    * common platform implementations (iOS, Android, Qt)
* working
    * auto script binding (still under feasibility testing)
    * more useful UI modules
    * basic network module
    * basic database module
* future
    * standalone visual UI editor
    * more IDE / compile env integrations
    * more platform implementations


# Quick overview

* this piece of code shows how to show a hello world on UI and log output

``` c++
    #include "ZFUIKit.h" // for UI module
    ZFMAIN_ENTRY(params) // app starts from here
    {
        // show a hello world as a text view
        zfblockedAlloc(ZFUIWindow, window);
        window->windowShow();
        zfblockedAlloc(ZFUITextView, textView);
        window->childAdd(textView);
        textView->textContentStringSet(zfText("hello world"));

        // show a hello world to log output
        zfLogT() << zfText("hello wolrd");
        return 0;
    }
```

* and here are screenshot of demo 2048 game built by ZFFramework:

    <table border="0"><tr>
    <td><img src="https://raw.githubusercontent.com/ZFFramework/zfframework.github.com/master/res/ZFFramework/ZF2048_iOS.png"></td>
    <td><img src="https://raw.githubusercontent.com/ZFFramework/zfframework.github.com/master/res/ZFFramework/ZF2048_Android.png"></td>
    <td><img src="https://raw.githubusercontent.com/ZFFramework/zfframework.github.com/master/res/ZFFramework/ZF2048_Qt_Windows.png"></td>
    <td><img src="https://raw.githubusercontent.com/ZFFramework/zfframework.github.com/master/res/ZFFramework/ZF2048_Qt_MacOS.png"></td>
    <td><img src="https://raw.githubusercontent.com/ZFFramework/zfframework.github.com/master/res/ZFFramework/ZF2048_Qt_Ubuntu.png"></td>
    </tr></table>


# Requirement

* for the core modlue:

    * C++03 compatible compiler (require templates, no boost/RTTI/exceptions required)
    * STL containers (require: map/unordered_map/vector/deque/list), or supply custom wrapper

* for the implementation module:

    * depends on the actual platform implementation


# Main features

* minimum requirement
* built-in reflection, serialzation, styleable, leak test
* "core + protocol + dynamic implementation" design

    support any platform if you are able to supply a native C++ implementation, most of implementation can be replaced easily, and implementation is required only if its owner module being used

* easy to communicate with native code

    even to embed UI elements and native UI elements with each other

* UI module to write cross-platform UI easily
* built-in auto scale logic to support multiple screen size

    you have no need to write size-dependent code in both app and implementation


# What we do

* aiming to be portable and can be ported easily,
    aiming to be lightweighted and able to be embeded easily,
    aiming to use 20% code to do 80% work
* supply Java-like / ObjectC-like app level APIs to build up small/medium sized app easily


# Getting started

* [Download](https://zfframework.github.io/doc/_doc_tag__download.html) necessary files
* [Setup](https://zfframework.github.io/doc/_doc_tag__setup.html) set up necessary environment for ZFFramework
* [Tutorial](https://zfframework.github.io/doc/_doc_tag__tutorial.html) quick tutorial to code with ZFFramework
* [FAQ](https://zfframework.github.io/doc/_doc_tag__f_a_q.html)


# License

ZFFramework is under MIT license ([see here](https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt)),
feel free to copy or modify or use it, except that you must leave the license unmodified

* project home page: [http://ZFFramework.com](http://ZFFramework.com)
* blog: [http://zsaber.com](http://zsaber.com)


if you like my work, please consider donate:

* [paypal](http://paypal.com/): zerofighter@163.com
* [alipay](http://alipay.com/): zerofighter@163.com

contact master@zsaber.com (Chinese or English only) before donate, we would really appreciate for your help

