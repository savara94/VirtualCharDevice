#include "device.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>  /* ioctl */

int32_t open_device(const char* device)
{
    int32_t filedesc = open(device, O_RDWR);
    
    if(filedesc < 0)
    {
        printf("An error occurred in the open.\n");
        return RET_ERR;
    }
    
    return filedesc;
}

int32_t read_device(int32_t fd, uint8_t *data, int32_t length)
{
    if(read(fd, data, length) < 0)
    {
        printf("An error occurred in the read.\n");
        return RET_ERR;
    }
    
    return RET_OK;
}

int32_t write_device(int32_t fd, uint8_t *data, int32_t length)
{
    if (write(fd, data, length) != length)
    {
        printf("An error occurred in the write.\n");
        return RET_ERR;
    }
 
    return RET_OK;
}

int32_t close_device(int32_t fd)
{
    if(close(fd) < 0)
    {
        printf("An error occurred in the close.\n");
        return RET_ERR;
    }
    
    return RET_OK;
}

int32_t ioctl_device(int32_t fd, ioctl_id_t id, void *parameter)
{
    int32_t ret_val;

    switch(id)
    {
        case SET_MODIFIER:
            ret_val = ioctl(fd, IOCTL_SET_MODIFIER, *((int32_t*)parameter));
            break;
            
        default:
            /* Should not happen */
            break;
            
    }

    if (ret_val < 0)
    {
        printf ("An error occurred in the ioctl.\n");
        return RET_ERR;
    }
    
    return RET_OK;
}