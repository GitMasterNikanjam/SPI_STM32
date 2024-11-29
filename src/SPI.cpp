#include "SPI.h"

SPIClass::SPIClass() 
{
  _currentSettings.setBitOrder(SPI_FIRSTBIT_MSB);
  _currentSettings.setClockFrequency(1000000);
  _currentSettings.setDataMode(SPI_MODE0);

  hspi.Instance = nullptr;
  hspi.Init.Mode = SPI_MODE_MASTER;
  hspi.Init.Direction = SPI_DIRECTION_2LINES;
  hspi.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi.Init.NSS = SPI_NSS_SOFT;
  hspi.Init.BaudRatePrescaler = _getPrescaler(_currentSettings.getClockFrequency());
  hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial = 10;

}

bool SPIClass::begin() 
{
    if(hspi.Instance == NULL)
    {
        errorMessage = "Error SPIClass: SPI handle instance is null.";
        return false;
    }

    hspi.Init.BaudRatePrescaler = _getPrescaler(_currentSettings.getClockFrequency());
    hspi.Init.FirstBit = _currentSettings.getBitOrder();
    hspi.Init.CLKPolarity = (_currentSettings.getDataMode() & 0x02) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
    hspi.Init.CLKPhase = (_currentSettings.getDataMode() & 0x01) ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;

    if (HAL_SPI_Init(&hspi) != HAL_OK) 
    {
        errorMessage = "Error SPIClass: HAL_SPI_Init is not succeeded.";
        return false;
    }

    return true;
}

bool SPIClass::end() 
{
    if (HAL_SPI_DeInit(&hspi) != HAL_OK) 
    {
        errorMessage = "Error SPIClass: HAL_SPI_DeInit is not succeeded.";
        return false;  
    }

    return true;
}

bool SPIClass::beginTransaction(SPISettings settings) 
{
    _currentSettings = settings;

    if (beginTransaction() == false) 
    {
        return false;
    }

    return true;
}

bool SPIClass::beginTransaction(void)
{
    // Update SPI configuration based on settings
    hspi.Init.BaudRatePrescaler = _getPrescaler(_currentSettings.getClockFrequency());
    hspi.Init.FirstBit = _currentSettings.getBitOrder();
    hspi.Init.CLKPolarity = (_currentSettings.getDataMode() & 0x02) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
    hspi.Init.CLKPhase = (_currentSettings.getDataMode() & 0x01) ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;

    if (HAL_SPI_Init(&hspi) != HAL_OK) 
    {
        errorMessage = "Error SPIClass: HAL_SPI_Init is not succeeded.";
        return false;
    }

    return true;
}

void SPIClass::endTransaction() 
{
    // Typically not needed for HAL-based SPI unless specific cleanup is required
}

uint8_t SPIClass::transfer(uint8_t data) 
{
    uint8_t receivedData = 0;
    if (HAL_SPI_TransmitReceive(&hspi, &data, &receivedData, 1, HAL_MAX_DELAY) != HAL_OK) 
    {
        // Error_Handler();
    }
    return receivedData;
}

uint16_t SPIClass::transfer16(uint16_t data) 
{
    uint16_t receivedData = 0;
    if (HAL_SPI_TransmitReceive(&hspi, (uint8_t *)&data, (uint8_t *)&receivedData, 2, HAL_MAX_DELAY) != HAL_OK) 
    {
        // Error_Handler();
    }
    return receivedData;
}

bool SPIClass::transfer(void *buf, size_t count) 
{
    if (HAL_SPI_TransmitReceive(&hspi, (uint8_t *)buf, (uint8_t *)buf, count, HAL_MAX_DELAY) != HAL_OK) 
    {
        return false;
    }

    return true;
}

bool SPIClass::setBitOrder(uint8_t bitOrder) 
{
    if(_currentSettings.setBitOrder(bitOrder) == false)
    {
        errorMessage = "Error SPIClass: bitOrder value is not correct.";
        return false;
    }
    return true;
}

bool SPIClass::setDataMode(uint8_t dataMode) 
{
    if(_currentSettings.setDataMode(dataMode) == false)
    {
        errorMessage = "Error SPIClass: dataMode value is not correct.";
        return false;
    }
    return true;
}

bool SPIClass::setClockDivider(uint32_t clockDiv) 
{
    if(_currentSettings.setClockFrequency(clockDiv) == false)
    {
        errorMessage = "Error SPIClass: clockDiv value is not correct.";
        return false;
    }

    return true;
}

uint32_t SPIClass::_getPrescaler(uint32_t clockSpeed) 
{
    uint32_t pclk = HAL_RCC_GetPCLK2Freq(); // Adjust as needed for the actual SPI bus
    if (clockSpeed >= (pclk / 2)) return SPI_BAUDRATEPRESCALER_2;
    if (clockSpeed >= (pclk / 4)) return SPI_BAUDRATEPRESCALER_4;
    if (clockSpeed >= (pclk / 8)) return SPI_BAUDRATEPRESCALER_8;
    if (clockSpeed >= (pclk / 16)) return SPI_BAUDRATEPRESCALER_16;
    if (clockSpeed >= (pclk / 32)) return SPI_BAUDRATEPRESCALER_32;
    if (clockSpeed >= (pclk / 64)) return SPI_BAUDRATEPRESCALER_64;
    if (clockSpeed >= (pclk / 128)) return SPI_BAUDRATEPRESCALER_128;
    return SPI_BAUDRATEPRESCALER_256;
}



