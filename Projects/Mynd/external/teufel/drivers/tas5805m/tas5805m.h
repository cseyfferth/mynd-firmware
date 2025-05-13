#pragma once

#include <stdint.h>
#include <stdbool.h>

#define E_TAS5805M_OK    0
#define E_TAS5805M_IO    1 // I/O operation failed
#define E_TAS5805M_PARAM 2 // Invalid param

typedef unsigned char cfg_u8;
typedef union
{
    struct
    {
        cfg_u8 offset;
        cfg_u8 value;
    };
    struct
    {
        cfg_u8 command;
        cfg_u8 param;
    };
} tas5805m_cfg_reg_t;

#define CFG_META_SWITCH (255)
#define CFG_META_DELAY  (254)
#define CFG_META_BURST  (253)

typedef int (*tas5805m_i2c_read_fn_t)(uint8_t i2c_address, uint8_t register_address, uint8_t *p_data, uint32_t length);
typedef int (*tas5805m_i2c_write_fn_t)(uint8_t i2c_address, uint8_t register_address, const uint8_t *p_data,
                                       uint32_t length);
typedef void (*tas5805m_delay_fn_t)(uint32_t ms);

typedef struct tas5805m_handler tas5805m_handler_t;

typedef struct
{
    tas5805m_i2c_read_fn_t  i2c_read_fn;
    tas5805m_i2c_write_fn_t i2c_write_fn;
    tas5805m_delay_fn_t     delay_fn;
    uint8_t                 i2c_device_address;
} tas5805m_config_t;

typedef enum
{
    TAS5805M_DEVICE_STATE_DEEP_SLEEP,
    TAS5805M_DEVICE_STATE_SLEEP,
    TAS5805M_DEVICE_STATE_HI_Z,
    TAS5805M_DEVICE_STATE_PLAY,
} tas5805m_device_state_t;

/**
 * @brief Initializes the TAS5805M driver.
 *
 * @param[in] p_config          pointer to configuration structure
 *
 * @return pointer to handler if successful, NULL otherwise
 */
tas5805m_handler_t *tas5805m_init(const tas5805m_config_t *p_config);

/**
 * @brief Loads the configuration for the TAS5805M amplifier.
 *
 * @param[in] h                     pointer to handler
 * @param[in] p_tasxxx_config       pointer to configuration array
 * @param[in] config_length         length of configuration array
 *
 * @return 0 if successful, error code otherwise
 */
int tas5805m_load_configuration(const tas5805m_handler_t *h, const tas5805m_cfg_reg_t *p_tasxxx_config,
                                uint32_t config_length);

/**
 * @brief Enables/disables the DSP in the amplifier.
 *
 * @details The DSP needs to be enabled only after  all input clocks are
 *          settled so that DMA channels do not go out of sync.
 *
 * @param[in] h             pointer to handler
 * @param[in] enable        enable option
 *
 * @return 0 if successful, error code otherwise
 */
int tas5805m_enable_dsp(const tas5805m_handler_t *h, bool enable);

/**
 * @brief Enables/disables the EQ in the amplifier's DSP.
 *
 * @param[in] h             pointer to handler
 * @param[in] enable        enable option
 *
 * @return 0 if successful, error code otherwise
 */
int tas5805m_enable_eq(const tas5805m_handler_t *h, bool enable);

/**
 * @brief Enables/disables the DRC in the amplifier's DSP.
 *
 * @param[in] h             pointer to handler
 * @param[in] enable        enable option
 *
 * @return 0 if successful, error code otherwise
 */
int tas5805m_enable_drc(const tas5805m_handler_t *h, bool enable);

/**
 * @brief Enables/disables the mute control for both left and right channels.
 *
 * @param[in] h             pointer to handler
 * @param[in] enable        enable option
 *
 * @return 0 if successful, error code otherwise
 */
int tas5805m_mute(const tas5805m_handler_t *h, bool enable);

/**
 * @brief Sets the device to the given state.
 *
 * @param[in] h             pointer to handler
 * @param[in] state         device state
 *
 * @return 0 if successful, error code otherwise
 */
int tas5805m_set_state(const tas5805m_handler_t *h, tas5805m_device_state_t state);

/**
 * @brief Sets the digital volume control for both left and right channels.
 *
 * @details The volume range goes from -90 dB to +10 dB.
 *          Anything less than -90 dB gets written as -infinite dB.
 *
 * @param[in] h             pointer to handler
 * @param[in] volume_db     volume in dB
 *
 * @return 0 if successful, error code otherwise
 */
int tas5805m_set_volume(const tas5805m_handler_t *h, int8_t volume_db);
/**
 * @brief Clears any analog faults in the device.
 *
 * @param[in] h             pointer to handler
 *
 * @return 0 if successful, error code otherwise
 */
int tas5805m_clear_analog_fault(const tas5805m_handler_t *h);
