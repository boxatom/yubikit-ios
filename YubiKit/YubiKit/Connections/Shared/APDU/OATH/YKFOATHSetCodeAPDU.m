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

#import "YKFOATHSetCodeAPDU.h"
#import "YKFAPDUCommandInstruction.h"
#import "YKFOATHCredential.h"
#import "YKFAssert.h"
#import "YKFNSMutableDataAdditions.h"
#import "YKFNSDataAdditions+Private.h"

static const UInt8 YKFKeyOATHSetCodeAPDUKeyTag = 0x73;
static const UInt8 YKFKeyOATHSetCodeAPDUChallengeTag = 0x74;
static const UInt8 YKFKeyOATHSetCodeAPDUResponseTag = 0x75;

@implementation YKFOATHSetCodeAPDU

- (instancetype)initWithCode:(NSString *)code salt:(NSData *)salt {
    YKFAssertAbortInit(code);
    YKFAssertAbortInit(salt.length);
    
    NSMutableData *data = [[NSMutableData alloc] init];
    
    if (code.length) {
        // Set password
        NSData *keyData = [[code dataUsingEncoding:NSUTF8StringEncoding] ykf_deriveOATHKeyWithSalt:salt];
        UInt8 algorithm = YKFOATHCredentialTypeTOTP | YKFOATHCredentialAlgorithmSHA1;
        
        [data ykf_appendEntryWithTag:YKFKeyOATHSetCodeAPDUKeyTag headerBytes:@[@(algorithm)] data:keyData];
        
        // Challenge
        
        UInt8 challengeBuffer[8];
        arc4random_buf(challengeBuffer, 8);
        NSData *challenge = [NSData dataWithBytes:challengeBuffer length:8];
        [data ykf_appendEntryWithTag:YKFKeyOATHSetCodeAPDUChallengeTag data:challenge];
        
        // Response
        
        NSData *response = [challenge ykf_oathHMACWithKey:keyData];
        [data ykf_appendEntryWithTag:YKFKeyOATHSetCodeAPDUResponseTag data:response];
    } else {
        // Remove password
        [data ykf_appendByte:YKFKeyOATHSetCodeAPDUKeyTag];
        [data ykf_appendByte:0x00];
    }
        
    return [super initWithCla:0 ins:0x03 p1:0 p2:0 data:data type:YKFAPDUTypeShort];
}

@end
