//
//  LoginViewController.h
//  YaLive
//
//  Created by Jérôme BLONDON on 08/01/13.
//
//

#import <Cocoa/Cocoa.h>
#import "WebKit/WebKit.h"
#import "Deck.h"
#import "nui.h"

@interface LoginViewController : NSWindowController
{
  NSWindow *mpWindow;
  WebView *mpWebView;
  Deck *mpCB;
}
- (id) initWithCB:(Deck *)cb;
- (void)webView:(WebView *)webView windowScriptObjectAvailable:(WebScriptObject *)windowScriptObject;
- (void)webView:(WebView *)sender runJavaScriptAlertPanelWithMessage:(NSString *)message;

-(void)loginCompleted:(NSString*)sessionId;
-(void)logout;
@end
