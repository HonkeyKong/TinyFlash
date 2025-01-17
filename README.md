## TinyFlash

This program can take a GBA ROM under 250KB in size, and flash it to one of those cheap bootleg GBA carts you can find everywhere on AliExpress. Right now, the 8MB (Link to the Past) and 16MB (Minish Cap/Pokemon) cartridges are supported. 32MB carts will probably come after I get my hands on one.

To use the flasher, simply copy your ROM to the `res` folder, and then do this from your project root:  
  
  ```
  mkdir build
  cd build
  arm-none-eabi-cmake ..
  make
  ```
  
  Once you've done this, you should have a `tinyflash.gba` file that you can use to write your game to a cartridge. If your file is too big, the linker will geek out and tell you your ROM won't fit into EWRAM. Just work on making it smaller, it's not that hard. :)
  
  Once you've loaded the TinyFlash ROM, either via multiboot or from a flash cartridge, you can safely insert your bootleg cartridge, press the A button, and flash your ROM.
  
  VOILA! You now have your game on a cartridge. Doesn't that feel nice?