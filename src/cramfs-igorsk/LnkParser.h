/* This is for C++ and does no harm in C */
#ifdef __cplusplus
extern "C" {
#endif

struct Lnk {
	char* name;
	char* relativePath;
	char* workingDir;
	char* iconLocation;
	char* arguments;
};

struct Lnk* parseLnk(char *filename);


#ifdef __cplusplus
}
#endif
