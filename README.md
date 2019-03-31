![sha256](https://user-images.githubusercontent.com/22517432/55295668-a1f1e680-5407-11e9-9115-bf5b0766ce96.png)

# Project Outline
This is a program in the C programming language that calculates the SHA-256 checksum of an input.

# What is the SHA256 Hashing Algorithm? 
The Secure Hash Algorithms are a family of cryptographic hash functions published by the National Institute of
Standards and Technology as a U.S. Federal Information Processing Standard.

The standard for the algorithm can be found [here](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf).

# Running the program
The main file in this project is `sha256.c`. The other files in this project are used for first implementations and testing purposes. 
To use the program, you must ensure that you have the [GCC compiler](https://gcc.gnu.org/install/binaries.html) installed. When this is correctly installed,
you must open the file directory with the the command prompt. When in the correct directory, you can compile the C file by entering the following:

`gcc sha256.c -o sha256`

Once this command is executed, and there is no compile time errors, you can run the program. The program is ran by executing the following command:

`./sha256 test.md`

The second argument is the file you wish to hash. In my example, the file is titled 'test.md'.

# Implementation
This project was created by adapting the [documention](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf) supplied by the U.S.
Federal Information Processing Standard. It was then translated into C code by following closely to the video tutorials by [Dr. Ian McLoughlin.](https://web.microsoftstream.com/user/20b32719-41e8-4560-9f7f-c83ba751229c)

Before writing any code, extensive research was done to get a better understanding of what the SHA-256 algorithm actually it is, and its real life practices.
For example, during my research I found that the algorithm is used throughout block-chain which is used for crypto currencies. Another use of SHA-256 is that it is used in application protocols.
There is also a multitude of Secure Hash Algorithms, such as SHA-2.

I then began to watch each video which was explained in great detail. This gave me great insight into how to implement the SHA-256 program. When I had completed watching the videos,
I then combined to 2 given files (padfile.c and hash.c) to create the program. There was some bugs in the code, such as the bit shifting within the methods:

* `rotr`
* `sig0`
* `sig1`
* `SIG0`
* `SIG1`
* `Ch`
* `Maj`

I then discovered that there was some issues with files being in big endian and little endian. This in turn was giving an incorrect hash value for the given file.
To resolve this, I needed to check if the input was big or little endian. I then created a method to convert Big endian into Little Endian,
see functions: 

`BigEndianToLittle`, `IS_BIG_ENDIAN` and `SWAP_UINT64`

These were then implemented in the Sh256 function. After this, I added some error checking in the form of file opening. This is to ensure the file that is given as the second argument is not corrupt and that it actually exists.

### Below is an online checksum of an input file. I used my test file to get a hash value.

![onlineChecksum](https://user-images.githubusercontent.com/22517432/55296067-abca1880-540c-11e9-80fe-d1d891e5891c.PNG)

### My SHA256 program checksum gets the same hash value, see below.

![myChecksum](https://user-images.githubusercontent.com/22517432/55296088-f9468580-540c-11e9-95c6-133d686db506.PNG)

I have ran my program against a number of different files, getting the correct hash value the majority of the time. The only issue that it seems 
to have is that if you test it with a relatively large file it gets an incorrect hash value. I believe this may be an issue with the little/big endian conversion.

  

# Research
* [What is SHA-256](https://www.quora.com/What-is-SHA-256)

* [The Difference Between SHA-1, SHA-2 and SHA-256 Hash Algorithms](https://www.thesslstore.com/blog/difference-sha-1-sha-2-sha-256-hash-algorithms/)

* [Video Tutorials](https://web.microsoftstream.com/user/20b32719-41e8-4560-9f7f-c83ba751229c)

* [US Government SHA-256 Standard](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)

* [Little/Big Endian](http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/)

* [SHA-256 Checksum with a File](https://emn178.github.io/online-tools/sha256_checksum.html)
