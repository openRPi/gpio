#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xcad60b4, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xc6ffe74c, __VMLINUX_SYMBOL_STR(platform_device_unregister) },
	{ 0xac9b9c1b, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0xb43ee367, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x28d65815, __VMLINUX_SYMBOL_STR(platform_device_register) },
	{ 0xa12b2c81, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x81f56b1c, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0xe08caaf0, __VMLINUX_SYMBOL_STR(framebuffer_release) },
	{ 0xecb24d5e, __VMLINUX_SYMBOL_STR(unregister_framebuffer) },
	{ 0x4d25330, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4C1BD6DF82E8A118EA27EC6");
