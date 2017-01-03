# kTPM: A Software TPM using kernel-level privilege separation for ARM
kTPM is a project aims to implements software TPM 2.0 using kernel-level privilege separation. kTPM support similar security level with hardware TPM chip and provide high performance.  It includes three major components: kernel patches, untrusted helper, and trusted cloud. This is the prototype kernel for Odroid-XU4 device, forked from [odroidxu4-v4.2-rc1](https://github.com/hardkernel/linux/tree/odroidxu4-v4.2-rc1).

### Technical Details
-Please check out paper for more details.-

### Notes
* It's just a proof-of-concept prototype, so try at your own risk.
* You'll need all three components to fully support software TPM 2.0 from kTPM, I'll push the other two components into other repos (later).

### Build
Reference: [Odroid Wiki: xu4_building_kernel](http://odroid.com/dokuwiki/doku.php?id=en:xu4_building_kernel)

#### Toolchain (4.9.2)
Click one of the site to download toolchain to build Linux kernel.
* [Download #1](http://releases.linaro.org/14.09/components/toolchain/binaries/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux.tar.xz)
* [Download #2](http://dn.odroid.com/toolchains/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux.tar.xz)

Once the download is done, extract the tarball to /opt/toolchains/.
```
$ sudo mkdir -p /opt/toolchains
$ sudo tar xvf gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux.tar.xz -C /opt/toolchains/
```

In order to add the toolchain path to PATH, paste below lines to $HOME/.bashrc.
```
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
export PATH=/opt/toolchains/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin/:$PATH
```

You can check if the toolchain installed above works properly while checking the version of toolchain. If you can find gcc version 4.9.2 20140904 (prerelease) at the end of the line, the toolchain is well installed.
```
$ arm-linux-gnueabihf-gcc -v
Using built-in specs.
COLLECT_GCC=/opt/toolchains/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin/arm-linux-gnueabihf-gcc
COLLECT_LTO_WRAPPER=/opt/toolchains/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin/../libexec/gcc/arm-linux-gnueabihf/4.9.2/lto-wrapper
Target: arm-linux-gnueabihf
Configured with: /cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/.build/src/gcc-linaro-4.9-2014.09/configure --build=i686-build_pc-linux-gnu --host=i686-build_pc-linux-gnu --target=arm-linux-gnueabihf --prefix=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/install --with-sysroot=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/install/arm-linux-gnueabihf/libc --enable-languages=c,c++,fortran --disable-multilib --enable-multiarch --with-arch=armv7-a --with-tune=cortex-a9 --with-fpu=vfpv3-d16 --with-float=hard --with-pkgversion='crosstool-NG linaro-1.13.1-4.9-2014.09 - Linaro GCC 4.9-2014.09' --with-bugurl=https://bugs.launchpad.net/gcc-linaro --enable-__cxa_atexit --enable-libmudflap --enable-libgomp --enable-libssp --with-gmp=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/.build/arm-linux-gnueabihf/build/static --with-mpfr=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/.build/arm-linux-gnueabihf/build/static --with-mpc=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/.build/arm-linux-gnueabihf/build/static --with-isl=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/.build/arm-linux-gnueabihf/build/static --with-cloog=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/.build/arm-linux-gnueabihf/build/static --with-libelf=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/.build/arm-linux-gnueabihf/build/static --enable-threads=posix --disable-libstdcxx-pch --enable-linker-build-id --enable-plugin --enable-gold --with-local-prefix=/cbuild/slaves/oorts/crosstool-ng/builds/arm-linux-gnueabihf-linux/install/arm-linux-gnueabihf/libc --enable-c99 --enable-long-long --with-mode=thumb --disable-multilib --with-float=hard
Thread model: posix
gcc version 4.9.2 20140904 (prerelease) (crosstool-NG linaro-1.13.1-4.9-2014.09 - Linaro GCC 4.9-2014.09) 
```
