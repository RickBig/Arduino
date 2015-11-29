// RS485Echo.ino
// Company: KMP Electronics Ltd, Bulgaria
// Web: http://kmpelectronics.eu/
// License: See the GNU General Public License for more details at http://www.gnu.org/copyleft/gpl.html
// Supported boards:
//		KMP DiNo II NETBOARD V1.0. Web: http://kmpelectronics.eu/en-us/products/dinoii.aspx
//		ProDiNo NetBoard V2.1. Web: http://kmpelectronics.eu/en-us/products/prodinoethernet.aspx
// Description:
//		RS485 echo test example.
// Example link: http://www.kmpelectronics.eu/en-us/examples/dinoii/rs485echo.aspx
// Version: 1.2.0
// Date: 29.11.2015
// Author: Plamen Kovandjiev <p.kovandiev@kmpelectronics.eu>
// Description: Compatibilie Arduinio version >= 1.6.5
// Warning! RS485 don't work Arduino version from 1.5.6 to 1.6.4. Please use the version: >=1.6.5.

#include <SPI.h>
#include <Ethernet.h>
#include "KmpDinoEthernet.h"
#include "KMPCommon.h"

// If in debug mode - print debug information in Serial. Comment in production code, this bring performance.
// This method is good for development and verification of results. But increases the amount of code and decreases performance.
//#define DEBUG

const uint8_t BUFF_MAX = 255;

uint8_t _dataBuffer[BUFF_MAX];

/**
* \brief Setup void. Arduino executed first. Initialize DiNo board and prepare Ethernet connection.
*
*
* \return void
*/
void setup()
{
#ifdef DEBUG
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    //while (!Serial) {
    //	; // wait for serial port to connect. Needed for Leonardo only. If need debug setup() void.
    //}
#endif

    // Init Dino board. Set pins and stop W5200. For this example W5200 not need to work.
    DinoInit(false);

    // Start RS485 with boud 19200 and 8N1.
    RS485Begin(19200);
}

/**
* \brief Loop void. Arduino executed second.
*
*
* \return void
*/
void loop()
{
    int i = RS485Read();
    
    // if i = -1 not data to read.
    if (i = -1)
    {
        return;
    }

    // If data send - On status led.
    OnStatusLed();
    
    uint8_t buffPos = 0;
    
    while(i > -1 && buffPos < BUFF_MAX)
    {
        _dataBuffer[buffPos++] = i;
#ifdef DEBUG
        Serial.write((char)i);
#endif
        // Read next char.
        i = RS485Read();
    }
    
    uint8_t sendPos = 0;
    while (sendPos < buffPos)
    {
        RS485Write(_dataBuffer[sendPos++]);
    }

    // Off status led.
    OffStatusLed();
}