#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x18\x00\x00\x00\x2b\x40\x05\xc9"
	"log_write_mmio\0\0"
	"\x1c\x00\x00\x00\x24\x3c\x4d\xfa"
	"log_post_write_mmio\0"
	"\x18\x00\x00\x00\x55\x48\x0e\xdc"
	"timer_delete\0\0\0\0"
	"\x14\x00\x00\x00\x2e\x61\xf9\x7f"
	"cdev_del\0\0\0\0"
	"\x18\x00\x00\x00\xfc\xe4\x27\x74"
	"device_destroy\0\0"
	"\x18\x00\x00\x00\xc1\xf7\x76\xa6"
	"class_destroy\0\0\0"
	"\x24\x00\x00\x00\x33\xb3\x91\x60"
	"unregister_chrdev_region\0\0\0\0"
	"\x10\x00\x00\x00\x53\x39\xc0\xed"
	"iounmap\0"
	"\x1c\x00\x00\x00\x48\x9f\xdb\x88"
	"__check_object_size\0"
	"\x1c\x00\x00\x00\xef\x6d\x5c\xa6"
	"alt_cb_patch_nops\0\0\0"
	"\x18\x00\x00\x00\x37\xca\x78\x36"
	"system_cpucaps\0\0"
	"\x20\x00\x00\x00\x28\xe1\xa4\x12"
	"__arch_copy_from_user\0\0\0"
	"\x18\x00\x00\x00\x80\x21\x72\x6b"
	"log_read_mmio\0\0\0"
	"\x1c\x00\x00\x00\xe3\x5e\x1a\x0c"
	"log_post_read_mmio\0\0"
	"\x1c\x00\x00\x00\x73\xde\x24\x7f"
	"jiffies_to_usecs\0\0\0\0"
	"\x10\x00\x00\x00\xa6\x50\xba\x15"
	"jiffies\0"
	"\x14\x00\x00\x00\xb8\x83\x8c\xc3"
	"mod_timer\0\0\0"
	"\x10\x00\x00\x00\xad\x64\xb7\xdc"
	"memset\0\0"
	"\x1c\x00\x00\x00\x1b\x2e\x3d\xec"
	"trace_hardirqs_off\0\0"
	"\x24\x00\x00\x00\x52\x3f\x0a\x4b"
	"gic_nonsecure_priorities\0\0\0\0"
	"\x1c\x00\x00\x00\x9a\xe6\x97\xd6"
	"trace_hardirqs_on\0\0\0"
	"\x18\x00\x00\x00\x8c\x89\xd4\xcb"
	"fortify_panic\0\0\0"
	"\x1c\x00\x00\x00\x8e\x9a\x96\xf1"
	"__usecs_to_jiffies\0\0"
	"\x1c\x00\x00\x00\x2b\x2f\xec\xe3"
	"alloc_chrdev_region\0"
	"\x18\x00\x00\x00\xd5\x2e\x41\x00"
	"class_create\0\0\0\0"
	"\x18\x00\x00\x00\xb9\x20\x92\x47"
	"device_create\0\0\0"
	"\x14\x00\x00\x00\xef\xf3\x89\xf3"
	"cdev_init\0\0\0"
	"\x14\x00\x00\x00\xe7\xb3\xba\x5a"
	"cdev_add\0\0\0\0"
	"\x20\x00\x00\x00\x0a\x60\x56\xaf"
	"arm64_use_ng_mappings\0\0\0"
	"\x18\x00\x00\x00\x39\x63\xf4\xc6"
	"init_timer_key\0\0"
	"\x18\x00\x00\x00\xa1\x3b\x86\x40"
	"ioremap_prot\0\0\0\0"
	"\x2c\x00\x00\x00\xc6\xfa\xb1\x54"
	"__ubsan_handle_load_invalid_value\0\0\0"
	"\x18\x00\x00\x00\x89\xd8\x58\x04"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D572CBF7AC76D182BA0EB1F");
