/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextEdit.h"
#include "ZFCore/protocol/ZFProtocolZFString.h"

#if ZF_ENV_sys_Qt

#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QGraphicsDropShadowEffect>

class _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit : public QLineEdit
{
    Q_OBJECT

public:
    ZFUITextEdit *ownerZFUITextEdit;
    zfindex textEditEventOverrideFlag;
    zfbool textEditSecured;
    QLineEdit::EchoMode textEditEchoModeSaved;
    Qt::InputMethodHints textEditInputMethodHintsSaved;
    QString textEditTextSaved;

public:
    _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit(ZF_IN ZFUITextEdit *ownerZFUITextEdit)
    : QLineEdit()
    , ownerZFUITextEdit(ownerZFUITextEdit)
    , textEditEventOverrideFlag(0)
    , textEditSecured(zffalse)
    , textEditEchoModeSaved(this->echoMode())
    , textEditInputMethodHintsSaved(this->inputMethodHints())
    , textEditTextSaved()
    {
        this->connect(this, SIGNAL(textChanged(QString)), this, SLOT(_ZFP_textChanged(QString)));
        this->_ZFP_textColorSet(ZFUIColorBlack);
        this->setFrame(false);
        this->setAttribute(Qt::WA_MacShowFocusRect, false);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Base, Qt::transparent);
        palette.setColor(QPalette::Background, Qt::transparent);
        this->setPalette(palette);
    }

public:
    void _ZFP_textSizeSet(ZF_IN zfint v)
    {
        QFont font = this->font();
        font.setPixelSize((int)v);
        this->setFont(font);
    }
    void _ZFP_textColorSet(ZF_IN ZFUIColor v)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::Text, ZFImpl_sys_Qt_ZFUIKit_ZFUIColorToQColor(v));
        this->setPalette(palette);
    }
    void _ZFP_textShadowUpdate(ZF_IN const ZFUIColor &textShadowColor, ZF_IN const ZFUISize &textShadowOffset)
    {
        if(textShadowColor == ZFUIColorTransparent)
        {
            this->setGraphicsEffect(zfnull);
        }
        else
        {
            QGraphicsDropShadowEffect *effect = qobject_cast<QGraphicsDropShadowEffect *>(this->graphicsEffect());
            if(effect == zfnull)
            {
                effect = new QGraphicsDropShadowEffect(this);
                this->setGraphicsEffect(effect);
            }
            effect->setBlurRadius(0);
            effect->setColor(ZFImpl_sys_Qt_ZFUIKit_ZFUIColorToQColor(textShadowColor));
            effect->setOffset(textShadowOffset.width, textShadowOffset.height);
        }
    }
    void _ZFP_textEditSecuredSet(ZF_IN zfbool textEditSecured)
    {
        if(this->textEditSecured == textEditSecured)
        {
            return ;
        }
        this->textEditSecured = textEditSecured;
        if(this->textEditSecured)
        {
            this->textEditEchoModeSaved = this->echoMode();
            this->textEditInputMethodHintsSaved = this->inputMethodHints();

            this->setEchoMode(QLineEdit::Password);
            this->setInputMethodHints(Qt::ImhHiddenText | Qt::ImhNoPredictiveText | Qt::ImhNoAutoUppercase);
        }
        else
        {
            this->setEchoMode(this->textEditEchoModeSaved);
            this->setInputMethodHints(this->textEditInputMethodHintsSaved);
        }
    }
    void _ZFP_textContentSet(QString const &text, zfbool needNotify)
    {
        if(this->textEditTextSaved == text)
        {
            return ;
        }
        int cursor = this->cursorPosition();
        zfblockedAllocWithoutLeakTest(ZFString, s, (void *)(new QString(text)));
        if(ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldChange(this->ownerZFUITextEdit, s))
        {
            int positionSaved = this->cursorPosition();
            int textLengthOld = this->text().length();
            this->textEditTextSaved = text;
            this->setText(text);
            if(text.length() >= textLengthOld)
            {
                cursor = positionSaved + text.length() - textLengthOld;
            }
            this->setCursorPosition(cursor);
        }
        else
        {
            this->setText(this->textEditTextSaved);
            this->setCursorPosition(cursor);
        }
        if(needNotify)
        {
            ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextChange(this->ownerZFUITextEdit, s);
        }
    }

public:
    virtual bool event(QEvent *event)
    {
        if(this->textEditEventOverrideFlag > 0)
        {
            return true;
        }
        bool ret = false;
        switch(event->type())
        {
            case QEvent::FocusIn:
                ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditBegin(this->ownerZFUITextEdit);
                ret = QLineEdit::event(event);
                break;
            case QEvent::FocusOut:
                ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditEnd(this->ownerZFUITextEdit);
                ret = QLineEdit::event(event);
                break;
            case QEvent::KeyPress:
            {
                QKeyEvent *keyEvent = (QKeyEvent *)event;
                if(keyEvent->key() == Qt::Key_Return)
                {
                    ret = true;
                }
                else
                {
                    ret = QLineEdit::event(event);
                }
            }
                break;
            case QEvent::KeyRelease:
            {
                QKeyEvent *keyEvent = (QKeyEvent *)event;
                if(keyEvent->key() == Qt::Key_Return)
                {
                    ret = true;
                    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextReturnClicked(this->ownerZFUITextEdit);
                }
                else
                {
                    ret = QLineEdit::event(event);
                }
            }
                break;
            default:
                ret = QLineEdit::event(event);
                break;
        }

        return ret;
    }
