#ifndef ESP_LITTLEFS_FLASH_H
#define ESP_LITTLEFS_FLASH_H

#include "../src/littlefs/lfs.h"

#if CONFIG_IDF_TARGET_ESP32
#include "esp32/rom/spi_flash.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/spi_flash.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/spi_flash.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/spi_flash.h"
#elif CONFIG_IDF_TARGET_ESP32H2
#include "esp32h2/rom/spi_flash.h"
#elif CONFIG_IDF_TARGET_ESP8684
#include "esp8684/rom/spi_flash.h"
#elif __has_include("esp32/rom/spi_flash.h")
#include "esp32/rom/spi_flash.h" //IDF 4
#else
#include "rom/spi_flash.h" //IDF 3
#endif

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ESP_LITTLEFS_FLASH_TAG "LFS_FLASH"

#define ESP_LITTLEFS_FLASH_CREATE_CONFIG_DEFAULT() { \
    .partition_label = NULL, \
    .format_on_error = true, \
    .lfs_read_size = 128, \
    .lfs_prog_size = 128, \
    .lfs_cache_size = 512, \
    .lfs_lookahead_size = 128, \
    .lfs_block_cycles = 512 \
}

/**
* Configuration structure for esp_littlefs_flash_create.
* Always initialize this with the ESP_LITTLEFS_FLASH_CREATE_CONFIG_DEFAULT() macro to ensure that all fields are filled with valid values.
*/
typedef struct {
    /**
     * Label of the partition to use.
     */
    const char *partition_label;
    bool format_on_error;
    /**
     * 4096 must be a multiple of this value.
     */
    lfs_size_t lfs_read_size;
    /**
     * 4096 must be a multiple of this value.
     */
    lfs_size_t lfs_prog_size;
    /**
     * Size of block caches. Each cache buffers a portion of a block in RAM.
     * The littlefs needs a read cache, a program cache, and one additional
     * cache per file. Larger caches can improve performance by storing more
     * data and reducing the number of disk accesses. Must be a multiple of
     * the read and program sizes, and a factor of the block size (4096).
     */
    lfs_size_t lfs_cache_size;
    /**
     * Must be a multiple of 8.
     */
    lfs_size_t lfs_lookahead_size;
    /**
     * Number of erase cycles before littlefs evicts metadata logs and moves
     * the metadata to another block. Suggested values are in the
     * range 100-1000, with large values having better performance at the cost
     * of less consistent wear distribution.
     * Set to -1 to disable block-level wear-leveling.
     */
    int32_t lfs_block_cycles;
} esp_littlefs_flash_create_conf_t;

/**
 * @param[out] lfs The newly created little fs.
 * @return ESP_OK on success.
 */
esp_err_t esp_littlefs_flash_create(lfs_t **lfs, const esp_littlefs_flash_create_conf_t *conf);

esp_err_t esp_littlefs_flash_delete(lfs_t **lfs);

/**
 * @brief erase a partition; make sure LittleFS is unmounted first.
 *
 * @param partition_label  Label of the partition to format.
 * @return
 *          - ESP_OK      if successful
 *          - ESP_FAIL    on error
 */
esp_err_t esp_littlefs_flash_erase(const char *partition_label);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //ESP_LITTLEFS_FLASH_H
