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
#import "LoginViewController.h"

@interface LoginViewControllerCocoa : NSWindowController<NSWindowDelegate>
{
  WebView *mpWebView;
  NSProgressIndicator *mpProgressIndicator;
  id <LoginEventsDelegate> delegate;
}
@property (retain) id delegate;

- (id) init;
- (void)webView:(WebView *)webView windowScriptObjectAvailable:(WebScriptObject *)windowScriptObject;
- (void)webView:(WebView *)sender runJavaScriptAlertPanelWithMessage:(NSString *)message;

-(void)loginCompleted:(NSString*)sessionId;
-(void)loginCanceled:(NSString*)dummy;
-(void)onCancel:(id)sender;

-(void)logout;
@end
