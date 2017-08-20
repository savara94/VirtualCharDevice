#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>

/* Could be changed */
#define MAJOR_NUM 245

#define IOCTL_SET_MODIFIER _IOR(MAJOR_NUM, 0, int)

typedef enum ioctl_id {
    SET_MODIFIER
} ioctl_id_t;

#endif
