//
//  NSViewMy.m
//  OSXWindowEvents
//
//  Created by Zaqro Butskhrikidze on 9/12/14.
//  Copyright (c) 2014 Zaqro Butskhrikidze. All rights reserved.
//

#import "NSViewMy.h"
#include <string>
#include "Controls/MercuryBaseWnd.h"
#include "Controls/ESChildControl.h"
#include "_PlatformCompat/PlatformDeviceContext.h"
#include "Utility/GrowableMemory.h"
#include "_PlatformCompat/PlatformUIMenu.h"
#include "Controls/ESChildTextBox.h"

@interface NSViewMy()
{
@protected
    _DC         memDC;
    CGImageRef  image;
    MercuryBaseView* _pMercuryView1;
    
    CGImageRef testImage;
    NSBitmapImageRep* testImageRep;
    _Image testImage_;
    
    NSWindow* window;
}

@end


@implementation NSViewMy

-(id)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    return self;
}

-(void)viewDidMoveToWindow
{
    [super viewDidMoveToWindow];
    
    // Initialize mercury view.
    [self initializeMercuryView:CGRectMake(0.0, 0.0, 0.0, 0.0) wnd:nil mercuryView:nullptr];
    
    if(_pMercuryView)
    {
        // Create UI control programatically
//        auto pTextBox = new ESChildTextBox();
//        pTextBox->AllowMouseEvents(true);
//        pTextBox->SetText("Hello there !!!\r\nHello There !!!");
//        pTextBox->SetMultiLine(true);
//        pTextBox->SetSel(0, 3, true, false);
//        _Rect rcRect(5, 5, 200, 200);
//        _pMercuryView->AddChildControl(pTextBox, 10000, rcRect, nullptr);
        
        // Load UI components from design file.
        _string designFile = _T("design/Lobby.des");
        //_string designFile = _T("/Volumes/OSX/Users/ZqrTalent/Desktop/Dev/design/Lobby.des");
        //_string designFile = _T("/Volumes/OSX-DATA/Dev/ESPoker_06.01.2014/_bin/ESPokerClient_Debug/design/LoginDialog.des");
        _pMercuryView->SetDesignerMode(true);
        _pMercuryView->LoadFromDesignFile(designFile);
    }
}

-(void)initializeMercuryView1:(NSRect)rcView wnd:(NSWindow*)wnd
{
    if( _pMercuryView1 == NULL )
    {
        NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);
        NSTrackingArea *area = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
        [self addTrackingArea: area];
        
        //_menu = [[NSMenuCtrl alloc] initialize];
        
        //FILE* file = fopen(_T("/Volumes/OSX-DATA/Dev/ESPoker_06.01.2014/_bin/ESPokerClient_Debug/design/Lobby.des"), "r+b");
        //_Rect rcDraw(rcView.origin.x, rcView.origin.y, (rcView.origin.x + rcView.size.width), (rcView.origin.y + rcView.size.height));
        _pMercuryView1 = new MercuryBaseView();
        _pMercuryView1->SetHWND(self);
        _pMercuryView1->SetDesignerMode(true);
        
        /*
        _string arrCOntrolNames[] = {
            _T("ESChildControlLabel"),
            _T("ESChildControlImage"),
            _T("ESChildControlImageButton"),
            _T("ESChildControlList"),
            _T("ESChildTextBox"),
            _T("ESChildControlTabCtrl"),
            _T("ESChildBanner"),
            _T("ESChildControlCheckBox"),
            _T("ESChildScrollBar"),
            _T("ESChildTabView"),
            _T("ESChildComboBox"),
            _T("ESSimpleRenderView"),
            _T("ESChildSlider"),
            _T("ESChildControlPropertyGrid"),
            _T("ESChildProgressCtrl"),
            _T("ESChildAvatar"),
        };
        
        for(int i=0; i<sizeof(arrCOntrolNames)/sizeof(_string); i++){
            ESChildControl* p = ESChildControl::GetChildClassInstance(arrCOntrolNames[i]);
            p->GetVariableInfo(_T("A"));
        }*/
                
        _string designFile = _T("design/Lobby.des");
        //designFile = _T("TournamentLobby.des");
        //_string designFile = _T("/Volumes/OSX/Users/ZqrTalent/Desktop/Dev/design/Lobby.des");
        //_string designFile = _T("/Volumes/OSX-DATA/Dev/ESPoker_06.01.2014/_bin/ESPokerClient_Debug/design/LoginDialog.des");
        _pMercuryView->LoadFromDesignFile(designFile);
        [self setNeedsDisplay:YES];
    }
}

