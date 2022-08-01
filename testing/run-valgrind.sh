cd ~/cpp-lib/testing
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./testing.elf out img regexglob
