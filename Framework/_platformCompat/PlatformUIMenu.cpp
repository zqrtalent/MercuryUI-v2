#include "PlatformUIMenu.h"
#ifdef _WINDOWS
#elif __APPLE__
    #include <Appkit/NSMenu.h>
    #include <Appkit/NSCursor.h>
    #include <CoreGraphics/CGEvent.h>
#endif

@implementation NSMenuCommandHandler

-(id)initialize:(ESFrameBase*)pFrameWnd handler:(CommandHandlerProc)handlerProc
{
    _pFrameWnd = pFrameWnd;
    _handler = handlerProc;
    return self;
}

-(void)onCommand:(id)sender
{
    int nCommandId = [(NSString*)[sender representedObject] intValue];
    (_pFrameWnd->*_handler)(nCommandId);
}

@end

_UIMenu::_UIMenu(ESFrameBase* pFrameWnd, CommandHandlerProc commandProc){
    _pFrameWnd = pFrameWnd;
    _commandProc = commandProc;
    _menuInstance = nil;
    _commandHandler = nil;
}

_UIMenu::~_UIMenu(){
    DestroyMenu();
    
    _pFrameWnd = nullptr;
    _commandProc = nullptr;
    _commandHandler = nil;
}

NSMenu*
_UIMenu::CreatePopupMenuRecursive(_UIMenuItem* pItem){
    NSMenu* pMenu = [[NSMenu alloc] init];
    while(pItem != nullptr){
        if(pItem->m_bSeparator){
            [pMenu addItem:[NSMenuItem separatorItem]];
        }
        else{
            NSMenuItem* item = [[NSMenuItem alloc] init];
            item.title = [NSString stringWithUTF8String:pItem->m_sItemText.c_str()];
            item.target = _commandHandler;
            item.action = @selector(onCommand:);
            item.representedObject = [NSString stringWithFormat:@"%lu", pItem->m_nItemId];
            [pMenu addItem:item];
            
            if(pItem->m_pChildItem != nullptr){
                NSMenu* pSubMenu = CreatePopupMenuRecursive(pItem->m_pChildItem);
                if(pSubMenu != nil)
                    [pMenu setSubmenu:pSubMenu forItem:item];
            }
        }
        
        pItem = pItem->m_pNextItem;
    }
    return pMenu;
}

bool
_UIMenu::CreatePopupMenu(_UIMenuItem* pItems){
    if(_menuInstance != nil || pItems == nil)
        return false;
    
    _commandHandler = [[NSMenuCommandHandler alloc] initialize:_pFrameWnd handler:_commandProc];
    _menuInstance = CreatePopupMenuRecursive(pItems);
    [_menuInstance setAutoenablesItems:NO];
    return true;
}

bool
_UIMenu::ShowContextMenu(_Point pt, HWND hwndOwner){
    if(!_menuInstance)
        return false;
    NSEvent *event = [NSEvent mouseEventWithType:NSEventTypeRightMouseUp
                                                 location:CGPointMake(pt.x, pt.y)
                                            modifierFlags:0
                                                timestamp:0
                                             windowNumber:[hwndOwner.window windowNumber]
                                                  context:nil
                                              eventNumber:0
                                               clickCount:0
                                                 pressure:0];
    
    [NSMenu popUpContextMenu:_menuInstance withEvent:event forView:hwndOwner];
    return true;
}

bool
_UIMenu::AppendItem(UINT_PTR nItemId, TCHAR* pszItemText, int flags){
    return false;
}

void
_UIMenu::DestroyMenu(){
    if(_menuInstance != nil){
        _menuInstance = nil;
    }
}
