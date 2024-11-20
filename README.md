![Hannibal](Hannibal.svg)

HBINS are mini versions of [Hannibal](https://github.com/silentwarble/Hannibal), to be used with Hannibal. Instead of the usual BOF/COFF files we are using a PIC .bin.

See the article [Making Mini Monsters](https://silentwarble.com/posts/making-mini-monsters/) for a deeper dive.

## Installation

You will need make, MingW, and Python installed to build these. See the [Making Monsters](https://silentwarble.com/posts/making-monsters-1/#installation) installation section. It is the same for HBINs.

Simply clone the repo, invoke the appropriate makefile ```make -f windows_makefile```, and use execute_hbin in Mythic to upload bin/hannibal.bin.

## Note

- These have not been extensively tested. They are functional, but need much more testing before being considered Op worthy.
- The debug process for these is different from Hannibal. A TODO is to get a standalone debug build working for these, but currently that is not supported. Use x64dbg or similar. 