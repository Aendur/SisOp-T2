
#for i in 0 1 2 3 4 5 6 7 8 9; do ./bin/client > client${i}.log & done
#for i in 0 1 2 3; do ./bin/client slow_ai4.ini > client${i}.log & done
#for i in 5 6    ; do ./bin/client slow_ai2.ini > client${i}.log & done
#for i in 7      ; do ./bin/client slow_ai1.ini > client${i}.log & done

./bin/client > c0.log &
./bin/client > c1.log &
#./bin/client slow_ai2x.ini > c1.log &
#./bin/client slow_ai2y.ini > c2.log &
#./bin/client slow_ai4.ini > c3.log &
