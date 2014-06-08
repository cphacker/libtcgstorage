/////////////////////////////////////////////////////////////////////////////
/// tcgs_packet.c
///
/// API to manipulate TCG packets   
///
/// (c) Artem Zankovich, 2014
//////////////////////////////////////////////////////////////////////////////   

#ifndef _TCGS_PACKET_C
#define _TCGS_PACKET_C

#include "tcgs_types.h"
#include "tcgs_packet.h"

static void sync_subpacket_header(TCGS_SubPacket_t* subpacket)
{
	memset(subpacket->buffer, 0, sizeof(TCGS_SubPacket_Header_t));
	(*((TCGS_SubPacket_Header_t*)subpacket->buffer))->kind = subpacket->kind;
	(*((TCGS_SubPacket_Header_t*)subpacket->buffer))->length = subpacket->length;
}

/*****************************************************************************
 * \brief Initializes SubPacket descriptor
 *
 * \par The function initializes SubPacket descriptor with default values and
 *      payload buffer 
 *
 * @param[inout]	subpacket		Pointer to a subpacket descriptor
 * @param[in]		payloadBuffer	Pointer to a payload buffer to store generated data
 * @param[in]		size			Size of the payload buffer
 *
 * \return TCGS_Error_t with error status
 *
 *****************************************************************************/
TCGS_Error_t TCGS_InitSubPacket(
		TCGS_SubPacket_t* subpacket,
		uint8* payloadBuffer,
		size_t size
)
{
	// Check space for subpacket header
	if (size < sizeof(TCGS_SubPacket_Header_t))
	{
		RETURN_ERROR(ERROR_BUILDER, ERROR_BUILDER_UNSUFFICIENT_BUFFER);
	}

	// Initialize SubPacket structure with default values
	memset(subpacket, 0, sizeof(subpacket));
	subpacket->kind   = SUBPACKET_DATA;
	subpacket->size   = size;
	subpacket->bufer  = payloadBuffer;
	sybpacket->length = sizeof(TCGS_SubPacket_Header_t);

	// Fill payload buffer with zeroes
	memset(payloadBuffer, 0, size);

	// Copy values from descriptor to the buffer
	sync_subpacket_header(subpacket);
	
	return ERROR_SUCCESS;
}



#endif //_TCGS_PACKET_C
