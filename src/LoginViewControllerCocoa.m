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

@synthesize delegate;

- (id) init
{
  self = [super init];
  [self logout];
  self.window = [[NSWindow alloc]
                      initWithContentRect: NSMakeRect(0, 0, 800, 650)
                      styleMask: NSTitledWindowMask | NSClosableWindowMask
                      backing: NSBackingStoreBuffered
                      defer: NO];
  [self.window setTitle: @"Login"];
  [self.window setDelegate:self];
  
  mpWebView = [[WebView alloc] initWithFrame:NSMakeRect(0, 50, 800, 600)];
  
  NSView *superview = [self.window contentView];
  [superview addSubview:mpWebView];
  [mpWebView setUIDelegate: self];
  [mpWebView setResourceLoadDelegate: self];
  [mpWebView setFrameLoadDelegate:self];
  
  NSRect frame = NSMakeRect(320, 10, 160, 30);
  NSButton *button = [[NSButton alloc] initWithFrame:frame];
  [button setButtonType:NSMomentaryLightButton]; //Set what type button You want
  [button setBezelStyle:NSRoundedBezelStyle]; //Set what style You want
  [button setTitle:@"Cancel"];
  [button setTarget:self];
  [button setAction:@selector(onCancel:)];
  [superview addSubview:button];
  [button release];
  
  
  NSString *urlString = [[NSString alloc] initWithFormat:@"%s/live/login/", YASOUND_SERVER];
  [[mpWebView windowScriptObject] setValue:self forKey:@"console"];
  [[mpWebView mainFrame] loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlString]]];
  
  [urlString release];

  [self windowDidLoad];
  return self;
}


- (void)dealloc
{
  [mpWebView release];
  [self.window release];
  [super dealloc];
}


- (void)windowDidLoad
{
  [super windowDidLoad];
  [self.window center];
  [self.window makeKeyAndOrderFront:nil];
}


- (void)webView:(WebView *)sender didClearWindowObject:(WebScriptObject *)windowObject forFrame:(WebFrame *)frame
{
  [windowObject setValue:self forKey:@"console"];
}

// called multiple times but at least it is called :(
- (void)webView:(WebView *)sender resource:(id)identifier didFinishLoadingFromDataSource:(WebDataSource *)dataSource
{
  [[mpWebView windowScriptObject] setValue:self forKey:@"console"];
}

- (void)webView:(WebView *)sender resource:(id)identifier didFailLoadingWithError:(NSError *)error fromDataSource:(WebDataSource *)dataSource
{
}

// javascript alert() callback
- (void)webView:(WebView *)sender runJavaScriptAlertPanelWithMessage:(NSString *)message
{
  NSLog(@"%@ received %@ with '%@'", self, NSStringFromSelector(_cmd), message);
}

// return allowed methods from javascript
+ (BOOL)isSelectorExcludedFromWebScript:(SEL)selector {
  if (selector == @selector(doOutputToLog:)
      || selector == @selector(loginCompleted:)
      || selector == @selector(loginCanceled:)) {
    return NO;
  }
  return YES;
}

// javascript method name to obj selector mapping
+ (NSString *) webScriptNameForSelector:(SEL)sel {
  if (sel == @selector(doOutputToLog:)) {
    return @"log";
  } else if (sel == @selector(loginCompleted:)) {
    return @"loginCompleted";
  } else if (sel == @selector(loginCanceled:)) {
    return @"loginCanceled";
  } else {
    return nil;
  }
}

// implements console.log()
- (void) doOutputToLog: (NSString*) theMessage {
  /* write the message to the log */
  NSLog(@"console: %@", theMessage);
  
}

// implements console.loginCompleted(sessionid)
-(void)loginCompleted:(NSString*)sessionId
{
  [self.window performClose:self];
  [[self delegate] loginCompleted:sessionId];
  [NSApp abortModal];
}

-(void)loginCanceled:(NSString*)dummy
{
  [self.window performClose:self];
  [[self delegate] loginCanceled];
}

-(void)logout
{
  NSHTTPCookieStorage *cookieJar = [NSHTTPCookieStorage sharedHTTPCookieStorage];
  NSString *server = [[NSString alloc] initWithFormat:@"%s", YASOUND_SERVER];
  for (NSHTTPCookie *cookie in [cookieJar cookiesForURL:[NSURL URLWithString:server]])
  {
    [cookieJar deleteCookie:cookie];
  }
  [server release];
}

-(void)onCancel:(id)sender
{
  [self loginCanceled:@""];
}

-(void)windowWillClose:(NSNotification *)notification
{
  [[self delegate] loginCanceled];
}

@end
