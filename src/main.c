/* Includes ------------------------------------------------------------------*/
#include "sml_processing.h"
#include "zigbee.h"
/* LOG channel def -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void main(void) {
    if (init_sml_processing() != 0) {
        return;
    }
    // The Zigbee thread will wait until the first sml message
    start_zigbee();
    start_sml_processing();
}
/* Exported functions --------------------------------------------------------*/
