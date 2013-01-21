//
//  LoginViewController.h
//  YaLive
//
//  Created by Jérôme BLONDON on 08/01/13.
//
//

#import "nui.h"
#import <Cocoa/Cocoa.h>
#import "WebKit/WebKit.h"
#import "Header.h"

@interface LoginViewControllerCocoa : NSWindowController
{
  NSWindow *mpWindow;
  WebView *mpWebView;
  Header *mpCB;
}
- (id) initWithCB:(Header*)cb;
- (void)webView:(WebView *)webView windowScriptObjectAvailable:(WebScriptObject *)windowScriptObject;
- (void)webView:(WebView *)sender runJavaScriptAlertPanelWithMessage:(NSString *)message;

-(void)loginCompleted:(NSString*)sessionId;
-(void)logout;
@end
