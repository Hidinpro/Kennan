#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "onvif_comm.h"
//#include "onvif_dump.h"


int main(int argc, char **argv)
{
    ONVIF_DetectDevice(cb_discovery);

    return 0;
}