#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x1bfd809d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0x3c8d0f22, __VMLINUX_SYMBOL_STR(gpiod_unexport) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0xc10e7f66, __VMLINUX_SYMBOL_STR(gpiod_set_debounce) },
	{ 0xf00670a9, __VMLINUX_SYMBOL_STR(gpiod_direction_input) },
	{ 0xf83c6f7a, __VMLINUX_SYMBOL_STR(gpiod_export) },
	{ 0x1f55f47f, __VMLINUX_SYMBOL_STR(gpiod_direction_output_raw) },
	{ 0x59f530a, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "652297B47467BCF1F61343E");
