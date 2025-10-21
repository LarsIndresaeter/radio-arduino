#include <watchdog.hpp>

namespace WATCHDOG {

void softReset()
{
    wdt_enable(WDTO_15MS);
    for (;;) {
    }
}

}
