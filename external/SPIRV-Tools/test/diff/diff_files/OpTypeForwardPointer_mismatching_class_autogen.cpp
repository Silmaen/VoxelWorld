// GENERATED FILE - DO NOT EDIT.
// Generated by generate_tests.py
//
// Copyright (c) 2022 Google LLC.
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

#include "../diff_test_utils.h"

#include "gtest/gtest.h"

namespace spvtools {
namespace diff {
namespace {

// Tests that two forwarded type pointers with mismatching storage classes
// aren't matched
constexpr char kSrc[] = R"(               OpCapability Kernel
               OpCapability Addresses
               OpCapability Linkage
               OpMemoryModel Logical OpenCL
               OpName %Aptr "Aptr"
               OpTypeForwardPointer %Aptr UniformConstant
       %uint = OpTypeInt 32 0
          %A = OpTypeStruct %Aptr %uint
  %Aptr = OpTypePointer UniformConstant %A)";
constexpr char kDst[] = R"(               OpCapability Kernel
               OpCapability Addresses
               OpCapability Linkage
               OpMemoryModel Logical OpenCL
               OpName %Aptr "Aptr"
               OpTypeForwardPointer %Aptr Function
       %uint = OpTypeInt 32 0
          %A = OpTypeStruct %Aptr %uint
  %Aptr = OpTypePointer Function %A
)";

TEST(DiffTest, OptypeforwardpointerMismatchingClass) {
  constexpr char kDiff[] = R"( ; SPIR-V
 ; Version: 1.6
 ; Generator: Khronos SPIR-V Tools Assembler; 0
-; Bound: 4
+; Bound: 6
 ; Schema: 0
 OpCapability Kernel
 OpCapability Addresses
 OpCapability Linkage
 OpMemoryModel Logical OpenCL
-OpName %1 "Aptr"
+OpName %4 "Aptr"
-OpTypeForwardPointer %1 UniformConstant
+OpTypeForwardPointer %4 Function
 %2 = OpTypeInt 32 0
-%3 = OpTypeStruct %1 %2
-%1 = OpTypePointer UniformConstant %3
+%5 = OpTypeStruct %4 %2
+%4 = OpTypePointer Function %5
)";
  Options options;
  DoStringDiffTest(kSrc, kDst, kDiff, options);
}

TEST(DiffTest, OptypeforwardpointerMismatchingClassNoDebug) {
  constexpr char kSrcNoDebug[] = R"(               OpCapability Kernel
               OpCapability Addresses
               OpCapability Linkage
               OpMemoryModel Logical OpenCL
               OpTypeForwardPointer %Aptr UniformConstant
       %uint = OpTypeInt 32 0
          %A = OpTypeStruct %Aptr %uint
  %Aptr = OpTypePointer UniformConstant %A
)";
  constexpr char kDstNoDebug[] = R"(               OpCapability Kernel
               OpCapability Addresses
               OpCapability Linkage
               OpMemoryModel Logical OpenCL
               OpTypeForwardPointer %Aptr Function
       %uint = OpTypeInt 32 0
          %A = OpTypeStruct %Aptr %uint
  %Aptr = OpTypePointer Function %A
)";
  constexpr char kDiff[] = R"( ; SPIR-V
 ; Version: 1.6
 ; Generator: Khronos SPIR-V Tools Assembler; 0
-; Bound: 4
+; Bound: 6
 ; Schema: 0
 OpCapability Kernel
 OpCapability Addresses
 OpCapability Linkage
 OpMemoryModel Logical OpenCL
-OpTypeForwardPointer %1 UniformConstant
+OpTypeForwardPointer %4 Function
 %2 = OpTypeInt 32 0
-%3 = OpTypeStruct %1 %2
-%1 = OpTypePointer UniformConstant %3
+%5 = OpTypeStruct %4 %2
+%4 = OpTypePointer Function %5
)";
  Options options;
  DoStringDiffTest(kSrcNoDebug, kDstNoDebug, kDiff, options);
}

}  // namespace
}  // namespace diff
}  // namespace spvtools
