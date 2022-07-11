




#ifndef RH_SDIO_H
#define RH_SDIO_H



int rh_sdio__init  (void);
int rh_sdio__mkdir (const char* __p  );
int rh_sdio__touch (const char* __pf );
int rh_sdio__echo  (const char* __pf, const void* __data, size_t __n, const char* opt);
int rh_sdio__query (const char* __pf, size_t * __len);
int rh_sdio__load  (const char* __pf, char   * __buf, size_t __len);

int rh_sdio__deinit(void);

#endif



