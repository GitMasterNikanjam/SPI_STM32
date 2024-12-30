#pragma once

// ##############################################################################################
// MCU Select:

#include "mcu_select.h"

/*
    If there is not exist mcu_select.h at beside of this header file, Create it and put this bellow following content. 
    Then select your desired MCU that want work with.
*/
// ----------------------------------------------------------------
// mcu_select.h file:

// Define the target MCU family here
// Uncomment the desired MCU family definition below:

// #define STM32F1
// #define STM32F4
// #define STM32H7

// ----------------------------------------------------------------

// ###############################################################################################
// Include libraries:

#if defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#endif

#include <string>

// ###############################################################################################
// Define Public Macros:

// Define SPI Modes
#define SPI_MODE0 ((SPI_POLARITY_LOW << 1) | SPI_PHASE_1EDGE)   // CPOL=0, CPHA=0
#define SPI_MODE1 ((SPI_POLARITY_LOW << 1) | SPI_PHASE_2EDGE)   // CPOL=0, CPHA=1
#define SPI_MODE2 ((SPI_POLARITY_HIGH << 1) | SPI_PHASE_1EDGE)  // CPOL=1, CPHA=0
#define SPI_MODE3 ((SPI_POLARITY_HIGH << 1) | SPI_PHASE_2EDGE)  // CPOL=1, CPHA=1

// SPI Clock Divider settings (approximation, adjust for actual board/system clock)
#define SPI_CLOCK_DIV2 SPI_BAUDRATEPRESCALER_2
#define SPI_CLOCK_DIV4 SPI_BAUDRATEPRESCALER_4
#define SPI_CLOCK_DIV8 SPI_BAUDRATEPRESCALER_8
#define SPI_CLOCK_DIV16 SPI_BAUDRATEPRESCALER_16
#define SPI_CLOCK_DIV32 SPI_BAUDRATEPRESCALER_32
#define SPI_CLOCK_DIV64 SPI_BAUDRATEPRESCALER_64
#define SPI_CLOCK_DIV128 SPI_BAUDRATEPRESCALER_128
#define SPI_CLOCK_DIV256 SPI_BAUDRATEPRESCALER_256

// ################################################################################################
// Global Classes:

/**
 * @class SPISettings
 */
class SPISettings 
{
public:

    /**
     * @brief Constructor. Init parameters.
     * @param clockFrequency: SPI Clock Frequency. [Hz]
     * @param bitOrder: SPI First Bit (SPI_FIRSTBIT_MSB/SPI_FIRSTBIT_LSB)
     * @param dataMode: SPI Mode (SPI_MODE0, SPI_MODE1, etc.)
     */
    SPISettings(uint32_t clockFrequency, uint32_t bitOrder, uint32_t dataMode)
        : clockFrequency(clockFrequency), bitOrder(bitOrder), dataMode(dataMode) {}

    /**
     * @brief Default constructor. Init parameters by Default value.
     */
    SPISettings()
        : clockFrequency(1000000), bitOrder(SPI_FIRSTBIT_MSB), dataMode(SPI_MODE0) {}

    /**
     * @brief Get SPI Clock Frequency
     */
    uint32_t getClockFrequency() const { return clockFrequency; }

    /**
     * @brief Get SPI First Bit (SPI_FIRSTBIT_MSB/SPI_FIRSTBIT_LSB)
     */
    uint32_t getBitOrder() const { return bitOrder; }

    /**
     * @brief Get SPI Mode (SPI_MODE0, SPI_MODE1, etc.)
     */
    uint32_t getDataMode() const { return dataMode; }

    /**
     * @brief Set SPI Clock Frequency
     */
    bool setClockFrequency(uint32_t data) 
    {
        uint32_t pclk = HAL_RCC_GetPCLK2Freq(); // Adjust as needed for the actual SPI bus
        if (data >= pclk)
        {
            return false;
        }

        clockFrequency = data;

        return true;
    }

    /**
     * @brief Set SPI First Bit (SPI_FIRSTBIT_MSB/SPI_FIRSTBIT_LSB)
     */
    bool setBitOrder(uint32_t data)
    {
        if( !((bitOrder == SPI_FIRSTBIT_LSB) || (bitOrder == SPI_FIRSTBIT_MSB)) )
        {
            return false;
        }
        bitOrder = data;

        return true;
    }

