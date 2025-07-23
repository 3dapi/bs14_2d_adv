//-----------------------------------------------------------------------------
// File: D3DSettings.cpp
//
// Desc: Settings class and change-settings dialog class for the Direct3D 
//       samples framework library.
//-----------------------------------------------------------------------------
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include <D3D9.h>
#include "DXUtil.h"
#include "D3DUtil.h"
#include "D3DEnumeration.h"
#include "D3DSettings.h"
#include "resource.h"


TCHAR* D3DDevTypeToString(D3DDEVTYPE devType)
{
    switch (devType)
    {
    case D3DDEVTYPE_HAL:        return TEXT("D3DDEVTYPE_HAL");
    case D3DDEVTYPE_SW:         return TEXT("D3DDEVTYPE_SW");
    case D3DDEVTYPE_REF:        return TEXT("D3DDEVTYPE_REF");
    default:                    return TEXT("Unknown devType");
    }
}


//-----------------------------------------------------------------------------
// Name: MultisampleTypeToString
// Desc: Returns the string for the given D3DMULTISAMPLE_TYPE.
//-----------------------------------------------------------------------------
TCHAR* MultisampleTypeToString(D3DMULTISAMPLE_TYPE MultiSampleType)
{
    switch (MultiSampleType)
    {
    case D3DMULTISAMPLE_NONE:   return TEXT("D3DMULTISAMPLE_NONE");
    case D3DMULTISAMPLE_NONMASKABLE: return TEXT("D3DMULTISAMPLE_NONMASKABLE");
    case D3DMULTISAMPLE_2_SAMPLES: return TEXT("D3DMULTISAMPLE_2_SAMPLES");
    case D3DMULTISAMPLE_3_SAMPLES: return TEXT("D3DMULTISAMPLE_3_SAMPLES");
    case D3DMULTISAMPLE_4_SAMPLES: return TEXT("D3DMULTISAMPLE_4_SAMPLES");
    case D3DMULTISAMPLE_5_SAMPLES: return TEXT("D3DMULTISAMPLE_5_SAMPLES");
    case D3DMULTISAMPLE_6_SAMPLES: return TEXT("D3DMULTISAMPLE_6_SAMPLES");
    case D3DMULTISAMPLE_7_SAMPLES: return TEXT("D3DMULTISAMPLE_7_SAMPLES");
    case D3DMULTISAMPLE_8_SAMPLES: return TEXT("D3DMULTISAMPLE_8_SAMPLES");
    case D3DMULTISAMPLE_9_SAMPLES: return TEXT("D3DMULTISAMPLE_9_SAMPLES");
    case D3DMULTISAMPLE_10_SAMPLES: return TEXT("D3DMULTISAMPLE_10_SAMPLES");
    case D3DMULTISAMPLE_11_SAMPLES: return TEXT("D3DMULTISAMPLE_11_SAMPLES");
    case D3DMULTISAMPLE_12_SAMPLES: return TEXT("D3DMULTISAMPLE_12_SAMPLES");
    case D3DMULTISAMPLE_13_SAMPLES: return TEXT("D3DMULTISAMPLE_13_SAMPLES");
    case D3DMULTISAMPLE_14_SAMPLES: return TEXT("D3DMULTISAMPLE_14_SAMPLES");
    case D3DMULTISAMPLE_15_SAMPLES: return TEXT("D3DMULTISAMPLE_15_SAMPLES");
    case D3DMULTISAMPLE_16_SAMPLES: return TEXT("D3DMULTISAMPLE_16_SAMPLES");
    default:                    return TEXT("Unknown Multisample Type");
    }
}


//-----------------------------------------------------------------------------
// Name: VertexProcessingTypeToString
// Desc: Returns the string for the given VertexProcessingType.
//-----------------------------------------------------------------------------
TCHAR* VertexProcessingTypeToString(VertexProcessingType vpt)
{
    switch (vpt)
    {
    case SOFTWARE_VP:      return TEXT("SOFTWARE_VP");
    case MIXED_VP:         return TEXT("MIXED_VP");
    case HARDWARE_VP:      return TEXT("HARDWARE_VP");
    case PURE_HARDWARE_VP: return TEXT("PURE_HARDWARE_VP");
    default:               return TEXT("Unknown VertexProcessingType");
    }
}


//-----------------------------------------------------------------------------
// Name: PresentIntervalToString
// Desc: Returns the string for the given present interval.
//-----------------------------------------------------------------------------
TCHAR* PresentIntervalToString( UINT pi )
{
    switch( pi )
    {
    case D3DPRESENT_INTERVAL_IMMEDIATE: return TEXT("D3DPRESENT_INTERVAL_IMMEDIATE");
    case D3DPRESENT_INTERVAL_DEFAULT:   return TEXT("D3DPRESENT_INTERVAL_DEFAULT");
    case D3DPRESENT_INTERVAL_ONE:       return TEXT("D3DPRESENT_INTERVAL_ONE");
    case D3DPRESENT_INTERVAL_TWO:       return TEXT("D3DPRESENT_INTERVAL_TWO");
    case D3DPRESENT_INTERVAL_THREE:     return TEXT("D3DPRESENT_INTERVAL_THREE");
    case D3DPRESENT_INTERVAL_FOUR:      return TEXT("D3DPRESENT_INTERVAL_FOUR");
    default:                            return TEXT("Unknown PresentInterval");
    }
}