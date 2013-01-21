//
//  LoginViewController.m
//  YaLive
//
//  Created by Jérôme BLONDON on 08/01/13.
//
//

#import "LoginViewControllerCocoa.h"
#import "Settings.h"


@implementation LoginViewControllerCocoa

- (id) initWithCB:(Header *)cb
{
  self = [super init];
  mpCB = cb;
  
  [self logout];
  mpWindow = [[NSWindow alloc]
                      initWithContentRect: NSMakeRect(0, 0, 640, 650)
                      styleMask: NSTitledWindowMask | NSClosableWindowMask
                      backing: NSBackingStoreBuffered
                      defer: NO];
  [mpWindow setTitle: @"New Window"];
  [mpWindow center];
  [mpWindow makeKeyAndOrderFront:nil];
  
  mpWebView = [[WebView alloc] initWithFrame:NSMakeRect(0, 0, 640, 650)];
  
  NSView *superview = [mpWindow contentView];
  [superview addSubview:mpWebView];

  [mpWebView setUIDelegate: self];
  [mpWebView setResourceLoadDelegate: self];
  
  
  NSString *urlString = [[NSString alloc] initWithFormat:@"%s/live/login/", YASOUND_SERVER];
  [[mpWebView mainFrame] loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlString]]];
  
  [urlString release];
  [self windowDidLoad];
  return self;
}


- (void)dealloc
{
  [mpWebView dealloc];
  [mpWindow dealloc];
  [super dealloc];
}


- (void)windowDidLoad
{
  [super windowDidLoad];
  [mpWindow center];
  [mpWindow makeKeyAndOrderFront:nil];
}

- (void)webView:(WebView *)sender didClearWindowObject:(WebScriptObject *)windowObject forFrame:(WebFrame *)frame
{
  NSLog(@"%@ received %@", self, NSStringFromSelector(_cmd));
  [windowObject setValue:self forKey:@"console"];
}


// called multiple times but at least it is called :(
- (void)webView:(WebView *)sender resource:(id)identifier didFinishLoadingFromDataSource:(WebDataSource *)dataSource
{
  [[mpWebView windowScriptObject] setValue:self forKey:@"console"];
}

- (void)webView:(WebView *)sender resource:(id)identifier didFailLoadingWithError:(NSError *)error fromDataSource:(WebDataSource *)dataSource
{
  NSLog(@"%@ received %@", self, NSStringFromSelector(_cmd));
}

// javascript alert() callback
- (void)webView:(WebView *)sender runJavaScriptAlertPanelWithMessage:(NSString *)message
{
  NSLog(@"%@ received %@ with '%@'", self, NSStringFromSelector(_cmd), message);
}

// in theory, it should be called whenever javascript is loaded.
// in practice it is never called :(
- (void)webView:(WebView *)webView windowScriptObjectAvailable:(WebScriptObject *)windowScriptObject {
  NSLog(@"%@ received %@", self, NSStringFromSelector(_cmd));
  
  [windowScriptObject setValue:self forKey:@"console"];
  
}

// return allowed methods from javascript
+ (BOOL)isSelectorExcludedFromWebScript:(SEL)selector {
  NSLog(@"%@ received %@ for '%@'", self, NSStringFromSelector(_cmd), NSStringFromSelector(selector));
  if (selector == @selector(doOutputToLog:)
      || selector == @selector(loginCompleted:)) {
    return NO;
  }
  return YES;
}

// javascript method name to obj selector mapping
+ (NSString *) webScriptNameForSelector:(SEL)sel {
  NSLog(@"%@ received %@ with sel='%@'", self, NSStringFromSelector(_cmd), NSStringFromSelector(sel));
  if (sel == @selector(doOutputToLog:)) {
    return @"log";
  } else if (sel == @selector(loginCompleted:)) {
    return @"loginCompleted";
  } else {
    return nil;
  }
}

// implements console.log()
- (void) doOutputToLog: (NSString*) theMessage {
  NSLog(@"%@ received %@ with message=%@", self, NSStringFromSelector(_cmd), theMessage);
  
  /* write the message to the log */
  NSLog(@"LOG: %@", theMessage);
  
}

// implements console.loginCompleted(sessionid)
-(void)loginCompleted:(NSString*)sessionId
{
  [mpWindow performClose:self];
  mpCB->OnLoginReceived([sessionId cStringUsingEncoding:NSASCIIStringEncoding]);
}

-(void)logout
{
  NSHTTPCookieStorage *cookieJar = [NSHTTPCookieStorage sharedHTTPCookieStorage];
  for (NSHTTPCookie *cookie in [cookieJar cookiesForURL:[NSURL URLWithString:@"http://localhost:8000"]])
  {
    [cookieJar deleteCookie:cookie];
  }
  
}


@end
