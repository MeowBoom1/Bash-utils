#!/bin/sh

EXPECTED=/tmp/expected.txt
GOT=/tmp/got.txt
Red='\033[0;31m'
Green='\033[0;32m'
NC='\033[0m'

while IFS= read -r line; do
    cat $line > "$EXPECTED" 2> /dev/null < /dev/null
    ./s21_cat $line > "$GOT" 2> /dev/null < /dev/null
    if cmp -s "$EXPECTED" "$GOT"; then
        echo "TEST s21_cat $line ${Green}PASSED${NC}"
    else
        echo "TEST s21_cat $line ${Red}FAILED${NC}"
        
    fi
done <<END
cat_s21.c
-b cat_s21.c
-e cat_s21.c
-n cat_s21.c
-s cat_s21.c
-t cat_s21.c
not_exists.txt
END