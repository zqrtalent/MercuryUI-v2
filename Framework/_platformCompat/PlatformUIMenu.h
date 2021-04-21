#if !defined(PLATFORMUIMENU_H_INCLUDED)
#define      PLATFORMUIMENU_H_INCLUDED

#include "PlatformCompat.h"
#include "PlatformPoint.h"

class ESFrameBase;
typedef void (__thiscall ESFrameBase::*CommandHandlerProc)(int);

@interface NSMenuCommandHandler : NSObject
{
@protected
    CommandHandlerProc _handler;
    ESFrameBase* _pFrameWnd;
}

-(id)initialize:(ESFrameBase*)pFrameWnd handler:(CommandHandlerProc)handlerProc;
-(void)onCommand:(id)sender;
@end

class _UIMenuItem
{
public:
    _UIMenuItem(){
        m_pNextItem = nullptr;
        m_pChildItem = nullptr;
        m_bSeparator = true;
    };
    
    _UIMenuItem(_string sText, UINT_PTR nId, _UIMenuItem* pPrevItem = nullptr, bool separatorItem = false){
        m_sItemText = sText;
        m_nItemId = nId;
        m_pNextItem = nullptr;
        m_pChildItem = nullptr;
        m_bSeparator = separatorItem;
        
        if(pPrevItem)
            pPrevItem->m_pNextItem = this;
    };
    
    ~_UIMenuItem(){
    
    };
    
public:
    UINT_PTR        m_nItemId;
    _string         m_sItemText;
    UINT_PTR        m_nFlags;
    _UIMenuItem*    m_pNextItem;
    _UIMenuItem*    m_pChildItem;
    bool            m_bSeparator;
};

class _UIMenuItemCollection
{
public:
    _UIMenuItemCollection(){
        m_pHead = nullptr;
        m_pTail = nullptr;
    }
    
    ~_UIMenuItemCollection(){
    }
    
    _UIMenuItem* AddItem(_UIMenuItem* pItem, _UIMenuItem* pParent = nullptr){
        ASSERT(pItem);
        if(!m_pHead){
            m_pHead = pItem;
            m_pTail = pItem;
        }
        else{
            if(!pParent){
                if(m_pTail)
                    m_pTail->m_pNextItem = pItem;
                m_pTail = pItem;
            }
            else{
                if(!pParent->m_pChildItem)
                    pParent->m_pChildItem = pItem;
                else{
                    _UIMenuItem* pTailSibling = pParent->m_pChildItem;
                    while(pTailSibling->m_pNextItem)
                        pTailSibling = pTailSibling->m_pNextItem;
                    pTailSibling->m_pNextItem = pItem;
                }
            }
        }
        return pItem;
    }
    
    _UIMenuItem* GetHead(){
        return m_pHead;
    };
    
    _UIMenuItem* GetTail(){
        return m_pTail;
    };
    
protected:
    _UIMenuItem* m_pHead;
    _UIMenuItem* m_pTail;
};

class _UIMenu
{
public:
    _UIMenu(ESFrameBase* pFrameWnd, CommandHandlerProc commandProc);
    ~_UIMenu();
    
    bool CreatePopupMenu(_UIMenuItem* pItems);
    bool ShowContextMenu(_Point pt, HWND hwndOwner);
    bool AppendItem     (UINT_PTR nItemId, TCHAR* pszItemText, int flags);
    void DestroyMenu    ();
    
protected:
    NSMenu* CreatePopupMenuRecursive(_UIMenuItem* pItem);

protected:
    ESFrameBase*            _pFrameWnd;
    CommandHandlerProc      _commandProc;
    NSMenu*                 _menuInstance;
    NSMenuCommandHandler*   _commandHandler;
};
#endif // !defined(PLATFORMUIMENU_H_INCLUDED)

