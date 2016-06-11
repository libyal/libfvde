**libfvde** is a library to access [FileVault](https://en.wikipedia.org/wiki/FileVault) Drive Encryption (FVDE) (or FileVault2) encrypted volumes.

The FVDE format is used by Mac OS X, as of Lion, to encrypt data on a storage media volume.

Project information:
* Status: experimental
* License: LGPLv3+

Supported FileVault2 implementations:
* Mac OS X Lion (10.7)
* Mac OS X Mountain Lion (10.8)
* Mac OS X Mavericks (10.9)
* Mac OS X Yosemite (10.10)

Supported encryption volume types:
* removable media volume - with encrypted context (initial support as of 20121113 version)
* system volume

Supported protection methods:
* password
* recovery password
* VMK key data (as of 20121114 version)

Also see:
* [VileFault](https://code.google.com/archive/p/vilefault/) for accessing FileVault encrypted disk images (or user directories)
* [Infiltrate the Vault: Security Analysis and Decryption of Lion Full Disk Encryption](https://eprint.iacr.org/2012/374.pdf)
* [Security Analysis and Decryption of FileVault 2](https://www.cl.cam.ac.uk/~osc22/docs/slides_fv2_ifip_2013.pdf)

If you find this project useful, please cite the following paper in your publications:
* Omar Choudary, Felix Grobert and Joachim Metz. "Security Analysis and Decryption of Filevault 2", in Advances in Digital Forensics IX, IFIP Advances in Information and Communication Technology 410, 2013, pp 349-363.

Work in progress:
* DEFLATE compressed XML plist
* removable media volume - without encrypted context
* removable media volume - decrypted
* extend CoreStorage volume support
* partial encrypted volumes
* Python bindings

Planned:
* Windows support
* Dokan support

For more information see:
* Project documentation: https://github.com/libyal/libfvde/wiki/Home
* How to build from source: https://github.com/libyal/libfvde/wiki/Building

