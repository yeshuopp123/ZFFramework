/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextEdit.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit : UITextField<UITextFieldDelegate>
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUITextEdit) *impl;
@property (nonatomic, assign) ZFUITextEdit *ownerZFUITextEdit;

@property (nonatomic, strong) NSString *fontName;
@property (nonatomic, assign) zfint textSize;

- (void)_textFieldTextChanged:(UITextField *)textField;
@end

@implementation _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit

// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];

    self.autoresizesSubviews = NO;
    self.backgroundColor = [UIColor clearColor];

    self.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;

    self.delegate = self;
    [self addTarget:self action:@selector(_textFieldTextChanged:) forControlEvents:UIControlEventEditingChanged];

    return self;
}
- (void)dealloc
{
    [self removeTarget:self action:@selector(_textFieldTextChanged:) forControlEvents:UIControlEventEditingChanged];
    self.fontName = nil;
}

- (void)setTextSize:(zfint)newTextSize ZFImpl_sys_iOS_overrideProperty
{
    if(self->_textSize != newTextSize)
    {
        self->_textSize = newTextSize;
        UIFont *newFont = [UIFont fontWithName:self.fontName size:newTextSize];
        self.font = newFont;
    }
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditBegin(self.ownerZFUITextEdit);
}
- (void)textFieldDidEndEditing:(UITextField *)textField
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditEnd(self.ownerZFUITextEdit);
}
- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSString *nativeStringNew = [textField.text stringByReplacingCharactersInRange:range withString:string];
    zfblockedAllocWithoutLeakTest(ZFString, stringNew, (__bridge void *)nativeStringNew);
    return ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldChange(self.ownerZFUITextEdit, stringNew);
}
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextReturnClicked(self.ownerZFUITextEdit);
    return YES;
}

- (void)_textFieldTextChanged:(UITextField *)textField
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextChange(self.ownerZFUITextEdit, zflineAllocWithoutLeakTest(ZFString, (__bridge void *)textField.text));
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextEditImpl_sys_iOS, ZFUITextEdit, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UITextField"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFString, zfText("iOS:NSString"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeTextEditCreate(ZF_IN ZFUITextEdit *textEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = [_ZFP_ZFUITextEditImpl_sys_iOS_TextEdit new];
        nativeImplView.ownerZFUITextEdit = textEdit;
        return (__bridge_retained void *)nativeImplView;
    }
    virtual void nativeTextEditDestroy(ZF_IN ZFUITextEdit *textEdit,
                                       ZF_IN void *nativeTextEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *tmp = (__bridge_transfer _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)nativeTextEdit;
        tmp = nil;
    }

// ============================================================
// properties
public:
    virtual void textEditEnableSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN zfbool textEditEnable)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.enabled = textEditEnable;
    }
    virtual void textEditSecuredSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN zfbool textEditSecured)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.secureTextEntry = textEditSecured;
    }
    virtual void textEditKeyboardTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                         ZF_IN ZFUITextEditKeyboardTypeEnum textEditKeyboardType)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        switch(textEditKeyboardType)
        {
            case ZFUITextEditKeyboardType::e_Normal:
                nativeImplView.keyboardType = UIKeyboardTypeDefault;
                break;
            case ZFUITextEditKeyboardType::e_CharBased:
                nativeImplView.keyboardType = UIKeyboardTypeASCIICapable;
                break;
            case ZFUITextEditKeyboardType::e_PhonePad:
                nativeImplView.keyboardType = UIKeyboardTypePhonePad;
                break;
            case ZFUITextEditKeyboardType::e_NumberPad:
                nativeImplView.keyboardType = UIKeyboardTypeNumberPad;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    virtual void textEditKeyboardReturnTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                               ZF_IN ZFUITextEditKeyboardReturnTypeEnum textEditKeyboardReturnType)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        switch(textEditKeyboardReturnType)
        {
            case ZFUITextEditKeyboardReturnType::e_Normal:
                nativeImplView.returnKeyType = UIReturnKeyDefault;
                break;
            case ZFUITextEditKeyboardReturnType::e_Next:
                nativeImplView.returnKeyType = UIReturnKeyNext;
                break;
            case ZFUITextEditKeyboardReturnType::e_Search:
                nativeImplView.returnKeyType = UIReturnKeySearch;
                break;
            case ZFUITextEditKeyboardReturnType::e_Done:
                nativeImplView.returnKeyType = UIReturnKeyDone;
                break;
            case ZFUITextEditKeyboardReturnType::e_Go:
                nativeImplView.returnKeyType = UIReturnKeyGo;
                break;
            case ZFUITextEditKeyboardReturnType::e_Send:
                nativeImplView.returnKeyType = UIReturnKeySend;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }

    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_OUT zfindexRange &textSelectRange)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        UITextRange *rangeImpl = nativeImplView.selectedTextRange;
        if(rangeImpl == nil)
        {
            textSelectRange = zfindexRangeZero;
            return ;
        }
        NSInteger start = [nativeImplView offsetFromPosition:nativeImplView.beginningOfDocument toPosition:rangeImpl.start];
        NSInteger end = [nativeImplView offsetFromPosition:nativeImplView.beginningOfDocument toPosition:rangeImpl.end];
        textSelectRange.start = start;
        textSelectRange.count = end - start;
    }
    virtual void textSelectRangeSet(ZF_IN ZFUITextEdit *textEdit, ZF_IN const zfindexRange &textSelectRange)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        UITextPosition *start = [nativeImplView positionFromPosition:nativeImplView.beginningOfDocument offset:textSelectRange.start];
        UITextPosition *end = [nativeImplView positionFromPosition:start offset:textSelectRange.count];
        [nativeImplView setSelectedTextRange:[nativeImplView textRangeFromPosition:start toPosition:end]];
    }

