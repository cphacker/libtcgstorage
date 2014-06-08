/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_encode.c
///
/// Encode/decodes data sent/received from transport interface
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "tcgs_config.h"
#include "tcgs_types.h"
#include "tcgs_level0discovery.h"
#include "tcgs_interface.h"
#include "tcgs_interface_encode.h"
#include "tcgs_verbose.h"


TCGS_Level0Discovery_Header_t *TCGS_DecodeLevel0Discovery (void* data)
{
	TCGS_Level0Discovery_Feature_t *iter;
	#define header ((TCGS_Level0Discovery_Header_t*)data)

	if (data != NULL)
	{
		header->length = _swap32(header->length);
		header->versionMajor = _swap16(header->versionMajor);
		header->versionMinor = _swap16(header->versionMinor);
	}
	iter = TCGS_GetLevel0DiscoveryFirstFeatureHeader(header);
	while (iter != NULL)
	{
		iter->code = _swap16(iter->code);
		switch (iter->code)
		{
		case FEATURE_TPER:
			//no decoding required
			break;
		case FEATURE_LOCKING:
			//no decoding required
			break;
		case FEATURE_OPAL1:
			((TCGS_Level0Discovery_FeatureOpal1_t*)iter)->baseComID =
					_swap16(((TCGS_Level0Discovery_FeatureOpal1_t*)iter)->baseComID);
			((TCGS_Level0Discovery_FeatureOpal1_t*)iter)->numberOfComIDs =
					_swap16(((TCGS_Level0Discovery_FeatureOpal1_t*)iter)->numberOfComIDs);
			break;
		//TODO: add decoding for other features
		}
		iter = TCGS_GetLevel0DiscoveryNextFeatureHeader(header, iter);
	}

	return header;
}
