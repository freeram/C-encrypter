# C-encrypter

Encryption used: [Blowfish](https://en.wikipedia.org/wiki/Blowfish_(cipher))

Padding scheme: PKCS#5

Cipher mode: ECB

Works on both little-endian and big-endian systems

##

:warning: **Disclaimer:** I wrote this program for fun. It should not be used for serious encryption as blowfish is suspectible to [birthday attacks](https://en.wikipedia.org/wiki/Birthday_attack).

For sensitive data, consider using openssl and AES.

## Installation

[WIP]

## Usage

| Flag &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;         | Description                                                                                   |
|------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [no flags] | Encrypts a file provided in stdin with key provided in `key.txt`. `key.txt` must be in the same directory as the binary                                                                    |
| `-g`       | Generates a new encryption key. When this flag is used, the program will create a new key and output it in a file. [WIP]                                                                   |
| `-k <path>`| Specifies the path to the key file. If `-k` is unspecified, the program looks for key in `key.txt` in the current directory.                                                               |
| `-d`       | Enables decryption mode. Use this flag to decrypt input using the specified key. If `-k` is unspecified, program looks for `key.txt` in the current directory.                             |


### Example Usage

[WIP]


## Acknowledgments

* Blowfish was adapted from pseudocode featured on the Wikipedia [article](https://en.wikipedia.org/wiki/Blowfish_(cipher)) for blowfish

* Input flag parsing with getopt was adapted from [Example-of-Getopt.html](https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html)