public:
    virtual void textContentSet(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN ZFString *text)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        if(text != zfnull)
        {
            NSString *nativeString = (__bridge NSString *)text->nativeString();
            nativeImplView.text = nativeString;
        }
        else
        {
            nativeImplView.text = nil;
        }
    }
    virtual void textAppearanceSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN ZFUITextAppearanceEnum const &textAppearance)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        switch(textAppearance)
        {
            case ZFUITextAppearance::e_Normal:
                nativeImplView.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_Bold:
                nativeImplView.fontName = [UIFont boldSystemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_Italic:
                nativeImplView.fontName = [UIFont italicSystemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_BoldItalic:
                nativeImplView.fontName = @"Helvetica-BoldOblique";
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    virtual void textAlignSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIAlignFlags const &textAlign)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        if(ZFBitTest(textAlign, ZFUIAlign::e_LeftInner))
        {
            nativeImplView.textAlignment = NSTextAlignmentLeft;
        }
        else if(ZFBitTest(textAlign, ZFUIAlign::e_RightInner))
        {
            nativeImplView.textAlignment = NSTextAlignmentRight;
        }
        else if(textAlign == ZFUIAlign::e_Center)
        {
            nativeImplView.textAlignment = NSTextAlignmentCenter;
        }
        else
        {
            nativeImplView.textAlignment = NSTextAlignmentLeft;
        }
    }
    virtual void textColorSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIColor const &textColor)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.textColor = ZFImpl_sys_iOS_ZFUIKit_ZFUIColorToUIColor(textColor);
    }
    virtual void textShadowColorSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN ZFUIColor const &textShadowColor)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.layer.shadowColor = ZFImpl_sys_iOS_ZFUIKit_ZFUIColorToUIColor(textShadowColor).CGColor;
    }
    virtual void textShadowOffsetSet(ZF_IN ZFUITextEdit *textEdit,
                                     ZF_IN ZFUISize const &textShadowOffset)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.layer.shadowOffset = ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(textShadowOffset);
    }
    virtual void textSizeSet(ZF_IN ZFUITextEdit *textEdit,
                             ZF_IN zfint textSize)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.textSize = textSize;
    }

// ============================================================
// layout
public:
    virtual ZFUISize measureNativeTextEdit(ZF_IN ZFUITextEdit *textEdit,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        zfint textSizeSaved = nativeImplView.textSize;
        nativeImplView.textSize = textSize;
        CGSize sizeTmp = ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(sizeHint);
        if(sizeTmp.width <= 0)
        {
            sizeTmp.width = 30000;
        }
        if(sizeTmp.height <= 0)
        {
            sizeTmp.height = 30000;
        }
        sizeTmp = [nativeImplView sizeThatFits:sizeTmp];
        nativeImplView.textSize = textSizeSaved;
        return ZFImpl_sys_iOS_ZFUIKit_ZFUISizeFromCGSize(sizeTmp);
    }

public:
    virtual void textEditBegin(ZF_IN ZFUITextEdit *textEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        [nativeImplView becomeFirstResponder];
    }
    virtual void textEditEnd(ZF_IN ZFUITextEdit *textEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        [nativeImplView resignFirstResponder];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextEditImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

