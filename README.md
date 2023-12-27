# C-encrypter

Encryption used: [Blowfish](https://en.wikipedia.org/wiki/Blowfish_(cipher))

Padding scheme: PKCS#5

Cipher mode: ECB

Works on both little-endian and big-endian systems

##

:warning: **Disclaimer:** I wrote this program for fun. It should not be used for serious encryption as blowfish is suspectible to [birthday attacks](https://en.wikipedia.org/wiki/Birthday_attack).

For sensitive data, consider using openssl and AES.

## Installation

```sh
git clone https://github.com/freeram/C-encrypter.git
```
```sh
cd C-encrypter/
```
```sh
make
```
```sh
cd bin/
```

## Usage

| Flag           | Description                                                                                                                                                                      |
|----------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [no flags]     | Encrypts a file provided in stdin with key provided in `key.txt`. `key.txt` must be in the same directory as the binary.                                                         |
| `-k <filename>`| Specifies the name of the key file. If `-k` is unspecified, the program looks for key in `key.txt` in the current directory.                                                     |
| `-d`           | Enables decryption mode. Use this flag to decrypt input using the specified key. If `-k` is unspecified, program looks for `key.txt` in the current directory.                   |
| `-h`           | Enables hex Input/Output mode. Use this flag to encrypt/decrypt input using the specified key (read in ASCII), but the input will be read in hex, and the output will be in hex. |


### Example Usage

Encrypt a file input.txt using the key in key.txt:
```sh
./encrypt < input.txt
```

Decrypt a file input.txt using the key in key.txt:
```sh
./encrypt -d < input.txt
```

Encrypt a file input.txt using the key in filename.txt:
```sh
./encrypt -k filename.txt < input.txt
```

Decrypt input.txt, while reading it in hex, and output hex:
```sh
./encrypt -h -d < input.txt
```

## Acknowledgments

* Blowfish was adapted from pseudocode featured on the Wikipedia [article](https://en.wikipedia.org/wiki/Blowfish_(cipher)) for blowfish

* Input flag parsing with getopt was adapted from [Example-of-Getopt.html](https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html)
