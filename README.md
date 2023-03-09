# memcpy_util - A collection of small "variants" of memcpy/memmove

# About

This is just a small collection of memcpy/memmove "versions" to "swap", "rotate", "flip" buffers etc. This was "yoinked" out of my personal game-engine and was mostly used for image-manipulation.
Some functions are optimized, some are not, some might not even do what they claim to do :)

# Functions and design

All functions come in one `memcpy_` and one `memmove_` flavour where the cpy-one expects non-aliasing buffers.
