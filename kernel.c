
void kmain(void) {
    const char *str = "my first kernel, thank you arjunsreedharan";
    char *vidptr = (char*)0xb8000; // video memory begins here
    unsigned int i = 0;
    unsigned int j = 0;

    // This loop clears the screen
    // There are 25 lines each 80 columns;
    // each element takes 2 bytes

    while(j < 80 * 25 * 2) {
        // black character
        vidptr[j] = ' ';
        // attribute-byte - light grey on black screen
        vidptr[j+1] = 0x07;
        j = j + 2;
    }
    j = 0;
    // this loop write the string to video memory
    while(str[j] != '\0') {
        // the character's ascii
        vidptr[i] = str[j];
        // attribute-byte: give character black background and light grey fg
        vidptr[i+1] = 0x07;
        ++j;
        i = i + 2;
    }
    return;
}
