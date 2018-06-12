#ifndef ID3V1_GWD_0814
#define ID3V1_GWD_0814

#pragma pack(1)
// Reverses the order of 2 bytes, like "12" to "21"
#define SWAP_SHORT(x) ( ( ( (x) & 0xFF00 ) >> 8 ) + ( ( (x) & 0x00FF ) << 8 ) )

// Reverses the order of 4 bytes, like "1234" to "4321"
#define SWAP_LONG(x) ( ( ( (x) & 0xFF000000 ) >> 24 ) + ( ( (x) & 0x00FF0000 ) >> 8 ) + ( ( (x) & 0x0000FF00 ) << 8 ) + ( ( (x) & 0x000000FF ) << 24 ) )

// Reverses the order of 8 bytes, like "12345678" to "87654321"
#define SWAP_64(x) ( ( SWAP_LONG( (x) & 0xFFFFFFFF ) << 32 ) | SWAP_LONG( (x) >> 32 ) )

typedef char CHAR;
typedef char BYTE;
typedef unsigned int DWORD;

typedef struct
{
	CHAR	szTag[3];
	CHAR	szSongname[30];
	CHAR	szArtist[30];
	CHAR	szAlbum[30];
	CHAR	szYear[4];
	CHAR	szComment[30];
	BYTE	nGenre;
} ID3V1;

#define ID3V1_TAG	"TAG"
typedef unsigned long long QWORD;

typedef struct
{
	CHAR	szTag[3];
	BYTE	nMajorVersion;
	BYTE	nMinorVersion;
	BYTE	nFlags;
	DWORD	nSize;
} ID3V2_HEADER;

#define ID3V2_TAG			"ID3"

#define ID3V2_UNSYNCHRONISED	0x80
#define ID3V2_EXTENDEDHEADER	0x40
#define ID3V2_EXPERIMENTAL		0x20
#define ID3V2_FOOTERPRESENT		0x10
#define ID3V2_KNOWNMASK			0xF0

typedef struct
{
	DWORD	nSize;
	BYTE	nFlags1;
	BYTE	nFlags2;
	DWORD	nPadding;
} ID3V2_EXTENDED_HEADER_1;

typedef struct
{
	DWORD	nSize;
	BYTE	nFlagBytes;
} ID3V2_EXTENDED_HEADER_2;

typedef struct
{
	CHAR	szID[4];
	DWORD	nSize;
	BYTE	nFlags1;
	BYTE	nFlags2;
} ID3V2_FRAME;

typedef struct
{
	CHAR	szID[3];
	BYTE	nSize[3];
} ID3V2_FRAME_2;

#define ID3V2_KNOWNFRAME	0x4F

#define ID3_DESYNC_SIZE(x)	{	\
	x = ( x & 0x7F ) | ( ( x & ~0xFF ) >> 1 );	\
	x = ( x & 0x03FFF ) | ( ( x & ~0x03FFF ) >> 1 );	\
	x = ( x & 0x01FFFFF ) | ( ( x & ~0x01FFFFF ) >> 1 ); }

typedef struct
{
	char cID[4];							// should equal 'MAC '
	unsigned short nVersion;				// version number * 1000 (3.81 = 3810)
	unsigned short nCompressionLevel;		// the compression level
	unsigned short nFormatFlags;			// any format flags (for future use)
	unsigned short nChannels;				// the number of channels (1 or 2)
	unsigned int nSampleRate;			// the sample rate (typically 44100)
	unsigned int nHeaderBytes;			// the bytes after the MAC header that compose the WAV header
	unsigned int nTerminatingBytes;		// the bytes after that raw data (for extended info)
	unsigned int nTotalFrames;			// the number of frames in the file
	unsigned int nFinalFrameBlocks;		// the number of samples in the final frame
} APE_HEADER;

typedef struct
{
	char cID[8];			// should equal 'APETAGEX'
	int nVersion;			// equals CURRENT_APE_TAG_VERSION
	int nSize;				// the complete size of the tag, including this footer
	int nFields;			// the number of fields in the tag
	int nFlags;				// the tag flags (none currently defined)
	char cReserved[8];		// reserved for later use
} APE_TAG_FOOTER;

typedef struct
{
    DWORD		dwMicroSecPerFrame;	// frame display rate (or 0L)
    DWORD		dwMaxBytesPerSec;	// max. transfer rate
    DWORD		dwPaddingGranularity;	// pad to multiples of this
                                                // size; normally 2K.
    DWORD		dwFlags;		// the ever-present flags
    DWORD		dwTotalFrames;		// # frames in file
    DWORD		dwInitialFrames;
    DWORD		dwStreams;
    DWORD		dwSuggestedBufferSize;

    DWORD		dwWidth;
    DWORD		dwHeight;

    DWORD		dwReserved[4];
} AVI_HEADER;

#define ID3_GENRES	148
#pragma pack()

#endif

