cat 2049-03-09_??-??.tsv 2049-03-11_??-??.tsv | awk -F '\t' '$4=="GET" {names[$3] += 1} END {for(key in names){print names[key], key}}' | sort -nk1 | tail

cat *.tsv | awk -F '\t' '{names[$3] += 1} END {cnt=0; for(key in names){if (names[key]>150) cnt++}; print cnt}'