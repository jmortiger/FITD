#define getVersion(string) sprintf(string,"Compiled the %s at %s\n",__DATE__,__TIME__);

/// @brief Directly calls `printf` with the static output of `getVersion`.
#define printVersion() printf("Compiled the %s at %s\n",__DATE__,__TIME__);

