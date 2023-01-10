#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct _RESERVED_IDENTIFIER_FIXUP_1365_18812
{
    float3x4 _RESERVED_IDENTIFIER_FIXUP_m0;
    float3x4 _RESERVED_IDENTIFIER_FIXUP_m1;
};

struct _RESERVED_IDENTIFIER_FIXUP_1126_22044
{
    float4x4 _RESERVED_IDENTIFIER_FIXUP_m0;
    float4x4 _RESERVED_IDENTIFIER_FIXUP_m1;
    float _RESERVED_IDENTIFIER_FIXUP_m9;
    char _m3_pad[12];
    packed_float3 _RESERVED_IDENTIFIER_FIXUP_m10;
    float _RESERVED_IDENTIFIER_FIXUP_m11;
    packed_float3 _RESERVED_IDENTIFIER_FIXUP_m12;
    float _RESERVED_IDENTIFIER_FIXUP_m17;
    float _RESERVED_IDENTIFIER_FIXUP_m18;
    float _RESERVED_IDENTIFIER_FIXUP_m19;
    float2 _RESERVED_IDENTIFIER_FIXUP_m20;
};

struct main0_out
{
    float3 _RESERVED_IDENTIFIER_FIXUP_3976 [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct main0_in
{
    float4 _RESERVED_IDENTIFIER_FIXUP_5275 [[attribute(0)]];
};

vertex main0_out main0(main0_in in [[stage_in]], constant _RESERVED_IDENTIFIER_FIXUP_1365_18812& _RESERVED_IDENTIFIER_FIXUP_18812 [[buffer(0)]], constant _RESERVED_IDENTIFIER_FIXUP_1126_22044& _RESERVED_IDENTIFIER_FIXUP_22044 [[buffer(1)]])
{
    main0_out out = {};
    float3 _RESERVED_IDENTIFIER_FIXUP_2;
    float3 _RESERVED_IDENTIFIER_FIXUP_23783;
    for (;;)
    {
        _RESERVED_IDENTIFIER_FIXUP_23783 = fast::normalize(float4(in._RESERVED_IDENTIFIER_FIXUP_5275.xyz, 0.0) * _RESERVED_IDENTIFIER_FIXUP_18812._RESERVED_IDENTIFIER_FIXUP_m1);
        break;
    }
    float4 _RESERVED_IDENTIFIER_FIXUP_14995 = _RESERVED_IDENTIFIER_FIXUP_22044._RESERVED_IDENTIFIER_FIXUP_m0 * float4(float3(_RESERVED_IDENTIFIER_FIXUP_22044._RESERVED_IDENTIFIER_FIXUP_m10) + (in._RESERVED_IDENTIFIER_FIXUP_5275.xyz * (_RESERVED_IDENTIFIER_FIXUP_22044._RESERVED_IDENTIFIER_FIXUP_m17 + _RESERVED_IDENTIFIER_FIXUP_22044._RESERVED_IDENTIFIER_FIXUP_m18)), 1.0);
    out._RESERVED_IDENTIFIER_FIXUP_3976 = _RESERVED_IDENTIFIER_FIXUP_23783;
    float4 _RESERVED_IDENTIFIER_FIXUP_6282 = _RESERVED_IDENTIFIER_FIXUP_14995;
    _RESERVED_IDENTIFIER_FIXUP_6282.y = -_RESERVED_IDENTIFIER_FIXUP_14995.y;
    out.gl_Position = _RESERVED_IDENTIFIER_FIXUP_6282;
    return out;
}