    /**
     * @brief Set SPI Mode (SPI_MODE0, SPI_MODE1, etc.)
     * @return true if succeeded.
     */
    bool setDataMode(uint32_t data)
    {
        if( !((data == SPI_MODE0) || (data == SPI_MODE1) || (data == SPI_MODE2) || (data == SPI_MODE3)) )
        {
            return false;
        }
        dataMode = data;

        return true;
    }

private:
    /**
     * @brief SPI Clock Frequency
     */
    uint32_t clockFrequency; 

    /**
     * @brief SPI First Bit (SPI_FIRSTBIT_MSB/SPI_FIRSTBIT_LSB)
     */
    uint32_t bitOrder;  

    /**
     * @brief SPI Mode (SPI_MODE0, SPI_MODE1, etc.)
     *  */     
    uint32_t dataMode;       
};

/**
 * @class SPIClass
 */
class SPIClass 
{
public:

    /**
     * @brief Last error occurred for the object.
     */
    std:: string errorMessage;

    /**
     * @brief SPI handle
     */
    SPI_HandleTypeDef hspi; 

    /**
     * @brief Default constructor. Init some variables and parameters.
     * @note - Default SPI bit order is: SPI_FIRSTBIT_MSB
     * 
     * @note - Default SPI clock frequency is: 1000000
     * 
     * @note - Default SPI data mode is: SPI_MODE0
     */
    SPIClass();

    /**
     * @brief Initialize SPI
     * @return true if succeeded.
     */
    bool begin();                                

    /**
     * @brief Deinitialize SPI
     * @return true if succeeded.
     */
    bool end();                                  

    /**
     * @brief Start SPI transaction with certain SPI setting
     * @return true if succeeded.
     */
    bool beginTransaction(SPISettings settings); 

    /**
     * @brief Start SPI transaction with deafult or last SPI setting.
     * @return true if succeeded.
     */
    bool beginTransaction(void);

    /**
     * @brief End SPI transaction
     */
    void endTransaction();                      

    /**
     * @brief Transfer a byte
     */
    uint8_t transfer(uint8_t data);             

    /**
     * @brief Transfer 16-bit data
     */
    uint16_t transfer16(uint16_t data);         

    /**
     * @brief Transfer multiple bytess
     * @param buf: used for send and recieved data.
     * @param count: Length of the data. 
     * @return true if succeeded.
     */
    bool transfer(void *buf, size_t count);     

    /**
     * @brief Set SPI First Bit (SPI_FIRSTBIT_MSB/SPI_FIRSTBIT_LSB)
     * @note After using this function, it is necessary to call begin() or beginTransaction().
     * @return true if succeeded.
     *  */ 
    bool setBitOrder(uint8_t bitOrder);

    /**
     * @brief Set SPI Mode (SPI_MODE0, SPI_MODE1, etc.)
     * @note After using this function, it is necessary to call begin() or beginTransaction().
     * @return true if succeeded.
     */
    bool setDataMode(uint8_t dataMode);

    /**
     * @brief Directly set clock divider
     * @note After using this function, it is necessary to call begin() or beginTransaction().
     * 
     * @note The value can be:
     * 
     * SPI_CLOCK_DIV2
     *  
     * SPI_CLOCK_DIV4
     *  
     * SPI_CLOCK_DIV8 
     * 
     * SPI_CLOCK_DIV16 
     * 
     * SPI_CLOCK_DIV32 
     * 
     * SPI_CLOCK_DIV64 
     * 
     * SPI_CLOCK_DIV128 
     * 
     * SPI_CLOCK_DIV256 
     * 
     * @return true if succeeded.
     *  */ 
    bool setClockDivider(uint32_t clockDiv);

private:                   

    /**
     * @brief Current SPI settings
     */
    SPISettings _currentSettings;                

    /**
     * @brief Helper function to select baud rate prescaler based on desired clock speed
     *  */ 
    uint32_t _getPrescaler(uint32_t clockSpeed);
};



