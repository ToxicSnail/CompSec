#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/cpu.h>
#include <linux/version.h>

#define procfs_name "tsulab"
static struct proc_dir_entry *our_proc_file = NULL;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static ssize_t procfile_read(struct file *file, char __user *buffer, size_t count, loff_t *offset)
#else
static int procfile_read(struct file *file, char *buffer, size_t length, loff_t *offset)
#endif
{
    int ret = 0;
    char cpu_info[256];

    // Получение информации о процессоре
    struct cpuinfo_x86 *c = &boot_cpu_data;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
    if (*offset >= snprintf(cpu_info, sizeof(cpu_info), "Processor: %s, Family: %d, Model: %d\n",
                            c->x86_model_id, c->x86, c->x86_model)) {
        return 0;
    }

    if (count > strlen(cpu_info) - *offset) {
        count = strlen(cpu_info) - *offset;
    }

    // Копирование данных в пользовательское пространство
    ret = raw_copy_to_user(buffer, cpu_info + *offset, count);
    if (ret == 0) {
        *offset += count;
        ret = count;
    }
#else
    if (copy_to_user(buffer, cpu_info, strlen(cpu_info)) != 0) {
        ret = -EFAULT;
    } else {
        ret = strlen(cpu_info);
    }
#endif

    return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops proc_file_fops = {
    .proc_read = procfile_read,
};
#else
static const struct file_operations proc_file_fops = {
    .read = procfile_read,
};
#endif

static int __init tsulab_init(void)
{
    our_proc_file = proc_create(procfs_name, 0644, NULL, &proc_file_fops);

    if (our_proc_file == NULL) {
        pr_err("Failed to create proc file\n");
        return -ENOMEM;
    }

    pr_info("tsulab module loaded\n");
    return 0;
}

static void __exit tsulab_exit(void)
{
    proc_remove(our_proc_file);
    pr_info("tsulab module unloaded\n");
}

module_init(tsulab_init);
module_exit(tsulab_exit);

MODULE_LICENSE("GPL");
