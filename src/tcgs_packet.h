/////////////////////////////////////////////////////////////////////////////
/// tcgs_packet.h
///
/// API to manipulate TCG packets   
///
/// (c) Artem Zankovich, 2014
//////////////////////////////////////////////////////////////////////////////   

#ifndef _TCGS_PACKET_H
#define _TCGS_PACKET_H

#include <stddef.h>
#include "tcgs_types.h"

// Level 0 Descriptors
typedef enum
{
	SUBPACKET_DATA				= 0x0000,
	SUBPACKET_CREDIT_CONTROL	= 0x8001,
} TCGS_SubPacket_Type_t;

typedef struct {
	uint8		reserved[6];
	uint16		kind;			//see TCGS_SubPacket_Type_t
	uint32		length;
} __attribute__((packed)) TCGS_SubPacket_Header_t;

typedef struct {
	TCGS_SubPacket_Type_t	kind;		//type of the buffer
	uint32					length;		//current usage of the buffer, bytes
	uint32					size;		//size of the provided buffer, bytes
	uint8*					buffer;		//pointer to a buffer with SubPacket data
} __attribute__((packed)) TCGS_SubPacket_t;

/*****************************************************************************
 * \brief Initializes SubPacket structure
 *
 * \par The function initializes structure that describes SubPacket
 *
 * @param[inout]	subpacket		Pointer to a subpacket descriptor
 * @param[in]		payloadBuffer	Pointer to a payload buffer to store generated data.
 *									Buffer is returned by TCGS_GetSubPacketPayload.
 *									Caller should manage allocation and deletion of
 *									this memory buffer
 * @param[in]		size			Size of the payload buffer
 *
 * \return TCGS_Error_t with error status
 *
 *****************************************************************************/
TCGS_Error_t TCGS_InitSubPacket(
	TCGS_SubPacket_t* subpacket,
	uint8* payloadBuffer,
	size_t size
);

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
);

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
);

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
);

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
);

typedef enum {
	TOKEN_SL	= 0xF0,		// Start List
	TOKEN_EL	= 0xF1,		// End List
	TOKEN_SN	= 0xF2,		// Start Name
	TOKEN_EN	= 0xF3,		// End Name
	TOKEN_CALL	= 0xF8,		// Call
	TOKEN_EOD	= 0xF9,		// End of Data
	TOKEN_EOS	= 0xFA,		// End of session
	TOKEN_ST	= 0xFB,		// Start transaction
	TOKEN_ET	= 0xFC,		// End transaction
	TOKEN_MT	= 0xFF,		// Empty atom
} TCGS_ControlToken_t;

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
);


#endif //_TCGS_PACKET_H  