-(void)resizeEvent:(NSSize)frameSize
{
    [super resizeEvent: frameSize];
    /*
    if( _pMercuryView != NULL )
    {
        //_pMercuryView->OnSize(0, (int)frameSize.width, (int)frameSize.height);
        _pMercuryView->OnSize(0, (int)[self frame].size.width, (int)[self frame].size.height);
        [self setNeedsDisplay:YES]; // Redraw view
    }
    else
    {
        //[self initializeMercuryView:self.frame wnd:[self window]];
    }*/
}

-(void)mouseMoved:(NSEvent *)theEvent
{
    [super mouseMoved:theEvent];
    /*
    if(_pMercuryView != NULL)
    {
        int nFlags = 0;
        NSEventModifierFlags flags = [theEvent modifierFlags];
        if( (flags & NSControlKeyMask) == NSControlKeyMask)
            nFlags |= MK_CONTROL;
        else
            if( (flags & NSCommandKeyMask) == NSCommandKeyMask)
                nFlags |= MK_COMMAND;
            else
                if( (flags & NSShiftKeyMask) == NSShiftKeyMask)
                    nFlags |= MK_SHIFT;
        
        NSPoint pt = [theEvent locationInWindow];
        NSPoint ptView = [self convertPoint:pt toView:self];
        _pMercuryView->OnMouseMove(nFlags, _Point(ptView.x, [self bounds].size.height - ptView.y));
        NSLog(@"mouse moved1 - buttonNumber %ld", (long)[theEvent subtype]);
    }*/
    
}

-(void)mouseDragged:(NSEvent *)theEvent
{
    [super mouseDragged:theEvent];
    /*
    if(_pMercuryView != NULL)
    {
        int nFlags = MK_LBUTTON;
        NSPoint pt = [theEvent locationInWindow];
        NSPoint ptView = [self convertPoint:pt toView:self];
        _pMercuryView->OnMouseMove(nFlags, _Point(ptView.x, [self bounds].size.height - ptView.y));
    }
    NSLog(@"mouse dragged");*/
}

-(void)mouseDown:(NSEvent *)theEvent
{
    [super mouseDown:theEvent];
    /*
    if(_pMercuryView != NULL)
    {
        int nFlags = 0;
        NSEventModifierFlags flags = [theEvent modifierFlags];
        if( (flags & NSControlKeyMask) == NSControlKeyMask)
            nFlags |= MK_CONTROL;
        else
            if( (flags & NSCommandKeyMask) == NSCommandKeyMask)
                nFlags |= MK_COMMAND;
            else
                if( (flags & NSShiftKeyMask) == NSShiftKeyMask)
                    nFlags |= MK_SHIFT;
        
        NSPoint pt = [theEvent locationInWindow];
        NSPoint ptView = [self convertPoint:pt toView:self];
        _pMercuryView->OnLButtonDown(nFlags, _Point(ptView.x, [self bounds].size.height - ptView.y));
    }*/
    
    /*
    NSMenu *theMenu = [[NSMenu alloc] initWithTitle:@"Contextual Menu"];
    SEL sel1 = @selector(testItemClick:);
    [theMenu insertItemWithTitle:@"Beep" action:sel1 keyEquivalent:@"" atIndex:0];
    [theMenu insertItemWithTitle:@"Honk" action:@selector(honk:) keyEquivalent:@"" atIndex:1];
    
    [NSMenu popUpContextMenu:theMenu withEvent:theEvent forView:self];
     */
    
    //[_menu showMenu:theEvent forView:self];
}

-(void)mouseUp:(NSEvent *)theEvent
{
    [super mouseUp:theEvent];
    return;
    
    if(_pMercuryView1 != NULL)
    {
        NSPoint pt = [theEvent locationInWindow];
        NSPoint ptView = [self convertPoint:pt toView:self];
        _pMercuryView1->OnLButtonUp(0, _Point(ptView.x, [self bounds].size.height - ptView.y));
    }
   /*
    NSRect frame = NSMakeRect(100, 100, 200, 200);
    NSUInteger styleMask =    NSBorderlessWindowMask;
    NSRect rect = [NSWindow contentRectForFrameRect:frame styleMask:styleMask];
    window =  [[NSWindow alloc] initWithContentRect:rect styleMask:styleMask backing: NSBackingStoreBuffered    defer:false];
    [window setBackgroundColor:[NSColor blueColor]];
    [window makeKeyAndOrderFront: window];
    */
    
    
    window = [[NSWindow alloc]
                        initWithContentRect:NSMakeRect(500, 500, 100, 100)
                        styleMask:NSBorderlessWindowMask
                        backing:NSBackingStoreBuffered
                        defer:NO];
    
    // configure window.
    [window setLevel:NSPopUpMenuWindowLevel];
    [window setHasShadow:NO];
    [window setIgnoresMouseEvents:YES];
    
    // show window.
    [window makeKeyAndOrderFront:self];
}

