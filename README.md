# U-Boot port for ZTE W103D (S905L3A)

Mainline U-Boot board support for the ZTE W103D OTT box (Amlogic S905L3A / G12A family),
verified on real hardware. Used as a chainload `u-boot.ext` jumped to from the vendor
BL33 via `go` (the box has eFuse secure-boot enabled, so FIP replacement is not possible
without the vendor key).

- Base: upstream U-Boot `55084065` (`v2026.10`)
- Reference binary (v2026.10 + patches 0001, 0002, 0004), 1,001,512 bytes, SHA-256
  `6ebedcdd2906c46a83718836517fc89aced8532b2a869f32777bc0afe8dfa9f6`
- Previous reference binary (`v2026.10-rc4`, without 0004): `u-boot-final.bin`, SHA-256
  `ea6077ff2b79d830fd4baa3e567cb1acadfca7758dfd978a74e0ac0892adbddb`
  (also published at
  [ophub/u-boot](https://github.com/ophub/u-boot) as `u-boot-w103d.bin`,
  see PR ophub/u-boot#51)

## Layout (paths relative to U-Boot tree root)

- `board/amlogic/w103d/` — board code, Makefile, MAINTAINERS
- `configs/w103d_defconfig` — production defconfig (`ENV_IS_NOWHERE`, self-contained bootcmd)
- `dts/upstream/src/arm64/amlogic/meson-g12a-w103d.dts` — board device tree
- `arch/arm/dts/meson-g12a-w103d-u-boot.dtsi` — U-Boot overlay (keeps Linux MMC aliases so
  eMMC is always mmc dev 2 regardless of SDIO probe order)
- `doc/board/amlogic/w103d.rst` — board documentation
- `patches/` — driver fixes carried by this port (also proposed upstream separately)

## Driver fixes (`patches/`)

- `0001-video-meson-dw-hdmi-fallback-edid.patch` — use a canned single-block 1080p60
  EDID when no usable EDID can be read from the sink; a real EDID keeps precedence.
- `0002-video-meson-vpu-1080p-fallback.patch` — when HDMI hardware is present but the
  sink provides no usable mode (e.g. 4K-only EDID beyond VENC 1920x1200 limits), bring
  up HDMI at safe 1080p60 instead of silently dropping to CVBS (black HDMI).
- `0004-mmc-meson-gx-clear-inherited-delay-and-adjust.patch` — clear the eMMC
  controller's DELAY1/DELAY2/ADJUST at probe. The vendor BL33 tunes the controller
  before chainloading and leaves its result behind (ADJUST = `0x00142000` on one board:
  resampling on, delay 20); this driver never programs those registers, so on some
  boards eMMC reads failed on some boots and worked on others.

## Build

```sh
git clone <upstream u-boot> && cd u-boot
git checkout v2026.10
# copy board/, configs/, dts/, arch/, doc/ from this repo into the tree
# apply patches/*.patch
make w103d_defconfig
make CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc) u-boot.bin
```

The resulting `u-boot.bin` is loaded as `u-boot.ext` on the boot FAT partition.

## Boot contract (FAT partition, eMMC `mmc 2:1`)

- `uEnv.txt` with `LINUX` / `INITRD` / `FDT` / `APPEND`
- optional `bootup.bmp` (1280x720 BMP, centered via `bmp display`; missing file never blocks boot)
- `/w103d-video` (`ok` / `fail`) is reported in the device-tree root for black-box verification

## License

GPL-2.0+ (board files), matching upstream U-Boot.