public slots:
    void _ZFP_textChanged(const QString &text)
    {
        if(this->textEditEventOverrideFlag > 0)
        {
            return ;
        }
        ++(this->textEditEventOverrideFlag);
        this->_ZFP_textContentSet(text, zftrue);
        --(this->textEditEventOverrideFlag);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextEditImpl_sys_Qt, ZFUITextEdit, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QLineEdit"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFString, zfText("Qt:QString"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeTextEditCreate(ZF_IN ZFUITextEdit *textEdit)
    {
        return new _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit(textEdit);
    }
    virtual void nativeTextEditDestroy(ZF_IN ZFUITextEdit *textEdit,
                                       ZF_IN void *nativeTextEdit)
    {
        delete ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, nativeTextEdit);
    }

// ============================================================
// properties
public:
    virtual void textEditEnableSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN zfbool textEditEnable)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->setReadOnly(!textEditEnable);
    }
    virtual void textEditSecuredSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN zfbool textEditSecured)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->_ZFP_textEditSecuredSet(textEditSecured);
    }
    virtual void textEditKeyboardTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                         ZF_IN ZFUITextEditKeyboardTypeEnum textEditKeyboardType)
    {
        // not supported
    }
    virtual void textEditKeyboardReturnTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                               ZF_IN ZFUITextEditKeyboardReturnTypeEnum textEditKeyboardReturnType)
    {
        // not supported
    }

    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_OUT zfindexRange &textSelectRange)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        if(nativeImplView->hasSelectedText())
        {
            textSelectRange.start = nativeImplView->selectionStart();
            textSelectRange.count = nativeImplView->selectedText().length();
        }
        else
        {
            textSelectRange.start = nativeImplView->cursorPosition();
            textSelectRange.count = 0;
        }
    }
    virtual void textSelectRangeSet(ZF_IN ZFUITextEdit *textEdit, ZF_IN const zfindexRange &textSelectRange)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        if(textSelectRange.count != 0)
        {
            nativeImplView->setSelection(textSelectRange.start, textSelectRange.count);
        }
        else
        {
            nativeImplView->setCursorPosition(textSelectRange.start);
        }
    }

public:
    virtual void textContentSet(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN ZFString *text)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        if(text != zfnull)
        {
            nativeImplView->_ZFP_textContentSet(*ZFCastStatic(QString *, text->nativeString()), zffalse);
        }
        else
        {
            nativeImplView->_ZFP_textContentSet("", zffalse);
        }
    }
    virtual void textAppearanceSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN ZFUITextAppearanceEnum const &textAppearance)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        QFont font = nativeImplView->font();
        switch(textAppearance)
        {
            case ZFUITextAppearance::e_Normal:
                font.setBold(false);
                font.setItalic(false);
                break;
            case ZFUITextAppearance::e_Bold:
                font.setBold(true);
                font.setItalic(false);
                break;
            case ZFUITextAppearance::e_Italic:
                font.setBold(false);
                font.setItalic(true);
                break;
            case ZFUITextAppearance::e_BoldItalic:
                font.setBold(true);
                font.setItalic(true);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        nativeImplView->setFont(font);
    }
    virtual void textAlignSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIAlignFlags const &textAlign)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->setAlignment(ZFImpl_sys_Qt_ZFUIKit_ZFUIAlignFlagsToQAlignment(textAlign));
    }
    virtual void textColorSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIColor const &textColor)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->_ZFP_textColorSet(textColor);
    }
    virtual void textShadowColorSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN ZFUIColor const &textShadowColor)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->_ZFP_textShadowUpdate(textShadowColor, textEdit->textShadowOffset());
    }
    virtual void textShadowOffsetSet(ZF_IN ZFUITextEdit *textEdit,
                                     ZF_IN ZFUISize const &textShadowOffset)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->_ZFP_textShadowUpdate(textEdit->textShadowColor(), textShadowOffset);
    }
    virtual void textSizeSet(ZF_IN ZFUITextEdit *textEdit,
                             ZF_IN zfint textSize)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->_ZFP_textSizeSet(textSize);
    }

// ============================================================
// layout
public:
    virtual ZFUISize measureNativeTextEdit(ZF_IN ZFUITextEdit *textEdit,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        QFont font = nativeImplView->font();
        font.setPixelSize(textSize);
        QFontMetrics fm(font);
        QRect rect = fm.boundingRect(nativeImplView->text());
        int padding = 8;
        return ZFUISizeMake(rect.width() + padding, rect.height() + padding);
    }

public:
    virtual void textEditBegin(ZF_IN ZFUITextEdit *textEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->setFocus();
    }
    virtual void textEditEnd(ZF_IN ZFUITextEdit *textEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = ZFCastStatic(_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *, textEdit->nativeImplView());
        nativeImplView->clearFocus();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextEditImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUITextEdit_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

