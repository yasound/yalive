//
//  LoginViewController.h
//  YaLive
//
//  Created by Jérôme BLONDON on 21/01/13.
//
//

#import <Foundation/Foundation.h>
#import "nui.h"
#import "Header.h"

@protocol LoginEventsDelegate <NSObject>
@required
- (void) loginCompleted: (NSString *)sessionId;
@end


@interface LoginViewController : NSObject <LoginEventsDelegate>
{
  Header *mpCB;
  id mpImpl;
}

-(id)initWithCB:(Header*) cb;
-(void)logout;

@end
