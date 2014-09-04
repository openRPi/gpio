#ifndef __qtft_spi__
#define __qtft_spi__

extern int qtft_spi_init(void);
extern void qtft_spi_exit(void);

extern int qtft_spi_write(const void *buf, size_t len);
extern int qtft_spi_read(void *buf, size_t len);

#endif 