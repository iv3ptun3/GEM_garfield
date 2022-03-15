if [ $# -ne 2 ];then
    echo "Usage : ./gen_all.sh [parameter file]"
elif [ ! -e ${1} ];then
    echo "Parameter file ${1} does not exist."
fi

python3 gen_scripts.py ${1} -all
python3 gen_fieldmap.py ${1}