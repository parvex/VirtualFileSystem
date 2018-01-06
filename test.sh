#!/bin/sh

VFS_FILE_NAME='drive.vfs'

echo 'Tworze dysk o rozmiarze 10240 bajtow. Posiada on Disc info block + 5 info Blocki + 5 blokow Pamieci'
./vfs $VFS_FILE_NAME create 10240
echo 'Drukuje mape pamieci'
./vfs $VFS_FILE_NAME map

echo 'Zapisuje na dysk plik o rozmiarze 2kb, zajmie on dokladnie 2kb na dysku writualnym'
./vfs $VFS_FILE_NAME push 2kb 2ktest

echo 'Drukuje mape pamieci'
./vfs $VFS_FILE_NAME map

echo 'Zapisuje na dysk plik o rozmiarze 2kb, zajmie on dokladnie 2kb na dysku writualnym'
./vfs $VFS_FILE_NAME push 2kb 2ktest2

echo 'Zapisuje na dysk plik o rozmiarze 2kb+1, zajmie on dokladnie 4kb na dysku writualnym'
./vfs $VFS_FILE_NAME push 2kb1 2k1test

echo 'Drukuje mape pamieci'
./vfs $VFS_FILE_NAME map

echo 'Dodanie pliku 4kb nie powiedzie sie gdyz brakuje miejsca na dysku'
./vfs $VFS_FILE_NAME push 4kb 4k1test

echo 'Usune plik 2kb i wsawie plik 2kb+1, spowoduje to powstanie fragmentacji'

./vfs $VFS_FILE_NAME remove 2ktest

./vfs $VFS_FILE_NAME push 2kb1 2k1

echo 'Drukuje mape pamieci'
./vfs $VFS_FILE_NAME map

echo 'Zapisuje oba pliki na dysku'

./vfs $VFS_FILE_NAME pull 2k1 d2k1
./vfs $VFS_FILE_NAME pull 2k1test d2k1test

echo 'usuwam dysk'

./vfs $VFS_FILE_NAME delete
