#include <stdint.h>
#include <stdio.h>
#include "device.h"
#include "../kmod/chardev.h"

#define ARRAY_SIZE(array)   (sizeof(array)/sizeof(array[0]))

#define CHAR_DEVICE     "/dev/chardev"

static uint8_t test_array[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g'
};

static uint8_t buffer[ARRAY_SIZE(test_array)];

void dump_bytes(uint8_t* array, size_t length)
{
    size_t i = 0;
    
    for(; i < length; i++)
    {
        printf("%c ", array[i]); 
    }
    
    printf("\n");
}


int32_t main(void)
{
    int32_t fd = open_device(CHAR_DEVICE);
    
    int32_t test_array_size = ARRAY_SIZE(test_array);
    int32_t buffer_size = ARRAY_SIZE(buffer);
    
    int32_t modifier = 1;
    
    if (fd == RET_ERR)
    {
        return RET_ERR;
    }
    
    if (ioctl_device(fd, SET_MODIFIER, &modifier) == RET_ERR)
    {
        return RET_ERR;
    }
    
    printf("Sending:\t");
    dump_bytes(test_array, test_array_size);

    if (write_device(fd, test_array, test_array_size) == RET_ERR)
    {
        return RET_ERR;
    }
    
    printf("Receiving:\t");
    
    if (read_device(fd, buffer, buffer_size) == RET_ERR)
    {
        return RET_ERR;
    }
    
    dump_bytes(buffer, buffer_size);
    
    if (close_device(fd) == RET_ERR)
    {
        return RET_ERR;
    }
    
    return RET_OK;
}