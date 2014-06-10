/////////////////////////////////////////////////////////////////////////////
/// tcgs_packet.c
///
/// API to manipulate TCG packets   
///
/// (c) Artem Zankovich, 2014
//////////////////////////////////////////////////////////////////////////////   

#ifndef _TCGS_PACKET_C
#define _TCGS_PACKET_C

#include <string.h>
#include <stdbool.h>
#include "tcgs_types.h"
#include "tcgs_packet.h"

static void sync_subpacket_header(TCGS_SubPacket_t* subpacket)
{
	memset(subpacket->buffer, 0, sizeof(TCGS_SubPacket_Header_t));
	((TCGS_SubPacket_Header_t*)(subpacket->buffer))->kind = subpacket->kind;
	((TCGS_SubPacket_Header_t*)(subpacket->buffer))->length = subpacket->length;
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
	subpacket->kind		= SUBPACKET_DATA;
	subpacket->size		= size;
	subpacket->buffer	= payloadBuffer;
	subpacket->length	= sizeof(TCGS_SubPacket_Header_t);

	// Fill payload buffer with zeroes
	memset(payloadBuffer, 0, size);

	// Copy values from descriptor to the buffer
	sync_subpacket_header(subpacket);
	
	return ERROR_SUCCESS;
}


/*****************************************************************************
 * \brief Get complete in SubPacket structure
 *
 * \par The function initializes structure that describes SubPacket
 *
 * @param[inout]	subpacket		Pointer to a subpacket descriptor
 *
 * \return pointer to a memory buffer filled with payload content
 *
 *****************************************************************************/
uint8* TCGS_GetSubPacketPayload(
	TCGS_SubPacket_t* subpacket
)
{
	// Copy values from descriptor to the buffer
	sync_subpacket_header(subpacket);

	return subpacket->buffer;
}

typedef struct
{
    uint8   flag    : 1;
    uint8   sign    : 1;
    uint8   data    : 6;
} TCGS_TinyAtom_t;             // 1 byte header + data

typedef struct
{
    uint8   flag    : 2;
    uint8   byte    : 1;
    uint8   sign    : 1;
    uint8   length  : 4;
} TCGS_ShortAtom_t;             // 1 byte header (up to 16 bytes of data)

typedef struct
{
    uint16  flag    : 3;
    uint16  byte    : 1;
    uint16  sign    : 1;
    uint16  length  : 11;
} TCGS_MediumAtom_t;            // 2 bytes header (up to 2,048 bytes of data)

typedef struct
{
    uint32  flag    : 4;
    uint32  reserved: 2;
    uint32  byte    : 1;
    uint32  sign    : 1;
    uint32  length  : 24;
} TCGS_LongAtom_t;              // 4 bytes header (up to 16,777,216 bytes of data)

typedef union
{
    TCGS_TinyAtom_t		tinyAtom;
    TCGS_ShortAtom_t	shortAtom;
    TCGS_MediumAtom_t	mediumAtom;
    TCGS_LongAtom_t		longAtom;    
} TCGS_TokenHeader_t;

#define ATOM_HEADER_TINY_FLAG   (0x00)
#define ATOM_HEADER_SHORT_FLAG  (0x02)
#define ATOM_HEADER_MEDIUM_FLAG (0x06)
#define ATOM_HEADER_LONG_FLAG   (0x0E)

#define ATOM_HEADER_SIGNED_FLAG (0x01)
#define ATOM_HEADER_BYTES_FLAG  (0x01)

// check for free space in the SubPacket buffer
static bool check_size(TCGS_SubPacket_t* subpacket, unsigned size)
{
    return subpacket->length + size <= subpacket->size;
}

/*****************************************************************************
 * \brief Encode unsigned integer
 *
 * \par The function encodes value of unsigned integer type in TCG stream format
 *      and places it to the provided SubPacked buffer. Buffer boundary is checked
 *      and an error is returned is there is no free space
 *
 * @param[in]	i		unsigned integer value to encode
 *
 * \return TCGS_Error_t with error status
 *
 *****************************************************************************/
TCGS_Error_t TCGS_EncodeUnsigned(
    TCGS_SubPacket_t* subpacket,
    uint32	i
)
{    
    if (i < 64)
    {
        if (!check_size(subpacket, 1))
        {
            RETURN_ERROR(ERROR_BUILDER, ERROR_BUILDER_UNSUFFICIENT_BUFFER);
        }
        ((TCGS_TinyAtom_t*)(subpacket->buffer + subpacket->length))->data = i;
        subpacket->length += 1;
    }
    else
    {
        unsigned length;

        length = get_unsigned_size(i);
        if (!check_size(subpacket, length))
        {
            RETURN_ERROR(ERROR_BUILDER, ERROR_BUILDER_UNSUFFICIENT_BUFFER);
        }        
        ((TCGS_ShortAtom_t*)(subpacket->buffer + subpacket->length))->flag = ATOM_HEADER_SHORT_FLAG;
        ((TCGS_ShortAtom_t*)(subpacket->buffer + subpacket->length))->length = length;
        subpacket->length += length;
    }
    
    
    return ERROR_SUCCESS;
}

/*****************************************************************************
 * \brief Encode UID
 *
 * \par The function encodes value of UID type in TCG stream format
 *      and places it to the provided SubPacked buffer. Buffer boundary is checked
 *      and an error is returned is there is no free space
 *
 * @param[inout]	subpacket		subpacket to place encoded value
 * @param[in]		uid				uid to encode
 *
 * \return TCGS_Error_t with error status
 *
 *****************************************************************************/
TCGS_Error_t TCGS_EncodeUid(
    TCGS_SubPacket_t* subpacket,
    uid_t	uid
)
{
	#error Implement me
}
/*****************************************************************************
 * \brief Encode byte sequence
 *
 * \par The function encodes byte sequence in TCG stream format and places it
 *      to the provided SubPacked buffer. Buffer boundary is checked and
 *      an error is returned is there is no free space
 *
 * @param[in]	p		pointer to the byte sequence
 * @param[in]	size	size of the byte sequence
 *
 * \return TCGS_Error_t with error status
 *
 *****************************************************************************/
TCGS_Error_t TCGS_EncodeBytes(
    TCGS_SubPacket_t* subpacket,
	uint8*	p,
	size_t	size
)
{
    return ERROR_SUCCESS;
}

/*****************************************************************************
 * \brief Encode TCG control sequence
 *
 * \par The function encodes TCG control token and places it
 *      to the provided SubPacked buffer. Buffer boundary is checked and
 *      an error is returned is there is no free space
 *
 * @param[in]	c		control token
 *
 * \return TCGS_Error_t with error status
 *
 *****************************************************************************/
TCGS_Error_t TCGS_EncodeControl(
    TCGS_SubPacket_t* subpacket,
	TCGS_ControlToken_t	c
)
{
    if (!check_size(subpacket, 1))
    {
        RETURN_ERROR(ERROR_BUILDER, ERROR_BUILDER_UNSUFFICIENT_BUFFER);
    }
    *((uint8*)(subpacket->buffer + subpacket->length)) = (uint8)c;
    subpacket->length += 1;

    return ERROR_SUCCESS;
}

#endif //_TCGS_PACKET_C
