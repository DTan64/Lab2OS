

#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/slab.h>
#include<asm/uaccess.h>


#define BUFFER_SIZE 200
static int OPEN_COUNTER = 0;
static int CLOSE_COUNTER = 0;
//static int BYTES_READ = 0;

/* Define device_buffer and other global data structures you will need here */
void* db;
void* dbStart;

ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */


	if(length > BUFFER_SIZE)
	{
		copy_to_user(buffer, db, BUFFER_SIZE);
		printk(KERN_ALERT "Read %i bytes", BUFFER_SIZE);
		return 0;
	}
	if(length < BUFFER_SIZE)
	{
		copy_to_user(buffer, db, length);
		printk(KERN_ALERT "Read %i bytes", length);
		return 0;
	}
}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
        int bufferSpace = BUFFER_SIZE - strlen(db);
        if(strlen(db) >= BUFFER_SIZE)
        {
                printk(KERN_ALERT "Buffer full can't write");
                return 0;
        }
        if(length > bufferSpace)
        {
                copy_from_user(db, buffer, bufferSpace);
		db = dbStart;
		db += strlen(db);
                printk(KERN_ALERT "Buffer full. Lost %i bytes", length - bufferSpace);
                printk(KERN_ALERT "Wrote %i bytes", bufferSpace);
                return bufferSpace;
        }
        else
        {
                copy_from_user(db, buffer, length);
		db = dbStart;
		db += strlen(db);
                printk(KERN_ALERT "Wrote %i bytes", strlen(db) - *offset);
                return length;
        }
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	OPEN_COUNTER++;
	printk(KERN_ALERT "Device is opened. Has been opened %d times\n", OPEN_COUNTER);
	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	CLOSE_COUNTER++;
	printk(KERN_ALERT "Device is closed. Has been closed %d times\n", CLOSE_COUNTER);
	return 0;
}

loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	if(offset > BUFFER_SIZE)
	{
		printk(KERN_ALERT "Invalid offset value");
		return -1;
	}
	if(whence == 0)
	{
		db = dbStart + offset;
		return db;
	}
	if(whence == 1)
	{
		if((db += offset) > strlen(db) || (db += offset < dbStart))
		{
			printk(KERN_ALERT "Invalid offset value");
			return -1;
		}
		db += offset;
		return db;
	}
	if(whence == 2)
	{
		void* tmp;
		db = dbStart;
		tmp = (dbStart + strlen(db)) - offset;
		db = tmp;
	}

}

struct file_operations simple_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	.read =	simple_char_driver_read,
	.write = simple_char_driver_write,
	.open =	simple_char_driver_open,
	.release =  simple_char_driver_close,
	.llseek = simple_char_driver_seek
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
};

static int simple_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	/* register the device */
	printk(KERN_ALERT "init function was called");
	register_chrdev(64,"simple_char_driver", &simple_char_driver_file_operations);
	db = kmalloc(BUFFER_SIZE, GFP_KERNEL);
	dbStart = db;
	return 0;
}

static void simple_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
	printk(KERN_ALERT "this is the exit function being called");
	unregister_chrdev(64, "simple_char_driver");
	kfree(db);

}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);