-(void)rightMouseUp:(NSEvent *)theEvent
{
    [super rightMouseUp:theEvent];
    return;
    
    if(_pMercuryView1 != NULL)
    {
        NSPoint pt = [theEvent locationInWindow];
        NSPoint ptView = [self convertPoint:pt toView:self];
        _pMercuryView1->OnRButtonUp(0, _Point(ptView.x, [self bounds].size.height - ptView.y));
    }
}

-(void)drawRect:(NSRect)rect
{
    [super drawRect:rect];
    return;
    
    NSLog(@"redraw %f %f %f %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    
    CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    if( _pMercuryView1 != NULL )
    {
        _Rect rcDraw(rect.origin.x, rect.origin.y, (rect.origin.x + rect.size.width), (rect.origin.y + rect.size.height));
        _pMercuryView1->OnPaint(context, rcDraw);
    }
    
    /*
    NSString* imageFile = @"/Users/ZqrTalent/Library/Developer/Xcode/DerivedData/MercuryUI-ckaqtaasuzdoasgxjirrmdvjynxf/Build/Products/Debug/design/images/logo.png";
    NSImage* nsImage = [[NSImage alloc] initWithContentsOfFile:imageFile];
    NSRect nsRect = NSMakeRect(0.0, 0.0, [nsImage size].width, [nsImage size].height);
    CGImageRef imageRef = [nsImage CGImageForProposedRect:nil context:[NSGraphicsContext currentContext] hints:nil];
    CGContextDrawImage(context, nsRect, imageRef);
    */
    
    return;
    if(testImage_.IsNull())
    {
        //testImage_.CreateDIBBitmap(32, nullptr, 0, 0);
    }

    if(testImage == nullptr)
    {
        BYTE* pColorTable = (BYTE*)malloc(sizeof(UINT)*g_szVScrollThumbImg.cy*g_szVScrollThumbImg.cx);
        int loop = 0;
        for(int i=0; i<4*g_szVScrollThumbImg.cy*g_szVScrollThumbImg.cx; i+=4)
        {
            UINT pixel = g_nVScrollThumbImg[loop];
            pColorTable[i] = (BYTE)((pixel & 0xFF0000)>>16); // R
            pColorTable[i+1] = (BYTE)((pixel & 0xFF00)>>8); // G
            pColorTable[i+2] = (BYTE)(pixel & 0xFF); // B
            pColorTable[i+3] = (BYTE)((pixel & 0xFF000000)>>24); // A
            //pColorTable[i+3] = 0xFF; // A
            loop ++;
        }
        
        int nWidth = g_szHeaderItemNormalImg.cx, nHeight = g_szHeaderItemNormalImg.cy, bytesPerRow = g_szHeaderItemNormalImg.cx*4, nBPP = 32;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        //const UINT *pColorTable = &g_nVScrollThumbImg[0];
        CGDataProviderRef dataProviderRef = CGDataProviderCreateWithData(NULL, &g_nHeaderItemNormalImg[0], (bytesPerRow*nHeight), NULL);
        CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast|kCGBitmapByteOrder32Big;
        
        
        testImage = CGImageCreate(nWidth, nHeight, 8, nBPP, bytesPerRow, colorSpaceRef, bitmapInfo, dataProviderRef,
                                       NULL, true, kCGRenderingIntentDefault);
        
        free(pColorTable);
        CGDataProviderRelease(dataProviderRef);
        CGColorSpaceRelease(colorSpaceRef);
        
        testImageRep = [[NSBitmapImageRep alloc] initWithCGImage:testImage];
        
        testImage_.Attach(testImage);
    }
    
    //[[NSBitmapImageRep alloc] initWithCGImage:image]
    
    //CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    //CGContextDrawImage(context, CGRectMake(0.0, 0.0, g_szVScrollThumbImg.cx, g_szVScrollThumbImg.cy), testImage);
    //CGContextDrawImage(context, CGRectMake(0.0, 0.0, g_szVScrollThumbImg.cx, g_szVScrollThumbImg.cy), [testImageRep CGImage]);
    
    _DC d;
    
    _Rect rc(0, 0, (int)testImageRep.size.width, (int)testImageRep.size.height);
    d.Attach(context);
    testImage_.RenderImage(&d, rc, rc, false, false);
    d.Detach();
    return;
    
    
    if( memDC.IsNull() )
    {
        _Rect rcFill1(0, 0, 200, 200);
        _Rect rcFill2(25, 25, 75, 75);
        
        memDC.CreateMemoryBitmapDC(32, 200, 200);
        memDC.FillSolidRect(rcFill1, ARGB(0, 255, 255, 255));
        memDC.FillSolidRect(rcFill2, ARGB(200, 200, 100, 255));
        
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        strcpy(lf.lfFaceName, "Zapfino");
        lf.lfHeight = 14;
        lf.lfItalic = 1;
        
        _Font font;
        font.CreateFontIndirect(&lf);
        memDC.SelectObject(&font);
        memDC.SetTextColor(ARGB(0, 0, 0, 255));
        
        
        NSString* drawStr = @"სუპ ბოი სუპ ბოი სუპ ბოი სუპ ბოი სუპ ბოი სუპ ბოი";
        //memDC.ExtTextOut(0, 0, 0, _Rect(0, 0, 200, 200), [drawStr cStringUsingEncoding:NSUTF8StringEncoding]);
        
        

        
        
        [NSGraphicsContext saveGraphicsState];
        [NSGraphicsContext setCurrentContext:memDC];
        [font setInContext:memDC];
        
        
        NSDictionary* dict22 = [NSDictionary dictionaryWithObjectsAndKeys:font, NSFontAttributeName,
                               [NSColor greenColor], NSForegroundColorAttributeName,
                               nil, NSBackgroundColorAttributeName,
                               [NSShadow alloc], NSShadowAttributeName,
                               
                               nil];
        
        //[drawStr drawInRect:CGRectMake(25.0, 30.0, 250.0, 50.0) withAttributes:memDC._fontAttributes];
        //[drawStr drawInRect:CGRectMake(15.0, 20.0, 250.0, 50.0) withAttributes:dict22];
        [NSGraphicsContext restoreGraphicsState];
        
        
        
        NSFont* nsfont = [NSFont fontWithName:@"Zapfino" size:14.0];
        //NSGraphicsContext* graphContext = [NSGraphicsContext graphicsContextWithGraphicsPort:memBitmap flipped:YES];
        
        [NSGraphicsContext saveGraphicsState];
        [NSGraphicsContext setCurrentContext:memDC];
        //CGContextSetTextDrawingMode(memBitmap, kCGTextFillClip);
        
        NSSize sz = [drawStr sizeWithAttributes:nil];
        
        [font setInContext:memDC];
        
        NSFontDescriptor* desc = [nsfont fontDescriptor];
        NSDictionary* fontAttr = [desc fontAttributes];
        
        //NSSize sz1 = [drawStr sizeWithAttributes:fontAttr];
        
        //[drawStr drawAtPoint:CGPointMake(15.0, 50) withAttributes:nil];
        
        //NSDictionary* dictAttr = [NSDictionary dictionaryWithObject:nsfont forKey:NSFontAttributeName];
        
        NSDictionary* dict2 = [NSDictionary dictionaryWithObjectsAndKeys:nsfont, NSFontAttributeName,
                               [NSColor greenColor], NSForegroundColorAttributeName,
                               nil, NSBackgroundColorAttributeName,
                               [NSShadow alloc], NSShadowAttributeName,
                               
                               nil];
                                  
        
        //[drawStr drawInRect:CGRectMake(15.0, 20.0, 250.0, 50.0) withAttributes:dict2];
        [NSGraphicsContext restoreGraphicsState];
        
        
        /*
        int width = 200;
        int height = 200;
        size_t bytesPerRow = width * (32/8);
        size_t dataBytesCt = bytesPerRow*height;
        CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
        //It does not works I do not know yet!!!
        //void* data = calloc(dataBytesCt, 0);
        
        CGContextRef context = NULL;
        context = CGBitmapContextCreate(NULL, width, height, 8, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedLast);
        CGColorSpaceRelease(colorSpace);
        
        CGContextSetRGBFillColor(context, 1.0, 0.0, 0.0, 0.5);
        CGContextFillRect(context, CGRectMake(0.0, 0.0, 200, 100));
        
        CGContextFillRect(context, CGRectMake(100.0, 0.0, 200, 100));
        image = CGBitmapContextCreateImage(context);
        //_dibImage = [[NSBitmapImageRep alloc] initWithCGImage:refImg];
        //_context = [NSGraphicsContext graphicsContextWithBitmapImageRep:_dibImage];
         */
    }
    
    //CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    //CGContextSetRGBFillColor(context, 1.0, 0.0, 0.0, 0.5);
    //CGContextFillRect(context, CGRectMake(0.0, 0.0, 200, 300));
    
    
    //CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    //CGContextDrawImage(context, CGRectMake(0.0, 0.0, 200, 200), image);

    
    //CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    //_DC::DrawDC(context, 0, 0, 200, 200, &memDC, 0, 0, 0);
    }

@end
