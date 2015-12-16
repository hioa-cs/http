// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2015-2016 Oslo and Akershus University College of Applied Sciences
// and Alfred Bratterud
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef HTTP_STATUS_CODE_CONSTANTS_HPP
#define HTTP_STATUS_CODE_CONSTANTS_HPP

//< 1xx: Informational - Request received, continuing process
#define Continue            100
#define Switching_Protocols 101
#define Processing          102

//< 2xx: Success - The action was successfully received, understood, and accepted
#define OK                  200
#define Created             201
#define Accepted            202
#define Non_Authoritative   203
#define No_Content          204
#define Reset_Content       205
#define Partial_Content     206
#define Multi_Status        207
#define Already_Reported    208
#define IM_Used             226

//< 3xx: Redirection - Further action must be taken in order to complete the request
#define  Multiple_Choices   300
#define  Moved_Permanently  301
#define  Found              302
#define  See_Other          303
#define  Not_Modified       304
#define  Use_Proxy          305
#define  Temporary_Redirect 307
#define  Permanent_Redirect 308

//< 4xx: Client Error - The request contains bad syntax or cannot be fulfilled
#define Bad_Request                     400
#define Unauthorized                    401
#define Payment_Required                402
#define Forbidden                       403
#define Not_Found                       404
#define Method_Not_Allowed              405
#define Not_Acceptable                  406
#define Proxy_Authentication_Required   407
#define Request_Timeout                 408
#define Conflict                        409
#define Gone                            410
#define Length_Required                 411
#define Precondition_Failed             412
#define Payload_Too_Large               413
#define URI_Too_Long                    414
#define Unsupported_Media_Type          415
#define Range_Not_Satisfiable           416
#define Expectation_Failed              417
#define Misdirected_Request             421
#define Unprocessable_Entity            422
#define Locked                          423
#define Failed_Dependency               424
#define Upgrade_Required                426
#define Precondition_Required           428
#define Too_Many_Requests               429
#define Request_Header_Fields_Too_Large 431

//< 5xx: Server Error - The server failed to fulfill an apparently valid request
#define Internal_Server_Error           500
#define Not_Implemented                 501
#define Bad_Gateway                     502
#define Service_Unavailable             503
#define Gateway_Timeout                 504
#define HTTP_Version_Not_Supported      505
#define Variant_Also_Negotiates         506
#define Insufficient_Storage            507
#define Loop_Detected                   508
#define Not_Extended                    510
#define Network_Authentication_Required 511

#endif //< HTTP_STATUS_CODE_CONSTANTS_HPP
