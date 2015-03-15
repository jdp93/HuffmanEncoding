Joseph Pallansch
To compile use the command:
make
OR
make compile
OR
g++ string_repr.cpp -o Huffman

To run use the command:
make run
OR
./Huffman

To clean use the command:
make clean
OR
rm Huffman
rm library.txt

This program allows the user to generate a Huffman coding tree using the text in the sample.txt file and prints the tree in a file called library.txt. The data in library.txt is formatted to be the character followed by the path in the tree to that character. I decided to do it this way as it made for easier encoding and decoding. The program can also take in characters as input and encode them into a string of 1s and 0s which it prints to the screen. It also prints out the compression ratio so that the user can see how many fewer bits have been used. Finally, the program allows the user to enter in a string of 1s and 0s and it will decode the string into a string of characters. The program ignores any characters that are not contained in its library.

*Encode does not change any characters that are not in library.txt, but
returns them in their place in the encoded message

*Decode does not change any strings that are not found in library.txt, but
returns them in thier place in the decoded message
