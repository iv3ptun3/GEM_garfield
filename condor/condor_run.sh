
#!/bin/bash
if [ $# -lt 4 ];then
    echo "Usage : ./condor_run.sh [nThreads] [exe] [<arg1> <arg2>...]"
fi
# file name with out path
EXE=${2##*/}
# absolute path of this script
SCRIPT="$(cd "$(dirname "$0")" && pwd)/$(basename "$0")"
SCRIPTPATH=${SCRIPT%/*}
DATE=`date +"%Y%m%d_%H:%M:%S"`
mkdir -p ${SCRIPTPATH}/${EXE}_${DATE}

for (( t=0 ; t < ${1} ; t++ )) ;
do
    EXE_T="${EXE}_`printf %04d $t`"
    echo $EXE_T
    ${SCRIPTPATH}/condor_single.sh "${SCRIPTPATH}/${EXE}_${DATE}/${EXE_T}" $2 $3 $4 $5 $6 $7 $8 
    echo "Jop $t submitted to Condor"
done

