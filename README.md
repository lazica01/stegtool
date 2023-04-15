# stegtool
steganography tool for linux written in c hide a photo within another photo.


Encrypt a photo:
stegtool [hide_in.png] [to_hide.png] [bits]

Decript a photo:
stegtool [try_to_extract.png] [bits]

-bits [1-8] is a number of bits that are going to be changed in a pixel, the less bit's used the higher quality photo will be outputed, but the image file size that can be encrypted will be smaller.
