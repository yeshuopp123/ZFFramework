/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZF.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFArray.h"
#include "ZFCore/ZFString.h"

#include "ZFMainEntry_sys_Qt.h"

#if ZF_ENV_sys_Qt
#include <QApplication>
#include <QWidget>
#include <QLayout>

class _ZFP_ZFMainEntry_sys_Qt_WindowLayout : public QLayout
{
    Q_OBJECT

public:
    QWidget *_ZFP_owner;
    ZFCoreArrayPOD<QLayoutItem *> _ZFP_children;

public:
    ~_ZFP_ZFMainEntry_sys_Qt_WindowLayout(void)
    {
        QLayoutItem *item = zfnull;
        while((item = this->takeAt(0)) != zfnull)
        {
            delete item;
        }
    }

public:
    virtual void addItem(QLayoutItem *layoutItem)
    {
        _ZFP_children.add(layoutItem);
    }
    virtual QSize sizeHint(void) const
    {
        return this->_ZFP_owner->geometry().size();
    }
    virtual QSize minimumSize(void) const
    {
        return QSize(0, 0);
    }
    virtual void setGeometry(const QRect &rect)
    {
        for(zfindex i = 0; i < this->_ZFP_children.count(); ++i)
        {
            this->_ZFP_children[i]->setGeometry(rect);
        }
    }

    virtual QLayoutItem *itemAt(int index) const
    {
        if(index < (int)_ZFP_children.count())
        {
            return _ZFP_children[index];
        }
        return zfnull;
    }
    virtual QLayoutItem *takeAt(int index)
    {
        if(index < (int)_ZFP_children.count())
        {
            QLayoutItem *ret = _ZFP_children[index];
            _ZFP_children.remove(index);
            return ret;
        }
        return zfnull;
    }
    virtual int indexOf(QWidget *widget) const
    {
        for(zfindex i = 0; i < _ZFP_children.count(); ++i)
        {
            if(_ZFP_children[i]->widget() == widget)
            {
                return (int)i;
            }
        }
        return -1;
    }
    virtual int count() const
    {
        return (int)_ZFP_children.count();
    }
};

ZFImpl_sys_Qt_Window::ZFImpl_sys_Qt_Window(void)
: QWidget()
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);

    _ZFP_ZFMainEntry_sys_Qt_WindowLayout *layout = new _ZFP_ZFMainEntry_sys_Qt_WindowLayout();
    layout->_ZFP_owner = this;
    this->setLayout(layout);
}

ZF_NAMESPACE_GLOBAL_BEGIN
static ZFImpl_sys_Qt_Window *_ZFP_ZFImpl_sys_Qt_rootWindowInstance = zfnull;
ZFImpl_sys_Qt_Window *ZFImpl_sys_Qt_rootWindow(void)
{
    return _ZFP_ZFImpl_sys_Qt_rootWindowInstance;
}
ZF_NAMESPACE_GLOBAL_END

int ZFMainEntry_sys_Qt(int argc, char **argv)
{
    QApplication app(argc, argv);
    ZFImpl_sys_Qt_Window window;
    _ZFP_ZFImpl_sys_Qt_rootWindowInstance = &window;
    window.show();

    ZFFrameworkInit();
    ZFArrayEditable *params = zfAllocWithoutLeakTest(ZFArrayEditable);

    for(int i = 0; i < argc; ++i)
    {
        ZFString *s = zfAllocWithoutLeakTest(ZFString, ZFStringA2Z(argv[i]));
        params->add(s);
        zfReleaseWithoutLeakTest(s);
    }

    ZFMainExecute(params);

    int ret = app.exec();
    zfReleaseWithoutLeakTest(params);
    ZFFrameworkCleanup();

    _ZFP_ZFImpl_sys_Qt_rootWindowInstance = zfnull;
    window.close();

    return ret;
}

#include "ZFMainEntry_sys_Qt.moc"
#endif

