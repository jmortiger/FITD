#define __FITD_COMPILE_DATETIME__ __TIME__ ", " __DATE__
#define getVersion(string) sprintf(string,"Compiled at %s\n",__FITD_COMPILE_DATETIME__);

/// @brief Directly calls `printf` with the static output of `getVersion`.
#define printVersion() printf("Compiled at %s\n",__FITD_COMPILE_DATETIME__);

