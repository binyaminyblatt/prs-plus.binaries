#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "unicode.h"
#include "LnkParser.h"

#define bool unsigned char
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
// For MSVS not to complain about unsave fopen
#define _CRT_SECURE_NO_DEPRECATE
// To prevent padding in struct
#pragma pack(1) 

struct ShellLinkHeader {
	uint32_t HeaderSize;
	uint32_t LinkCLSID[4];
	uint32_t LinkFlags;
	uint32_t FileAttributes;
	uint32_t CreationTime[2];
	uint32_t AccessTime[2];
	uint32_t WriteTime[2];
	uint32_t FileSize;
	uint32_t IconIndex;
	uint32_t ShowCommand;
	uint16_t HotKey;
	uint16_t Reserved1;
	uint32_t Reserved2;
	uint32_t Reserved3;

};

uint8_t* readString(uint8_t **buf, int isUnicode) {
	uint8_t *name, *result, *pbuf0;
	int utf8Len;
	int i;
	ConversionResult res;
	uint16_t len = (*buf)[0] + (*buf)[1]*256;
	if (isUnicode) {
		len *= 2;
	}
	name = (*buf) + 2;
	*buf += sizeof(uint16_t) + len;
	
	// Say, utf8 can take up to 4 bytes per utf16 byte.
	utf8Len = len * 4;
	pbuf0 = (uint8_t*) malloc(utf8Len + 2);
	if (isUnicode) {
		result = pbuf0;
		res = ConvertUTF16toUTF8((const UTF16**) &name,(const UTF16*) (name + len), (UTF8**) &pbuf0, (UTF8*) pbuf0 + utf8Len, lenientConversion);
		*pbuf0 = 0;
	} else {
		result = (uint8_t *) malloc(len + 1);
		for (i = 0; i < len; i++) {
			result[i] = name[i];
		}
		result[len] = 0; // null terminated str
	}

	return result;
};

struct Lnk* parseLnk(char *filename) {
	int i;
	struct stat st; 
	FILE* f;
	uint8_t *buf, *buf0;
	struct ShellLinkHeader *slh;
	bool hasName, hasRelativePath, hasWorkingDir, hasArguments, hasIconLocation, isUnicode;
	struct Lnk* result;

	result = (struct Lnk*) malloc(sizeof(struct Lnk));
	if (!result) {
		printf("Failed to allocate memory");
		exit(-1);
	}

	// poor man's memset
	for (i = 0; i < sizeof(struct Lnk); i++) {
		((uint8_t*) result) [i] = 0;
	}

	// open file
	f = fopen(filename, "rb");
	if (!f) {
		printf("Failed to open file %s", filename);
		exit(-1);
	}

	// file size
	if (stat(filename, &st)) {
		printf("Failed to get size of file %s", filename);
		exit(-2);
	}

	// allocate buffer
	buf = (uint8_t*) malloc(st.st_size);
	buf0 = buf;
	if (!buf) {
		printf("Failed to allocate memory");
		exit(-3);
	}

	// read file content
	if (fread(buf, 1, st.st_size, f) != st.st_size) {
		printf("Failed to read %s content", filename);
		exit(-4);
	}

	// parse it
	slh = (struct ShellLinkHeader*) buf;
	if (slh->HeaderSize != 0x4c 
			|| slh->LinkCLSID[0] != 0x21401
			|| slh->LinkCLSID[1] != 0x0
			|| slh->LinkCLSID[2] != 0xc0
			|| slh->LinkCLSID[3] != 0x46000000) {

		printf("Invalid header");
		exit(-5);
	}
	buf += 0x4c;

	// SHELL_LINK = SHELL_LINK_HEADER [LINKTARGET_IDLIST] [LINKINFO] [STRING_DATA] *EXTRA_DATA
	// HasLinkTargetIDList? [LINKTARGET_IDLIST]
	if (slh->LinkFlags & 1) {
		// skip over LINKTARGET_IDLIST structure
		buf += *((uint16_t*) buf) + 2;  // IDListSize variable itself + it's content
	}

	// HasLinkInfo? [LINKINFO]
	if (slh->LinkFlags & 2) {
		buf += *((uint32_t*) buf);
	}

	// HasName? 
	hasName = slh->LinkFlags & 4;
	// HasRelativePath
	hasRelativePath = slh->LinkFlags & 8;
	// HasWorkingDir? 
	hasWorkingDir = slh->LinkFlags & 16;
	// HasArguments?
	hasArguments = slh->LinkFlags & 32;
	// HasIconLocation?
	hasIconLocation = slh->LinkFlags & 64;
	// IsUnicode?
	isUnicode = slh->LinkFlags & 128;

	// HasStringData?
	if (hasName || hasRelativePath || hasWorkingDir || hasArguments || hasIconLocation) {
		if (hasName) {
			result->name = readString(&buf, isUnicode);
		}
		if (hasRelativePath) {
			result->relativePath = readString(&buf, isUnicode);
		}
		if (hasWorkingDir) {
			result->workingDir = readString(&buf, isUnicode);
		}
		if (hasArguments) {
			result->arguments = readString(&buf, isUnicode);
		}
		if (hasIconLocation) {
			result->iconLocation = readString(&buf, isUnicode);
		}
	}

	fclose(f);
	free(buf0);
	return result;
}