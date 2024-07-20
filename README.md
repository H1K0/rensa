# RENSA cipher

## Contents

- [About](#about)
- [How it works](#how-it-works)
- [Usage](#usage)

## About

RENSA (_jp._ 連差, _lit._ "chain variation", _omon._ 連鎖 "chain") is an encryption method I invented a couple of days ago while relaxing at sea. Actually, I'm not sure if I'm not reinventing the wheel with this method because it is pretty simple and obvious, but anyway, it really works. As for me, one of the coolest features of RENSA is that it can even be used for encrypted communications with virtually no latency.

## How it works

First of all, RENSA needs a key. This key maps each char to a specific value. Let's call these values _hashes_ of chars. When encrypting, each char is shifted up by the hash of the previous char; when decrypting, it is shifted down. Very simple.

For example, if you have a message like "kagi", "a" is shifted by the hash of "k", "g" is shifted by the hash of "a" and so on. By default, the first char in the message is shifted by the hash of the _zeroth_ char which defaults to `'\0'` but can also be specified particularly.

## Usage

Just compile [`main.c`](main.c), [`rensa.h`](rensa.h) and [`rensa.c`](rensa.c) and you are all set.

```
Usage:
  rensa [-e/-d] [-i <input_path>] [-o <output_path>] [-k <key_path>] [-s <key_shift>] [-z <zeroth_char>]

Options:
  -h                  Print help and exit
  -e                  Encryption mode (default)
  -d                  Decryption mode
  -i <input_path>     Input file path (stdio by default)
  -o <output_path>    Output file path (stdout by default)
  -k <key_path>       Key file path
  -s <key_shift>      Key shift in bytes (0 by default)
  -z <zeroth_char>    Zeroth char (0 by default)
```

If `-k` is omitted, the default key is used. The default key simply maps each char to its value.

---

_&copy; Masahiko AMANO aka H1K0, 2024-present_
