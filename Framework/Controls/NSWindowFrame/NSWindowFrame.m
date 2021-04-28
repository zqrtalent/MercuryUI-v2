//
//  MyModalWindow.m
//  WindowCreationApp
//
//  Created by Zaqro Butskrikidze on 10/15/16.
//  Copyright © 2016 zaqro butskrikidze. All rights reserved.
//

#import "NSWindowFrame.h"
#import "NSWindowFrameView.h"

@interface NSWindowFrame()
{
    _Font _selectedFont;
    void* _fontChangeCallbackData;
    std::function<void(void*,_Font*)> _fontChangeCallback;
}

@end

@implementation NSWindowFrame

-(id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag

{
    self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag];
    NSLog(@"init");
   
    [self setAcceptsMouseMovedEvents:YES];
    return self;
}

-(void)keyDown:(NSEvent *)theEvent
{
    [super keyDown:theEvent];
    NSLog(@"event code %d", theEvent.keyCode);
}

-(void)mouseDown:(NSEvent *)theEvent
{
    [super mouseDown:theEvent];
    NSLog(@"mouse down");
}

-(void)mouseUp:(NSEvent *)theEvent
{
    [super mouseUp:theEvent];
    
    NSLog(@"mouse up");
}

-(void)rightMouseDown:(NSEvent *)theEvent
{
    [super rightMouseDown:theEvent];
    
    NSLog(@"right mouse down");
}

-(void)rightMouseUp:(NSEvent *)theEvent
{
    [super rightMouseUp:theEvent];
    
    NSLog(@"right mouse up");
}

-(void)mouseMoved:(NSEvent *)theEvent
{
    [super mouseMoved:theEvent];
    
    //NSLog(@"mouse moved");
}

-(void)mouseDragged:(NSEvent *)theEvent
{
    [super mouseDragged:theEvent];
    
    NSLog(@"mouse dragged");
}

-(void)scrollWheel:(NSEvent *)theEvent
{
    [super scrollWheel:theEvent];
    
    NSLog(@"scroll wheel x=%f y=%f z=%f", theEvent.deltaX, theEvent.deltaY, theEvent.deltaZ);
}

-(void)magnifyWithEvent:(NSEvent*)theEvent
{
    [super magnifyWithEvent:theEvent];
    
    NSLog(@"magnify with event");
}

-(NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
    NSLog(@"resize event %f %f", frameSize.width, frameSize.height);
    [((NSWindowFrameView*)self.contentView) resizeEvent:frameSize];
    return frameSize;
}

-(void)windowWillMiniaturize:(NSNotification *)notification
{
    NSLog(@"on minimize");
}


 -(void)windowWillClose:(NSNotification *)notification
{
}

-(BOOL)windowShouldClose:(id)sender
{
    return YES;
}

-(void) chooseFont:(_Font*)selectedFont withCallback:(std::function<void(void*,_Font*)>)callback andData:(void*)data
{
    _selectedFont.DeleteObject();
    selectedFont->CopyFont(&_selectedFont);
    _fontChangeCallbackData = data;
    _fontChangeCallback = callback;
    
    NSFontManager* fontMgr = [NSFontManager sharedFontManager];
    [fontMgr setTarget: self];
    [fontMgr setAction:@selector(changeMyCustomFont:)];
    
    auto fontPanel = [NSFontPanel sharedFontPanel];
    [fontPanel setPanelFont:_selectedFont isMultiple: NO];
    [fontPanel setWorksWhenModal: YES];
    [fontPanel makeKeyAndOrderFront:nil];
    fontPanel.enabled = YES;
}

-(void) changeMyCustomFont:(NSFontManager*)sender
{
    auto font = _selectedFont.Detach();
    _selectedFont.Attach([sender convertFont: font]);
    NSLog(@"New font: %@", (NSFont*)_selectedFont);
    _fontChangeCallback(_fontChangeCallbackData, &_selectedFont);
}

@end
