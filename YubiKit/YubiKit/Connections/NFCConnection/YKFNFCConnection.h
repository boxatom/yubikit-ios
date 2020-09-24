// Copyright 2018-2019 Yubico AB
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#import <Foundation/Foundation.h>
#import "YKFNFCTagDescription.h"
#import "YKFNFCOTPService.h"
#import "YKFKeyU2FService.h"
#import "YKFKeyFIDO2Session.h"
#import "YKFKeyOATHSession.h"
#import "YKFKeyRawCommandSession.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, YKFNFCConnectionState) {
    /// The session is closed. No commands can be sent to the key.
    YKFNFCConnectionStateClosed,
    
    /// The session is in an intermediary state between closed and opened. Before the tag was discovered.
    /// The application should not send commands to the key when the session is in this state.
    YKFNFCConnectionStatePooling,

    /// The session is opened and ready to use. The application can send immediately commands to the key.
    YKFNFCConnectionStateOpen
};

@protocol YKFNFCConnectionProtocol<NSObject>

typedef void (^OATHSession)(id<YKFKeyOATHSessionProtocol> _Nullable, NSError* _Nullable);

- (void)oathSession:(OATHSession _Nonnull)callback;

/*!
 @property sessionState
 
 @abstract
    This property allows to check and observe the state of the connection with the YubiKey.
 
 NOTE:
    This is a KVO compliant property. Observe it to get updates when the key is connected.
 */
@property (nonatomic, assign, readonly) YKFNFCConnectionState nfcConnectionState;

/*!
 @property sessionError
  @abstract
    This property allows to check errors encountered during NFCSession.
  NOTE:
    The property is available when key gets disconnected, state is closed and session is invalidated
 */
@property (nonatomic, readonly, nullable) NSError *nfcConnectionError;

/*!
 @property tagDescription
 
 @returns
    The description of the discovered tag.
 
 NOTE:
    This property becomes available when the tag is discovered and is nil when the tas is lost.
 */
@property (nonatomic, readonly, nullable) YKFNFCTagDescription *tagDescription API_AVAILABLE(ios(13.0));


/*!
 @property otpService
 
 @abstract
    The shared object to interact with the OTP application from the YubiKey. This property is
    always available and handles its own NFC NDEF session.
 */
@property (nonatomic, readonly) id<YKFNFCOTPServiceProtocol> otpService API_AVAILABLE(ios(11.0));

/*!
 @property u2fService
 
 @abstract
    The shared object to interact with the U2F application from the YubiKey.
    This property becomes available when the key is connected and the session opened and is nil when
    the session is closed. This property should be accessed based on the session state.
 */
@property (nonatomic, readonly, nullable) id<YKFKeyU2FServiceProtocol> u2fService API_AVAILABLE(ios(13.0));

/*!
 @property fido2Service
 
 @abstract
    The shared object to interact with the FIDO2 application from the YubiKey.
    This property becomes available when the key is connected and the session opened and is nil when
    the session is closed. This property should be accessed based on the session state.
 */
@property (nonatomic, readonly, nullable) id<YKFKeyFIDO2SessionProtocol> fido2Service API_AVAILABLE(ios(13.0));

/*!
 @property oathService
 
 @abstract
    The shared object to interact with the OATH application from the YubiKey.
    This property becomes available when the key is connected and the session opened and is nil when
    the session is closed. This property should be accessed based on the session state.
 */
@property (nonatomic, readonly, nullable) id<YKFKeyOATHSessionProtocol> oathService API_AVAILABLE(ios(13.0));

/*!
 @property rawCommandService
 
 @abstract
    The shared object which provides an interface to send raw commands to the YubiKey.
    This property becomes available when the key is connected and the session opened and is nil when
    the session is closed. This property should be accessed based on the session state.
 */
@property (nonatomic, readonly, nullable) id<YKFKeyRawCommandSessionProtocol> rawCommandService API_AVAILABLE(ios(13.0));

/*!
 @method startIso7816Session
 
 @abstract
    To allow the session to connect and interract with the YubiKey, the session needs to be started.
 */
- (void)start API_AVAILABLE(ios(13.0));

/*!
 @method stopIso7816Session
 
 @abstract
    Closes the communication with the key and disables the key connection events.
 */
- (void)stop API_AVAILABLE(ios(13.0));

/*!
 @method cancelCommands
 
 @abstract:
    Cancels all issued commands to the key, which are still in the processing queue but not yet started. This method
    would be usually called when the user wants to cancel an operation in the UI and the application also cancels the
    requests to the key.
 */
- (void)cancelCommands API_AVAILABLE(ios(13.0));

- (void)setAlertMessage:(NSString*) alertMessage API_AVAILABLE(ios(13.0));
@end

@interface YKFNFCConnection: NSObject<YKFNFCConnectionProtocol>
@end

NS_ASSUME_NONNULL_END
