  #!/bincd /sh
  # Written by Uwe Hermann <uwe@hermann-uwe.de>, released as public domain.
  # Modified by Michael Feiner
  # Set your HOST and your TARGET
  # and set the ON OFF flags for MAKE, best you compile step by step.
  
  HOST=amd64-linux
  #HOST=i386-linux

  #TARGET=arm-elf
  #TARGET=arm-none-eabi
  #TARGET=h8300-hms
  #TARGET=h8300-coff
  #TARGET=m68k-elf
  TARGET=i386-elf

  
  if [ "$TARGET" = "arm-elf" ]
  then
  BINUTILS_VERSION=2.20.1
  GCC_VERSION=4.4.3
  LIBC_VERSION=1.18.0
  GDB_VERSION=7.4.1

  TARGET_CONFIG_FLAGS="--enable-interwork --enable-multilib"    # to enable both arm and thumb code
  TARGET_CFLAGS=""
  LANGUAGES="c,c++"
  fi

  if [ "$TARGET" = "arm-none-eabi" ]
  then
  BINUTILS_VERSION=2.20.1
  GCC_VERSION=4.4.3
  LIBC_VERSION=1.18.0
  GDB_VERSION=7.4.1

  TARGET_CONFIG_FLAGS="--enable-interwork --enable-multilib"    # to enable both arm and thumb code
  TARGET_CFLAGS=""
  LANGUAGES="c,c++"
  fi

  if [ "$TARGET" = "h8300-hms" ]
  then
  BINUTILS_VERSION=2.16.1
  GCC_VERSION=3.4.6
  LIBC_VERSION=1.18.0
  GDB_VERSION=7.1

  TARGET_CONFIG_FLAGS="" 
  TARGET_CFLAGS="-O2 -fomit-frame-pointer"
  LANGUAGES="c"
  fi

  if [ "$TARGET" = "h8300-coff" ]
  then
  PATCH=-h8
  BINUTILS_VERSION=2.16.1$PATCH
  GCC_VERSION=3.4.6$PATCH
  LIBC_VERSION=1.18.0
  GDB_VERSION=7.1

  TARGET_CONFIG_FLAGS="" 
  TARGET_CFLAGS="-O2 -fomit-frame-pointer"
  LANGUAGES="c"
  fi

  if [ "$TARGET" = "m68k-elf" ]
  then
  BINUTILS_VERSION=2.20.1
  GCC_VERSION=4.4.3
  LIBC_VERSION=1.18.0
  GDB_VERSION=7.4.1

  TARGET_CONFIG_FLAGS="" 
  TARGET_CFLAGS=""  
  LANGUAGES="c,c++"
  fi

  if [ "$TARGET" = "i386-elf" ]
  then
  BINUTILS_VERSION=2.20.1
  GCC_VERSION=4.4.3
  LIBC_VERSION=1.18.0
  GDB_VERSION=7.4.1

  TARGET_CONFIG_FLAGS="" 
  TARGET_CFLAGS=""
  LANGUAGES="c,c++"
  fi

  BINUTILS="binutils-"$BINUTILS_VERSION
  GCC="gcc-"$GCC_VERSION
  NEWLIB="newlib-"$LIBC_VERSION
  GDB="gdb-"$GDB_VERSION

  VERSION=$GCC_VERSION

  CFLAGS="-Wno-error "$TARGET_CFLAGS
  CONFIG_FLAGS=$TARGET_CONFIG_FLAGS
  WITH_GNU="--with-gnu-as --with-gnu-ld"

  #PARALLEL=""
  PARALLEL="-j 2"

  # BUILD : 1 = ON, 0 = OFF
  INSTALL_BASEUTILS=0
  MAKE_BINUTILS=0
  MAKE_GCC_1=0
  MAKE_NEWLIB=0
  MAKE_GCC_2=0
  MAKE_GDB=0

  SCRIPTDIR=`pwd`
  CROSSDIR=$MOST_ROOT/cross
  PREFIX=$CROSSDIR/bin/$HOST/$TARGET/$VERSION                         # Install location of your final toolchain
  export PATH="$PATH:$PREFIX/bin"

  mkdir $CROSSDIR/build


  # install base utilities
  if [ $INSTALL_BASEUTILS -eq 1 ]
  then    
  apt-get install flex bison libgmp3-dev libmpfr-dev libncurses5-dev libmpc-dev autoconf texinfo build-essential
  fi

  # first build binutils, gcc is setting up configuration depending on used binutil version
  if [ $MAKE_BINUTILS -eq 1 ]
  then
  cd $CROSSDIR/build
  $CROSSDIR/src/binutils/$BINUTILS/configure --target=$TARGET --prefix=$PREFIX $CONFIG_FLAGS \
    $WITH_GNU --disable-nls
  make $PARALLEL "CFLAGS=$CFLAGS"
  make install
  cd $SCRIPTDIR
  fi

  if [ $MAKE_GCC_1 -eq 1 ]
  then
  cd $CROSSDIR/build
  $CROSSDIR/src/gcc/$GCC/configure --target=$TARGET --prefix=$PREFIX $CONFIG_FLAGS \
    --enable-languages="c" --with-newlib --without-headers --disable-shared $WITH_GNU
  make $PARALLEL "CFLAGS=$CFLAGS" all-gcc
  make install-gcc
  cd $SCRIPTDIR
  fi

  if [ $MAKE_NEWLIB -eq 1 ]
  then
  cd $CROSSDIR/build
  $CROSSDIR/src/libc/$NEWLIB/configure --target=$TARGET --prefix=$PREFIX $CONFIG_FLAGS \
    $WITH_GNU --disable-nls
  make $PARALLEL "CFLAGS=$CFLAGS"
  make install
  cd $SCRIPTDIR
  fi

  # Yes, you need to build gcc again!
  if [ $MAKE_GCC_2 -eq 1 ]
  then
  cd $CROSSDIR/build
  $CROSSDIR/src/gcc/$GCC/configure --target=$TARGET --prefix=$PREFIX $CONFIG_FLAGS \
    --enable-languages=$LANGUAGES --with-newlib --disable-shared $WITH_GNU
  make $PARALLEL "CFLAGS=$CFLAGS"
  make install
  cd ..
  fi

  if [ $MAKE_GDB -eq 1 ]
  then
  cd $CROSSDIR/build
  $CROSSDIR/src/gdb/$GDB/configure --target=$TARGET --prefix=$PREFIX $CONFIG_FLAGS
  make $PARALLEL "CFLAGS=$CFLAGS"
  make install
  cd $SCRIPTDIR
  fi  

  rm -rf $CROSSDIR/build
