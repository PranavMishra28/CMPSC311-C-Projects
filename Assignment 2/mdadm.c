#include "mdadm.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "jbod.h"

uint32_t getOP(int command, int diskID, int blockID)
{
  uint32_t op = 0;
  op = command << 26;
  op |= diskID << 22;
  op |= blockID;

  return op;
}

int unmount = 1; // Created a global variable to check if the disk has been
                 // unmounted or not. At this moment, by defining it to be 1,
                 // we say that the disk is unmounted.

int mdadm_mount(void)
{
  if (unmount == 1)
  {
    uint32_t op = getOP(JBOD_MOUNT, 0, 0);
    int isNum = jbod_operation(op, NULL);
    if (isNum == 0)
    {
      unmount = 0;
      return 1;
    }
  }
  else
  {
    return -1;
  }
  return -1;
}

int mdadm_unmount(void)
{
  if (unmount == 1)
  { // Checks if the disk is mounted already
    return -1;
  }
  else
  {
    uint32_t op = getOP(JBOD_UNMOUNT, 0, 0);
    int isNum = jbod_operation(op, NULL);
    if (isNum == 0)
    {
      unmount = 1;
      return 1;
    }
  }
  return -1;
}

int min(int a, int b){
  if(a < b){
    return a;
  }
  else{
    return b;
  }
}

int mdadm_read(uint32_t addr, uint32_t len, uint8_t *buf){
  // int bytes = 0;
  int offset = addr % 256;               // Calculating the offset
  uint32_t currentDisk = addr/65536;     // Calculates the current disk at which the operation is at
  uint32_t blockID = (addr / 256) % 256; // Gives the blockID belonging to the block of the specific disk
  uint32_t op;                           // Helps utilize the helper function
  // int remainingBytes; 
  int pointer = 0;                       // Making a pointer which helps us to read across the disks

  // Checking for all the edge cases. If any of the cases fail we would return
  // -1 and exist out of the function.
  // 1) If the disk is unmounted.
  // 2) If read goes beyond the length of  the disk.
  // 3) If addr is greater than 1024.
  // 4) If a NULL pointer is given as the place to read and the addr is also 0 at the same time.

  if(buf == NULL && len == 0){
    return 0;
  }

  if (len > 1024 || unmount == 1 || (buf == NULL && len < 0) || (addr + len > JBOD_DISK_SIZE * JBOD_NUM_DISKS) || (buf == NULL && len > 0) || len < 0){
    return -1;
  }

  // After reading the command fields, following this order of methods to run
  // out operations 1) SEEK TO DISK 2) SEEK TO BLOCK 3) READ TO THE BLOCK

  // Reading from the disk
  op = getOP(JBOD_SEEK_TO_DISK, currentDisk, 0);
  if (jbod_operation(op, NULL) == -1){
    return -1;
  }

  op = getOP(JBOD_SEEK_TO_BLOCK, currentDisk, blockID);
  if (jbod_operation(op, NULL) == -1){
    return -1;
  }

  uint8_t temp_buf[256]; // We create a temporary buffer function to read in data

  while(pointer < len){           
    op = getOP(JBOD_READ_BLOCK, currentDisk, blockID);
    if(jbod_operation(op, temp_buf) == -1){
      return -1;
    }

    if(pointer == 0){
      if(len<(256-offset)){
        memcpy(&buf[pointer], &temp_buf[offset], len);
        pointer += len;
      }
      else{
        memcpy(&buf[pointer], &temp_buf[offset], (256-offset));
        pointer += (256-offset);
        offset = 0;       // Shifting to new disk and therefore offset becomes 0
        if(blockID == 255){
          currentDisk += 1;
          blockID = 0;
          op = getOP(JBOD_SEEK_TO_DISK, currentDisk, 0);
          if(jbod_operation(op, NULL) == -1){
            return -1;
          }
        }
      }
    }
    else{
      if((len-pointer)<256){ // Same disk
        memcpy(&buf[pointer], &temp_buf[0], (len - pointer));
        pointer += (len-pointer);
      }
      else{
        memcpy(&buf[pointer], &temp_buf[0], 256);
        pointer += 256;
        if(blockID == 255){
          currentDisk += 1;  // Different disk within the block
          blockID = 0;
          op = getOP(JBOD_SEEK_TO_DISK, currentDisk, 0);
          if(jbod_operation(op, NULL) == -1){
            return -1;
          }
        }
      }
    } 
  }
  return len;
}

  // }
  // while (len > 0){
  //   op = getOP(JBOD_READ_BLOCK, currentDisk, blockID);
  //   if (jbod_operation(op, temp_buf) == -1)    // Read it into temporary buffer
  //   {
  //     return -1;
  //   }
      
  //   remainingBytes = min(len, min(256, 256 - offset));
  //   memcpy(buf + bytes, temp_buf + offset, remainingBytes);

  //   bytes += remainingBytes;
  //   len -= remainingBytes;
  //   addr += remainingBytes;
  //   offset = 0;
  // }
  
  // return bytes;