// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ddk/debug.h>
#include <ddk/protocol/platform-defs.h>
#include <soc/hi3660/hi3660-hw.h>

#include <stdio.h>

#include "hikey960.h"
#include "hikey960-hw.h"

//#define GPIO_TEST 1

static const pbus_mmio_t dwc3_mmios[] = {
    {
        .base = MMIO_USB3OTG_BASE,
        .length = MMIO_USB3OTG_LENGTH,
    },
};

static const pbus_irq_t dwc3_irqs[] = {
    {
        .irq = IRQ_USB3,
    },
};

static const pbus_dev_t dwc3_dev = {
    .name = "dwc3",
    .vid = PDEV_VID_GENERIC,
    .pid = PDEV_PID_GENERIC,
    .did = PDEV_DID_USB_DWC3,
    .mmios = dwc3_mmios,
    .mmio_count = countof(dwc3_mmios),
    .irqs = dwc3_irqs,
    .irq_count = countof(dwc3_irqs),
};

static const pbus_mmio_t xhci_mmios[] = {
    {
        .base = MMIO_USB3OTG_BASE,
        .length = MMIO_USB3OTG_LENGTH,
    },
};

static const pbus_irq_t xhci_irqs[] = {
    {
        .irq = IRQ_USB3,
    },
};

static const pbus_dev_t xhci_dev = {
    .name = "dwc3-xhci",
    .vid = PDEV_VID_GENERIC,
    .pid = PDEV_PID_GENERIC,
    .did = PDEV_DID_USB_XHCI,
    .mmios = xhci_mmios,
    .mmio_count = countof(xhci_mmios),
    .irqs = xhci_irqs,
    .irq_count = countof(xhci_irqs),
};

static const pbus_mmio_t mali_mmios[] = {
    {
        .base = MMIO_G3D_BASE,
        .length = MMIO_G3D_LENGTH,
    },
};

static const pbus_irq_t mali_irqs[] = {
    {
        .irq = IRQ_G3D_JOB,
    },
    {
        .irq = IRQ_G3D_MMU,
    },
    {
        .irq = IRQ_G3D_GPU,
    },
};

static const pbus_dev_t mali_dev = {
    .name = "mali",
    .vid = PDEV_VID_GENERIC,
    .pid = PDEV_PID_GENERIC,
    .did = PDEV_DID_ARM_MALI,
    .mmios = mali_mmios,
    .mmio_count = countof(mali_mmios),
    .irqs = mali_irqs,
    .irq_count = countof(mali_irqs),
};

static const pbus_mmio_t sdhci_mmios[] = {
    {
        // SD MMIO
        .base = MMIO_SD3_BASE,
        .length = MMIO_SD3_LENGTH,
    },
    {
        // SD pinmux
        .base = MMIO_IOC_MMC0_BASE,
        .length = MMIO_IOC_MMC0_LENGTH,
    },
};

static const pbus_irq_t sdhci_irqs[] = {
    {
        .irq = IRQ_SD3,
    },
};

static const pbus_dev_t sdhci_dev = {
    .name = "sdhci",
    .vid = PDEV_VID_HISILICON,
    .pid = PDEV_PID_HI3660,
    .did = PDEV_DID_HI3660_SDHCI,
    .mmios = sdhci_mmios,
    .mmio_count = countof(sdhci_mmios),
    .irqs = sdhci_irqs,
    .irq_count = countof(sdhci_irqs),
};

#if GPIO_TEST
static const pbus_gpio_t gpio_test_gpios[] = {
    {
        .gpio = GPIO_USER_LED1,
    },
    {
        .gpio = GPIO_USER_LED2,
    },
    {
        .gpio = GPIO_USER_LED3,
    },
    {
        .gpio = GPIO_USER_LED4,
    },
};

static const pbus_dev_t gpio_test_dev = {
    .name = "hikey960-gpio-test",
    .vid = PDEV_VID_96BOARDS,
    .pid = PDEV_PID_HIKEY960,
    .did = PDEV_DID_HIKEY960_GPIO_TEST,
    .gpios = gpio_test_gpios,
    .gpio_count = countof(gpio_test_gpios),
};
#endif

zx_status_t hikey960_add_devices(hikey960_t* hikey) {
    zx_status_t status;

    if ((status = pbus_device_add(&hikey->pbus, &dwc3_dev, 0)) != ZX_OK) {
        zxlogf(ERROR, "hi3360_add_devices could not add dwc3_dev: %d\n", status);
        return status;
    }
    // xhci_dev is enabled/disabled dynamically, so don't enable it here
    if ((status = pbus_device_add(&hikey->pbus, &xhci_dev, PDEV_ADD_DISABLED)) != ZX_OK) {
        zxlogf(ERROR, "hi3360_add_devices could not add xhci_dev: %d\n", status);
        return status;
    }
    if ((status = pbus_device_add(&hikey->pbus, &mali_dev, 0)) != ZX_OK) {
        zxlogf(ERROR, "hi3360_add_devices could not add mali_dev: %d\n", status);
        return status;
    }
    if ((status = pbus_device_add(&hikey->pbus, &sdhci_dev, 0)) != ZX_OK) {
        zxlogf(ERROR, "hi3360_add_devices could not add sdhci_dev: %d\n", status);
        return status;
    }

#if GPIO_TEST
    if ((status = pbus_device_add(&hikey->pbus, &gpio_test_dev, 0)) != ZX_OK) {
        zxlogf(ERROR, "hi3360_add_devices could not add gpio_test_dev: %d\n", status);
        return status;
    }
#endif

    return ZX_OK;
}
