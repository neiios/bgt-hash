# Hash generatorius

# To see the original readme open [v0.2](https://github.com/richard96292/bgt-hash/tree/v0.2.0).

## Setup

```bash
# clone the repository
git clone https://github.com/richard96292/bgt-hash

# compile program with make
make

# hash abc string
./bin -s "abc"

# the same but with a file
printf abc > test.txt
./bin -f test.txt
```

## CLI arguments

```bash
# run the binary without any flags or with -h flag to get these
pass -f and a filename to read from a file
pass -s to read from stdin
```

## Pseudocode

```bash
start with original message with length of L bits

append N 8 bit values 101010 until message is a multiple of 1024 bits

append original message length as last 64 bits bits to the message

process the message in 1024 bit chunks
  create array of 128 32bit words

  generate first 32 words from the initial message
    convert 4 8bit values into 1 32bit value

  generate the rest 96 words
    W[j] = fun1(W[j - 30]) + rotr(W[j - 4], 3) + fun0(W[j - 7]) + fun0(W[j - 32]);

  initialize H (8 32bit values) with initial numbers

  loop through each of 128 word
    tempWord1 = ufun0(H[3]) + majority(H[1], H[2], H[3]) + H[7] + K[j] + W[j];
    tempWord2 = ufun1(H[0]) + choice(H[5], H[6], H[7]);

    move every word in H up

    H[7] = (T1 + T2) ^ T2;
    H[3] += T1;

  for all initial values where i is iterator
    new initial value[i] += H[i]

convert initial values to hex and merge them
```
