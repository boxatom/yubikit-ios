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

#import "YKFAPDU.h"
#import "YKFCommandConfiguration.h"

NS_ASSUME_NONNULL_BEGIN

typedef void (^YKFConnectionControllerCommandResponseBlock)(NSData* _Nullable, NSError* _Nullable, NSTimeInterval);
typedef void (^YKFConnectionControllerCompletionBlock)(void);

@protocol YKFConnectionControllerProtocol

- (void)execute:(YKFAPDU *)command completion:(YKFConnectionControllerCommandResponseBlock)completion;
- (void)execute:(YKFAPDU *)command configuration:(YKFCommandConfiguration *)configuration completion:(YKFConnectionControllerCommandResponseBlock)completion;

- (void)dispatchOnSequentialQueue:(YKFConnectionControllerCompletionBlock)block delay:(NSTimeInterval)delay;
- (void)dispatchOnSequentialQueue:(YKFConnectionControllerCompletionBlock)block;

- (void)closeConnectionWithCompletion:(YKFConnectionControllerCompletionBlock)completion;
- (void)cancelAllCommands;

@end

NS_ASSUME_NONNULL_END
