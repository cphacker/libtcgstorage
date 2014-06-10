/////////////////////////////////////////////////////////////////////////////
/// tcgs_types.h
///
/// Common types and definitions of TCG Storage Lib
///
/// (c) Artem Zankovich
////////////////////////////////////////////////////////////////////////////// 

#ifndef _TCGS_TYPES_H
#define _TCGS_TYPES_H

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;
typedef uint64				uid_t;

//#define bool unsigned char
#define FALSE  (0)
#define TRUE   (!FALSE)

//Error types
typedef enum
{
	ERROR_SUCCESS,
	ERROR_BUILDER,
	ERROR_INTERFACE,
	ERROR_TPER,
} TCGS_Error_t;

//Error codes
typedef enum 
{
	ERROR_NONE,
	ERROR_BAD_PARAMETER,				//unsupported parameter of function
	ERROR_OPENING_DEVICE,				//error opening device
	ERROR_DEVICE_NOT_OPENED,			//device need to be opened before
	ERROR_COMMAND_UNSUPPORTED,			//interface command is not supported
	ERROR_PROTOCOL_UNSUPPORTED,			//interface protocol is not supported
	ERROR_IO_COMMAND,					//error sending interface IO command
	ERROR_INVALID_INTERFACE_STATUS,		//device returned invalid status
	ERROR_INVALID_INTERFACE_ERROR,		//device returned invalid error code
	
	ERROR_BUILDER_UNSUFFICIENT_BUFFER,	//no place in the buffer to generate data
} TCGS_ErrorCode_t;


//minimal block size of the storage device
#define TCGS_BLOCK_SIZE 512

/*
 * Store last error code of libtcgstore. Previous error code is overwritten.
 * Use TCGS_GetError to get it
 *
 * \see TCGS_GetError
 */
void TCGS_SetError(TCGS_ErrorCode_t errorCode);

#define RETURN_ERROR(errorCode, errorType) TCGS_SetError(errorCode); return errorType;

#define SIZEOF_ARRAY(array) ((sizeof(array))/(sizeof(array[0])))

#endif //_TCGS_TYPES_H
