//
//  CurrentUserUIKit.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 24/01/13.
//
//

#include "CurrentUser.h"

#include "PDKeychainBindings.h"

const nglString& Models::CurrentUser::GetSessionId()
{
  mSessionId = "";
  NSString *sessionid = [[PDKeychainBindings sharedKeychainBindings] objectForKey:@"sessionid"];
  if (sessionid)
  {
    mSessionId = [sessionid cStringUsingEncoding:NSASCIIStringEncoding];
  }
  return mSessionId;
}

void Models::CurrentUser::SetSessionId(const nglString& sessionId)
{
  PDKeychainBindings *bindings=[PDKeychainBindings sharedKeychainBindings];
  NSString *nsSessionId = [[NSString alloc] initWithUTF8String:sessionId.GetChars()];
  [bindings setObject:nsSessionId forKey:@"sessionid"];
  [nsSessionId release];
}
