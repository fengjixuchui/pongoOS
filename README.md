pongoOS
=======

An experimental pre-boot execution environment for Apple boards built on top of checkra1n.

Contributions
-------------

By submitting a pull request, you certify that this contribution is coming from you and no one else. If you want to import third-party code, that shall be noted prominently for us to evaluate it appropriately.

Module
------
You can build the module at example/ with an iOS cross-compiler on Linux or a Mac. Refer to scripts/ to see how to load modules.

Kernel patchfinder
------------------

Note that the checkra1n patchfinder is not currently open-source. However, the KPF JIT that will ship on checkra1n 0.10.0 onwards is part of this repository. That means that pongoOS builds from this repository will always boot to the shell by default instead of XNU.
