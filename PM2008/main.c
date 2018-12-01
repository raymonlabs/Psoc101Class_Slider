#include "project.h"
#include <stdio.h>

#define PM2008_I2C_ADDRESS                    0x28
#define PM2008_I2C_FRAME_HEADER               0x16

void print(char * name, uint16_t value){
    char str[100];

    sprintf(str, "%s : %d\r\n", name, value);
    UART_UartPutString(str);
}

void printHex(char * name, uint8_t value){
    char str[100];

    sprintf(str, "%s : %02X\r\n", name, value);
    UART_UartPutString(str);
}

void printBuffer(uint8_t * buffer, uint8_t length) {
    char str_buffer[100];

    for(uint8_t i = 0; i < length; i++)
    {
        char str[3];
        sprintf(str, "%02X", *(buffer+i));
        str_buffer[i*2] = str[0];
        str_buffer[i*2 + 1] = str[1];
    }

    UART_UartPutString( str_buffer );
    UART_UartPutString("\r\n");
}

int main(void)
{
    uint32 ret = 0;

    uint8_t     status;
    uint16_t    measuring_mode;
    uint16_t    calibration_coefficient;
    uint16_t    pm1p0_grimm;
    uint16_t    pm2p5_grimm;
    uint16_t    pm10_grimm;
    uint16_t    pm1p0_tsi;
    uint16_t    pm2p5_tsi;
    uint16_t    pm10_tsi;
    uint16_t    number_of_0p3_um;
    uint16_t    number_of_0p5_um;
    uint16_t    number_of_1_um;
    uint16_t    number_of_2p5_um;
    uint16_t    number_of_5_um;
    uint16_t    number_of_10_um;

    CyGlobalIntEnable; /* Enable global interrupts. */
 
    UART_Init();
    UART_Start();
    
    I2C_1_Init();
    I2C_1_Start();
   
    uint8_t command[7] = {PM2008_I2C_FRAME_HEADER, 0x7, 0x3, 0xFF, 0xFF, 0x0, 0x12};
    ret = I2C_1_I2CMasterWriteBuf(PM2008_I2C_ADDRESS, command, 7, I2C_1_I2C_MODE_COMPLETE_XFER);
   
    if (ret != I2C_1_I2C_MSTR_NO_ERROR) {
        UART_UartPutString( "Write Failed!\r\n" );
        UART_UartPutString( "Exit!" );
        return 0;
    }
   
    CyDelay(1000);
   
    uint8_t rx_buffer[32];

    for(;;)
    {
        ret = I2C_1_I2CMasterReadBuf(PM2008_I2C_ADDRESS, rx_buffer, 32, I2C_1_I2C_MODE_COMPLETE_XFER);
        
        if (ret == I2C_1_I2C_MSTR_NO_ERROR) {
            // Check frame header
            if (rx_buffer[0] != PM2008_I2C_FRAME_HEADER) {
                UART_UartPutString( "Frame header is different!\r\n" );
                return 0;
            }

            // Check frame length
            if (rx_buffer[1] != 32) {
                UART_UartPutString( "Frame length is not 32!\r\n" );
                return 0;
            }

            // // Check checksum
            // uint8_t check_code = rx_buffer[0];
            // for (uint8_t i = 1; i < 31; i++) {
            //     check_code ^= rx_buffer[i];
            // }

            // if (rx_buffer[31] != check_code) {
            //     UART_UartPutString( "Check code is different!\r\n" );
            //     printHex("rx_buffer[31]", rx_buffer[31]);
            //     printHex("check_code", check_code);
            //     printBuffer(rx_buffer, 32);
            //     return 0;
            // }

            status = rx_buffer[2];
            measuring_mode = (rx_buffer[3] << 8) + rx_buffer[4];
            calibration_coefficient = (rx_buffer[5] << 8) + rx_buffer[6];
            pm1p0_grimm = (rx_buffer[7] << 8) + rx_buffer[8];
            pm2p5_grimm = (rx_buffer[9] << 8) + rx_buffer[10];
            pm10_grimm = (rx_buffer[11] << 8) + rx_buffer[12];
            pm1p0_tsi = (rx_buffer[13] << 8) + rx_buffer[14];
            pm2p5_tsi= (rx_buffer[15] << 8) + rx_buffer[16];
            pm10_tsi= (rx_buffer[17] << 8) + rx_buffer[18];
            number_of_0p3_um= (rx_buffer[19] << 8) + rx_buffer[20];
            number_of_0p5_um= (rx_buffer[21] << 8) + rx_buffer[22];
            number_of_1_um= (rx_buffer[23] << 8) + rx_buffer[24];
            number_of_2p5_um= (rx_buffer[25] << 8) + rx_buffer[26];
            number_of_5_um= (rx_buffer[27] << 8) + rx_buffer[28];
            number_of_10_um= (rx_buffer[29] << 8) + rx_buffer[30];

            print("status", status);
            print("measuring_mode", measuring_mode);
            print("calibration_coefficient", calibration_coefficient);
            print("pm1p0_grimm", pm1p0_grimm);
            print("pm2p5_grimm", pm2p5_grimm);
            print("pm10_grimm", pm10_grimm);
            print("pm1p0_tsi", pm1p0_tsi);
            print("pm2p5_tsi", pm2p5_tsi);
            print("pm10_tsi", pm10_tsi);
            print("pm2p5_tsi", pm2p5_tsi);
            print("number_of_0p3_um", number_of_0p3_um);
            print("number_of_0p5_um", number_of_0p5_um);
            print("number_of_1_um", number_of_1_um);
            print("number_of_2p5_um", number_of_2p5_um);
            print("number_of_5_um", number_of_5_um);
            print("number_of_10_um", number_of_10_um);
            
        }

        CyDelay( 2000 ) ;
    }
}
