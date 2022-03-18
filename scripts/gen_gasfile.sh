#!/bin/bash
if [ $# -ne 7 ];then
    echo "Usage : source gen_gasfile [arg1] [arg2] [arg3] [arg4] [arg5] [arg6] [arg7]"
    echo "Arguments list : "
    echo "  [arg1] : name of gas1"
    echo "  [arg2] : volume fraction of gas1(in percentage)"
    echo "  [arg3] : name of gas2"
    echo "  [arg4] : volume fraction of gas2(in percentage)"
    echo "  [arg5] : pressure increment"
    echo "  [arg6] : pressure begin"
    echo "  [arg7] : pressure end(the end point is included)"
    echo "This scripts generate gas file iterating with pressures increased by [arg5] starting from [arg6] to [arg7]."
    return
fi

for p in $( seq -f %04g ${6} ${5} ${7} )
do
    echo "Generating gas file ${1}_${2}_${3}_${4}_${p}.gas ..."
    ./gen_gasfile ${1} ${2} ${3} ${4} ${p}  
done