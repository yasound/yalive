//
//  LoginViewControllerUIKit.m
//  YaLive
//
//  Created by Jérôme BLONDON on 21/01/13.
//
//

#import "LoginViewControllerUIKit.h"
#import "Settings.h"

@implementation LoginViewControllerUIKit
@synthesize mpWebView;
@synthesize delegate;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
      mpWebView = [[UIWebView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
      
      [[self view] addSubview:mpWebView];
      
      UIWindow *mainWindow = (UIWindow *)([[UIApplication sharedApplication].windows objectAtIndex:0]);
      [mainWindow addSubview:[self view]];
    }
    return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];


  [self logout];
  NSString *urlString = [[NSString alloc] initWithFormat:@"%s/live/login/", YASOUND_SERVER];
  NSURL *url = [NSURL URLWithString:urlString];
  NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
  mpWebView.delegate = self;
  [mpWebView loadRequest:requestObj];
}

- (void)dealloc
{
  [mpWebView release];
  [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotate
{
  return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
  return YES;
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


#pragma mark Webview delegate methods

- (BOOL)webView:(UIWebView *)webView2
shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType
{  
  // Intercept custom location change, URL begins with "js-call:"
  if ([[[request URL] absoluteString] hasPrefix:@"js-frame:"]) {
    NSString *requestString = [[request URL] absoluteString];
    NSArray *components = [requestString componentsSeparatedByString:@":"];
    NSString *function = [components objectAtIndex:1];
    NSString *param = [components objectAtIndex:2];
    
    if ([function isEqualToString:@"loginCompleted"])
    {
      [self loginCompleted:param];
    }
    
    // Cancel the location change
    return NO;
  }
  
  // Accept this location change
  return YES;
  
}

- (void)loginCompleted:(NSString *)sessionId
{
  [[self view] removeFromSuperview];
  [[self delegate] loginCompleted:sessionId];
}


@end
